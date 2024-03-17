cmake_minimum_required(VERSION 3.22)

option(VERBOSE_OUTPUT OFF)

set(PROJECT_NAME EDGSG)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

include(InitVCPKG)
include(Functions)
include(ToolchainUpdate)
include(GitVersion)

# Git Version implementation for CMake
get_version_info("APP")

# Initialize Date and Time variables
init_time_and_date_variables()

project(${PROJECT_NAME} VERSION ${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}.${APP_VERSION_TWEAK} LANGUAGES CXX)

set(EXE_PLATFORM_KEYWORD "")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(EXE_PLATFORM_KEYWORD "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(EXE_PLATFORM_KEYWORD "MACOSX_BUNDLE")
endif()

# Resource file
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Resources/App.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/App.rc")

set(SHADER_FILES
    Resources/Shaders/line-fs.glsl
    Resources/Shaders/line-vs.glsl
    Resources/Shaders/point-fs.glsl
    Resources/Shaders/point-vs.glsl
    Resources/Shaders/triangle-fs.glsl
    Resources/Shaders/triangle-vs.glsl
)

set(SOURCE_FILES
    Source/Main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/App.rc

    # Fonts
    Source/Fonts/FontAwesome.cpp
    Source/Fonts/FontAwesome.h
    Source/Fonts/FontAwesome2.cpp
    Source/Fonts/IconsFontAwesome5.h
    Source/Fonts/Lato.cpp
    Source/Fonts/Lato.h

    # Geometry
    Source/Geometry/AABB.cpp
    Source/Geometry/AABB.h
    Source/Geometry/BasicGeometry.h
    Source/Geometry/Bezier.cpp
    Source/Geometry/Bezier.h
    Source/Geometry/Circle.cpp
    Source/Geometry/Circle.h
    Source/Geometry/Edge3d.cpp
    Source/Geometry/Edge3d.h
    Source/Geometry/EisemannRay.cpp
    Source/Geometry/EisemannRay.h
    Source/Geometry/Intersections3d.h
    Source/Geometry/Line.cpp
    Source/Geometry/Line.h
    Source/Geometry/Line3d.cpp
    Source/Geometry/Line3d.h
    Source/Geometry/Plane.cpp
    Source/Geometry/Plane.h
    Source/Geometry/Point.cpp
    Source/Geometry/Point.h
    Source/Geometry/PointCloud.cpp
    Source/Geometry/PointCloud.h
    Source/Geometry/PointCloud3d.cpp
    Source/Geometry/PointCloud3d.h
    Source/Geometry/Polygon.cpp
    Source/Geometry/Polygon.h
    Source/Geometry/Ray3d.cpp
    Source/Geometry/Ray3d.h
    Source/Geometry/RayLine.cpp
    Source/Geometry/RayLine.h
    Source/Geometry/Segment3d.cpp
    Source/Geometry/Segment3d.h
    Source/Geometry/SegmentLine.cpp
    Source/Geometry/SegmentLine.h
    Source/Geometry/Triangle.cpp
    Source/Geometry/Triangle.h
    Source/Geometry/Triangle3d.cpp
    Source/Geometry/Triangle3d.h
    Source/Geometry/TriangleModel.cpp
    Source/Geometry/TriangleModel.h
    Source/Geometry/Vect2d.cpp
    Source/Geometry/Vect2d.h
    Source/Geometry/Vect3d.cpp
    Source/Geometry/Vect3d.h
    Source/Geometry/Vertex.cpp
    Source/Geometry/Vertex.h
    Source/Geometry/Voxelization.cpp
    Source/Geometry/Voxelization.h
    Source/Geometry/VoxelModel.cpp
    Source/Geometry/VoxelModel.h

    # Patterns
    Source/Patterns/Singleton.h

    # Rendering
    Source/Rendering/ApplicationState.h
    Source/Rendering/Camera.cpp
    Source/Rendering/Camera.h
    Source/Rendering/CameraGuiAdapter.cpp
    Source/Rendering/CameraGuiAdapter.h
    Source/Rendering/CameraProjection.cpp
    Source/Rendering/CameraProjection.h
    Source/Rendering/DrawAABB.cpp
    Source/Rendering/DrawAABB.h
    Source/Rendering/DrawBezier.cpp
    Source/Rendering/DrawBezier.h
    Source/Rendering/DrawCircle.cpp
    Source/Rendering/DrawCircle.h
    Source/Rendering/DrawLine.cpp
    Source/Rendering/DrawLine.h
    Source/Rendering/DrawMesh.cpp
    Source/Rendering/DrawMesh.h
    Source/Rendering/DrawPlane.cpp
    Source/Rendering/DrawPlane.h
    Source/Rendering/DrawPoint.cpp
    Source/Rendering/DrawPoint.h
    Source/Rendering/DrawPointCloud.cpp
    Source/Rendering/DrawPointCloud.h
    Source/Rendering/DrawPolygon.cpp
    Source/Rendering/DrawPolygon.h
    Source/Rendering/DrawRay.cpp
    Source/Rendering/DrawRay.h
    Source/Rendering/DrawSegment.cpp
    Source/Rendering/DrawSegment.h
    Source/Rendering/DrawTriangle.cpp
    Source/Rendering/DrawTriangle.h
    Source/Rendering/DrawVoxelization.cpp
    Source/Rendering/DrawVoxelization.h
    Source/Rendering/FBO.cpp
    Source/Rendering/FBO.h
    Source/Rendering/FBOScreenshot.cpp
    Source/Rendering/FBOScreenshot.h
    Source/Rendering/GUI.cpp
    Source/Rendering/GUI.h
    Source/Rendering/GuiUtilities.cpp
    Source/Rendering/GuiUtilities.h
    Source/Rendering/Image.cpp
    Source/Rendering/Image.h
    Source/Rendering/InputManager.cpp
    Source/Rendering/InputManager.h
    Source/Rendering/InterfaceAdapter.h
    Source/Rendering/Model3D.cpp
    Source/Rendering/Model3D.h
    Source/Rendering/Renderer.cpp
    Source/Rendering/Renderer.h
    Source/Rendering/RenderingShader.cpp
    Source/Rendering/RenderingShader.h
    Source/Rendering/SceneContent.cpp
    Source/Rendering/SceneContent.h
    Source/Rendering/Scenes.cpp
    Source/Rendering/Scenes.h
    Source/Rendering/ShaderProgram.cpp
    Source/Rendering/ShaderProgram.h
    Source/Rendering/ShaderProgramDB.cpp
    Source/Rendering/ShaderProgramDB.h
    Source/Rendering/Texture.cpp
    Source/Rendering/Texture.h
    Source/Rendering/TextureList.cpp
    Source/Rendering/TextureList.h
    Source/Rendering/VAO.cpp
    Source/Rendering/VAO.h
    Source/Rendering/Window.cpp
    Source/Rendering/Window.h

    # Utils
    Source/Utils/ChronoUtilities.h
    Source/Utils/FilesystemUtilities.cpp
    Source/Utils/FilesystemUtilities.h
    Source/Utils/RandomUtilities.h

    # Libraries
    Source/Libraries/ImGuiFileDialog/ImGuiFileDialog.cpp
    Source/Libraries/ImGuiFileDialog/ImGuiFileDialog.h
    Source/Libraries/ImGuiFileDialog/ImGuiFileDialogConfig.h

    # Shaders
    ${SHADER_FILES}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(isMultiConfig)
    # Dummy target to ensure that resources are up-to-date
    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Dummy")
        message(FATAL_ERROR "\"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Dummy\" exists, remove it!")
    endif()

    add_custom_target(
            always_run_resources_target ALL
            DEPENDS
            "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/DummyResourses" # fake! ensure we run!
    )

    add_custom_command(
            OUTPUT
            "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/DummyResourses" # fake! ensure we run!
            "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Resources"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
            "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Resources"
    )
else()
    # Dummy target to ensure that resources are up-to-date
    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/Dummy")
        message(FATAL_ERROR "\"${CMAKE_CURRENT_BINARY_DIR}/Dummy\" exists, remove it!")
    endif()

    add_custom_target(
            always_run_resources_target ALL
            DEPENDS
            "${CMAKE_CURRENT_BINARY_DIR}/DummyResourses" # fake! ensure we run!
    )

    add_custom_command(
            OUTPUT
            "${CMAKE_CURRENT_BINARY_DIR}/DummyResourses" # fake! ensure we run!
            "${CMAKE_CURRENT_BINARY_DIR}/Resources"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
            "${CMAKE_CURRENT_BINARY_DIR}/Resources"
    )
endif()

# Find packages
find_package(assimp CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(lodepng CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_path(DIRENT_INCLUDE_DIRS "dirent.h")

if (MSVC)
    add_compile_definitions(
        NOMINMAX                # suppress min/max macros conflicting with names from std::
        _USE_MATH_DEFINES       # ensure standard math consts such as M_PI are defined
        _CRT_SECURE_NO_WARNINGS # suppress warnings about unsafe functions
    )
endif()

# Add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} always_run_resources_target)

# Use WinMain as a default endpoint on Windows
if(WIN32)
    # set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE "$<IF:$<CONFIG:Release>,TRUE,${CMAKE_WIN32_EXECUTABLE}>")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

# Enable PDB files in the Release configuration and a couple of useful flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:__cplusplus")
    target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Release>:/Zi>")
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:/DEBUG:FASTLINK>")
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:/OPT:REF>")
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:/OPT:ICF>")
    target_compile_options(${PROJECT_NAME} PUBLIC "/MP")
endif()

target_link_libraries(${PROJECT_NAME}
    assimp::assimp
    GLEW::GLEW
    glfw
    glm::glm-header-only
    imgui::imgui
    imguizmo::imguizmo
    lodepng
    OpenGL::GL
    pugixml::pugixml
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${Stb_INCLUDE_DIR}
    ${DIRENT_INCLUDE_DIRS}
)

if(VERBOSE_OUTPUT)
    cmake_list_all_variables()
endif()
