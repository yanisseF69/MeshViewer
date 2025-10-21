cmake_minimum_required(VERSION 3.19)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)

add_executable(MeshViewerTests
    test_mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/vertex.cpp
    ${PROJECT_SOURCE_DIR}/src/triangle.cpp
    ${PROJECT_SOURCE_DIR}/src/edgeKeyHash.cpp

)

target_include_directories(MeshViewerTests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(MeshViewerTests
    PRIVATE
        GTest::gtest_main
        Qt::Core
        Qt::Widgets
        Qt::OpenGL
        Qt::OpenGLWidgets
)

include(GoogleTest)
gtest_discover_tests(MeshViewerTests)

add_custom_command(
    TARGET MeshViewerTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/data/test
        ${CMAKE_CURRENT_BINARY_DIR}/data/test
)
