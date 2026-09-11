cmake_minimum_required(VERSION 3.10)
project(assignment-01)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

cmake_policy(SET CMP0072 NEW)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME}
    src/glo/camera.cpp
    src/glo/colour.cpp
    src/glo/log.cpp
    src/glo/projection.cpp
    src/glo/program/program.cpp
    src/glo/program/shader.cpp
    src/glo/window.cpp
    src/glo/mesh/buffers/assembly.cpp
    src/glo/mesh/buffers/colour.cpp
    src/glo/mesh/buffers/index.cpp
    src/glo/mesh/shapes/cube.cpp
    src/glo/mesh/shapes/cylinder.cpp
    src/glo/mesh/shapes/decal.cpp
    src/glo/mesh/shapes/pyramid.cpp
    src/glo/mesh/shapes/sphere.cpp
    src/glo/mesh/shapes/star.cpp
    src/glo/mesh/textures/image.cpp
    src/glo/scene/animators/orb.cpp
    src/glo/scene/animators/ornament.cpp
    src/glo/scene/factories/cot_mobile.cpp
    src/glo/scene/factories/geometric.cpp
    src/glo/scene/factories/train.cpp
    src/glo/scene/node.cpp
    src/scene/scene.cpp
    src/scene/torch.cpp
    src/scene/graphs/main_light.cpp
    src/scene/graphs/objects.cpp
    src/scene/graphs/orbs.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        OpenGL::GL
        GLEW::GLEW
)

target_link_options(${PROJECT_NAME} PRIVATE -rdynamic)

add_custom_command(TARGET assignment-01 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:assignment-01>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/textures
        $<TARGET_FILE_DIR:assignment-01>/textures
)

