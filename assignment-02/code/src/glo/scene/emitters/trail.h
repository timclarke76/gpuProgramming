/*
 * Emits particles from an orb, with slight randomness. Each particle lives for
 * a limited time, and gradually "falls" and fades as it does so. Emission is
 * set to 500 particles per second.
 *
 * Instancing is used for efficiency, reducing the number of calls to the GPU.
 *
 * CITE: I was already intending to drop the particles, and reduce their size,
 * over time. The idea of fading them came from reading about particles in the
 * Learn OpenGL guide (Vries 2020), where I also learned of the GL_ONE blend
 * function. I thought randomising the colour a little would also add some
 * "sparkle" and asked DeepSeek (DeepSeek 2025) to give me a range of individual
 * RGB floats that added together would create a yellow-gold colour.
 *
 * I was struggling with the particles turning invisible at certain camera
 * angles. I reread the Learn OpenGL Guide sections for both particles and
 * instancing. I learned about Billboarding --- I had missed its importance
 * previously. I couldn't see where the guide covered this technique in detail,
 * so used DeepSeek to help me understand the concept.
 *
 * I ended up with a combination of instancing and particles. Because I had been
 * reading both sections from the Learn OpenGL Guide, I thought that my
 * implementation might be wrong, and that I should be using one method or the
 * other (the two sections are separated by 239 pages in the PDF). I searched
 * the internet, and realised that this is considered the best approach. I could
 * have used just particles, as I am only rendering circles, but by using
 * instancing I have more flexibility should I choose to, for example, add a
 * texture or change the shape.
 *
 * I had a problem with clumping of the particles, and so I added a loop and
 * used glm::mix to interpolate between the orb's previous position (saved as a
 * member variable) and the orb's current position.
 */
#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "emitter.h"

class SceneNode;

class TrailEmitter : public Emitter
{
    static constexpr GLuint EMISSION_RATE = 500;
    static constexpr GLfloat EMISSION_INTERVAL = 1.0f / EMISSION_RATE;
    static constexpr GLfloat FALLING_VELOCITY = 0.08f;
    static constexpr GLfloat INITIAL_ALPHA = 0.2f;

    // changed from 2.0f to 5.0f so that the particle drop is more evident.
    static constexpr GLfloat INITIAL_TTL = 5.0f;

    static constexpr GLfloat INITIAL_SIZE = 0.002f;
    static constexpr GLuint MAX_PARTICLES = EMISSION_RATE * INITIAL_TTL;
    static constexpr GLfloat REL_POS_Y = -0.003f;

    struct Particle
    {
        glm::vec3 position;
        glm::vec4 colour;
        GLfloat diameter;
        GLfloat ttl;
    };

    static constexpr GLuint POSITION_OFFSET = 0;
    static constexpr GLuint COLOUR_OFFSET = POSITION_OFFSET + sizeof(glm::vec3);
    static constexpr GLuint DIAMETER_OFFSET = COLOUR_OFFSET + sizeof(glm::vec4);

  public:
    TrailEmitter();
    virtual ~TrailEmitter();
    virtual void emit(const GLfloat deltaTime, SceneNode & node);
    virtual void render();

  private:
    void initialiseMeshBuffer();
    void initialiseInstancesBuffer();
    void updateParticles(const GLfloat deltaTime);
    void removeDeadParticles();
    void createNewParticles(const GLfloat deltaTime, const SceneNode & node);

    void addParticle(const glm::vec3 & orbPos);
    static GLfloat rand(const GLfloat min, const GLfloat max);
    static GLfloat randPos();

    GLuint VAO;
    GLuint meshVBO;
    GLuint instanceVBO;
    std::vector<Particle> particles;

    // retain the previous orb position so that we can interpolate approximate
    // positions since the last frame
    glm::vec3 prevOrbPos{0.0f};
};
