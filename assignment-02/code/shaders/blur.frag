/*
 * shaders/blur.frag
 *
 * Uses a Gaussian blur technique to blur the srcTexture. Blur direction is
 * determined by isHorizontal --- horizontal (true) or vertical (false).
 *
 * CITE: I originally took the code from the Learn OpenGL guide. It is a fixed
 * algorithm, but thought the original code a little messy and difficult to
 * maintain. So I tidied it up by adding appropriate variable usage to make the
 * process clearer, and added comments, all so I could clearly understand the
 * algorithm myself --- which as it became clear, is very simple.
 *
 * The original weights that I used also come from the Learn OpenGL guide. I did
 * some investigation into different Gaussian approaches. From what I learned,
 * the single-pass approach produces similar quality results, but is less
 * efficient (sampling N^2 pixels for each pass, compared to 2N pixels for the
 * two-pass approach (4N total)).
 *
 * I also learned that there are, for example, 5-Tap Kernel and Optimised 5-Tap
 * Kernel sets of weights --- the Optimised versions use computationally cheaper
 * numbers, with a negligible drop in quality.
 *
 * I experimented with Optimised 3-, 5-, 7-, and 9-Tap weights, which I asked
 * DeepSeek (DeepSeek 2025) for. I felt the 9-Tap weights offered a significant
 * quality improvement which outweighed the extra computational cost over the
 * alternatives that I tried.
 *
 * However, I was keen to allow the amount of bloom to be adjusted via user
 * input, and so I rewrote the code to remove the horizontal/vertical
 * duplication (by introducing the direction variable), and worked around the
 * nature of GLSL's array handling by introducing switch statements. This
 * sacrifices a little performance for improved flexibility and maintainability.
 *
 * © Tim Clarke November 2025
 */

#version 420 core

uniform sampler2D srcTexture;
uniform bool isHorizontal;
uniform int tapCount;  // 3, 5, 7, or 9

in vec2 v_TextureCoord;

out vec4 o_Colour;

void main()
{
    const vec2 texelOffset = 1.0 / vec2(textureSize(srcTexture, 0));
    const vec2 direction = isHorizontal ? vec2(texelOffset.x, 0.0)
        : vec2(0.0, texelOffset.y);

    // 3-Tap: [1,2,1] / 4
    const float weights3[2] = float[]
    (
        0.5,  // centre
        0.25  // offset 1
    );

    // 5-Tap: [1,4,6,4,1] / 16
    const float weights5[3] = float[]
    (
        0.375,  // centre
        0.25,   // offset 1  
        0.0625  // offset 2
    );

    // 7-Tap: [1,6,15,20,15,6,1] / 64
    const float weights7[4] = float[]
    (
        0.3125,  // centre
        0.234375,// offset 1
        0.09375, // offset 2
        0.015625 // offset 3
    );

    // 9-Tap: [1,8,28,56,70,56,28,8,1] / 256  
    const float weights9[5] = float[]
    (
        0.2734375, // centre
        0.21875,   // offset 1
        0.109375,  // offset 2
        0.03125,   // offset 3
        0.00390625 // offset 4
    );

    vec3 centreColour = texture(srcTexture, v_TextureCoord).rgb;
    int numWeights;
    float adjWeight;

    switch (tapCount)
    {
        case 3:
        {
            centreColour *= weights3[0];
            numWeights = 2;
        }
        break;

        case 5:
        {
            centreColour *= weights5[0];
            numWeights = 3;
        }
        break;

        case 7:
        {
            centreColour *= weights7[0];
            numWeights = 4;
        }
        break;

        default: // 9
        {
            centreColour *= weights9[0];
            numWeights = 5;
        }
        break;
    }

    for (int weightIdx = 1; weightIdx < numWeights; weightIdx++)
    {
        switch (tapCount)
        {
            case 3:  adjWeight = weights3[weightIdx]; break;
            case 5:  adjWeight = weights5[weightIdx]; break;
            case 7:  adjWeight = weights7[weightIdx]; break;
            default: adjWeight = weights9[weightIdx]; break;
        }

        const vec2 coordOffset = direction * float(weightIdx);

        vec3 adjColour = texture(srcTexture, v_TextureCoord - coordOffset).rgb;
        adjColour *= adjWeight;
        centreColour += adjColour;

        adjColour = texture(srcTexture, v_TextureCoord + coordOffset).rgb;
        adjColour *= adjWeight;
        centreColour += adjColour;
    }

    o_Colour = vec4(centreColour, 1.0);
}
