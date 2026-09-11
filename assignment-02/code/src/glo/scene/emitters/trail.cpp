/*
 * glo/scene/emitters/trail.cpp
 *
 * © Tim Clarke November 2025
 */

#include "trail.h"

#include <random>
#include <stdexcept>

#include "../../error.h"
#include "../../log.h"
#include "../node.h"

TrailEmitter::TrailEmitter()
{
    glGenVertexArrays(1, &VAO);
    if (VAO == 0) throw std::runtime_error("Failed to create VAO");
    Log::d("TrailEmitter", std::format("VAO {} created", VAO));

    glGenBuffers(1, &meshVBO);
    if (meshVBO == 0) throw std::runtime_error("Failed to create meshVBO");
    Log::d("TrailEmitter", std::format("meshVBO {} created", meshVBO));

    glGenBuffers(1, &instanceVBO);
    if (instanceVBO == 0)
        throw std::runtime_error("Failed to create instanceVBO");
    Log::d("TrailEmitter", std::format("instanceVBO {} created", instanceVBO));

    GL_ERROR_CHECK;
    particles.reserve(MAX_PARTICLES);

    glBindVertexArray(VAO);
    initialiseMeshBuffer();
    initialiseInstancesBuffer();
    glBindVertexArray(0);
}

/*
 * Initialise the mesh (a simple quad) to be shared by all instances, and stored
 * in layout location 0. This uses a triangle strip for simplicity.
 */
void
TrailEmitter::initialiseMeshBuffer()
{
    // clang-format off
    const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom-left
         0.5f, -0.5f, 0.0f, // bottom-right
        -0.5f,  0.5f, 0.0f, // top-left
         0.5f,  0.5f, 0.0f  // top-right
    };
    // clang-format on

    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (void *)0);
    glEnableVertexAttribArray(0);
    GL_ERROR_CHECK;
}

/*
 * Initialise the data buffer and positions for where the instance
 * data will be sent to the GPU. Layout locations:
 *   position = 1
 *   colour = 2
 *   diameter = 3
 */
void
TrailEmitter::initialiseInstancesBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle), nullptr,
        GL_DYNAMIC_DRAW);
    GL_ERROR_CHECK;

    // === POSITIONS ===
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (void *)POSITION_OFFSET);
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(1);
    GL_ERROR_CHECK;

    // === COLOURS ===
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (void *)COLOUR_OFFSET);
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(2);
    GL_ERROR_CHECK;

    // === DIAMETERS ===
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (void *)DIAMETER_OFFSET);
    glVertexAttribDivisor(3, 1);
    glEnableVertexAttribArray(3);
    GL_ERROR_CHECK;
}

/*
 * Delete our VAO and two VBOs.
 */
TrailEmitter::~TrailEmitter()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &meshVBO);
    glDeleteBuffers(1, &instanceVBO);
}

/*
 * Update our particles, remove any which are beyond their lifetime, and create
 * new particles for the time passed.
 *
 * Optimisation opportunity: reuse dead particles instead of removing them and
 * creating new particles. However, by using the technique that we have, the
 * lifetime of a particle is seperated making the code easier to understand and
 * maintain.
 */
void
TrailEmitter::emit(const GLfloat deltaTime, SceneNode & node)
{
    updateParticles(deltaTime);
    removeDeadParticles();
    createNewParticles(deltaTime, node);
}

/*
 * We change the blend functions and whether or not the depth mask is enabled.
 * So we first save those values before changing them for our needs, then
 * drawing our instances, before restoring the values again.
 */
void
TrailEmitter::render()
{
    if (particles.empty()) return;

    // === ADJUST OPENGL CONFIGURATION ===
    const GLboolean isBlendEnabled = glIsEnabled(GL_BLEND);
    if (!isBlendEnabled) glEnable(GL_BLEND);

    GLint blendFuncSrc, blendFuncDst;
    glGetIntegerv(GL_BLEND_SRC_RGB, &blendFuncSrc);
    glGetIntegerv(GL_BLEND_DST_RGB, &blendFuncDst);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    GLint isDepthMaskEnabled;
    glGetIntegerv(GL_DEPTH_WRITEMASK, &isDepthMaskEnabled);
    if (isDepthMaskEnabled) glDepthMask(GL_FALSE);

    // === DRAW OUR INSTANCES ===
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle),
        particles.data(), GL_DYNAMIC_DRAW);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles.size());
    glBindVertexArray(0);

    // === RESTORE OPENGL CONFIGURATION ===
    if (!isBlendEnabled) glDisable(GL_BLEND);
    glBlendFunc(blendFuncSrc, blendFuncDst);
    if (isDepthMaskEnabled) glDepthMask(GL_TRUE);
}

/*
 * Update our particle data. First update the ttl, and if it is now dead
 * continue straight to the next particle.
 *
 * If the particle lives, slightly lower its vertical position (y), reduce it in
 * size, and make it more transparent. Thus creating a fading effect.
 */
void
TrailEmitter::updateParticles(const GLfloat deltaTime)
{
    for (auto & particle : particles)
    {
        particle.ttl -= deltaTime;
        if (particle.ttl <= 0.0f) continue;

        particle.position.y -= FALLING_VELOCITY * deltaTime;

        const GLfloat ttlFraction = particle.ttl / INITIAL_TTL;
        particle.diameter = INITIAL_SIZE * ttlFraction;
        particle.colour.a = INITIAL_ALPHA * ttlFraction;
    }
}

/*
 * Remove any particles whose ttl is less than, or equal to, 0.0f.
 */
void
TrailEmitter::removeDeadParticles()
{
    const auto isDeadPredicate = [](const Particle & p) -> bool
    { return p.ttl <= 0.0f; };

    const auto isDeadIterator =
        std::remove_if(particles.begin(), particles.end(), isDeadPredicate);

    particles.erase(isDeadIterator, particles.end());
}

/*
 * Create new particles with some randomisation. The number to create is the
 * EMISSION_RATE (per second) multiplied by the delta time. Particles all start
 * at the same size and lifetime.
 *
 * glm::mix is used to interpolate each particle's start position according to
 * the orb's previous and current position.
 */
void
TrailEmitter::createNewParticles(const GLfloat deltaTime,
    const SceneNode & node)
{
    const glm::vec3 currOrbPos = node.getWorldPosition();
    
    if (prevOrbPos == glm::vec3(0.0f))
    {
        prevOrbPos = currOrbPos;
        return;
    }

    const GLuint maxParticles = MAX_PARTICLES - particles.size();
    GLuint toEmit = static_cast<GLuint>(deltaTime * EMISSION_RATE);
    toEmit = glm::min(toEmit, maxParticles);

    for (auto particleIdx = 0; particleIdx < toEmit; particleIdx++)
    {
        const GLfloat timeFraction =
            static_cast<GLfloat>(particleIdx) / static_cast<GLfloat>(toEmit);
        addParticle(glm::mix(prevOrbPos, currOrbPos, timeFraction));
    }

    prevOrbPos = currOrbPos;
}

void
TrailEmitter::addParticle(const glm::vec3 & orbPos)
{
    Particle particle;
    glm::vec3 relPos;
    relPos.x = randPos();
    relPos.y = rand(-0.02f, 0.00f);
    relPos.z = randPos();

    particle.position = orbPos + relPos;
    particle.colour = glm::vec4(rand(0.9f, 1.0f), rand(0.7f, 0.9f),
        rand(0.0f, 0.3f), INITIAL_ALPHA);
    particle.diameter = INITIAL_SIZE;
    particle.ttl = INITIAL_TTL;

    particles.push_back(particle);
}

/*
 * Convenience method to return a random float between min and max.
 */
GLfloat
TrailEmitter::rand(const GLfloat min, const GLfloat max)
{
    if (max < min) return rand(max, min);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<GLfloat> dis(min, max);
    return dis(gen);
}

/*
 * Convenience method to return a random float between -0.005f and 0.005f for
 * initial positioning of the particles.
 */
GLfloat
TrailEmitter::randPos()
{
    return rand(-25.0f, 25.0f) / 5000.0f;
}
