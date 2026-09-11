/*
 * glo/mesh/buffers/colour.cpp
 *
 * © Tim Clarke November 2025
 */

#include "colour.h"

#include <cassert>
#include <utility>

/*
 * colour: the single colour to be held in the buffer. Multi-coloured
 * buffers are not yet supported (TODO).
 *
 * size: how many vertices this buffer is for. Must be greater than zero.
 */
ColourBuffer::ColourBuffer(const std::shared_ptr<Colour> colour,
    const GLsizei size)
{
    assert(size > 0);

    std::vector<glm::vec4> colours;
    colours.reserve(size);

    for (GLsizei i = 0; i < size; i++)
        colours.emplace_back(colour->getRgba());

    setData(std::move(colours));
}
