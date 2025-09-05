cmake_minimum_required(VERSION 3.19)
project(MeshViewer VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGL Widgets OpenGLWidgets)

qt_standard_project_setup()

set(MESHVIEWER_DATA_DIR "${CMAKE_INSTALL_PREFIX}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Release")
    set(MESHVIEWER_DATA_DIR "${CMAKE_SOURCE_DIR}")
endif()
configure_file(${PROJECT_SOURCE_DIR}/include/config.h.in
    ${PROJECT_SOURCE_DIR}/include/config.h)



include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/mainwindow.ui
    src/mainwindow.cpp
    src/openGLWidget.cpp
    src/camera.cpp
    src/vertex.cpp
    src/triangle.cpp
    src/mesh.cpp
)

set(HEADERS
    include/mainwindow.h
    include/openGLWidget.h
    include/camera.h
    include/vertex.h
    include/triangle.h
    include/mesh.h
    include/config.h
)


qt_add_executable(MeshViewer WIN32 MACOSX_BUNDLE
    ${SOURCES} ${HEADERS}
    include/config.h.in
)

target_include_directories(MeshViewer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MeshViewer
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::OpenGL
        Qt::OpenGLWidgets
)

file(GLOB SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/shaders")
foreach(SHADER_FILE ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    configure_file(${SHADER_FILE} ${CMAKE_CURRENT_BINARY_DIR}/shaders/${NAME})
endforeach()

# file(GLOB DATA "${CMAKE_CURRENT_SOURCE_DIR}/data/*")
# file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/data")
# foreach(DATA_FILE ${DATA})
#     get_filename_component(DATA_NAME ${DATA_FILE} NAME)
#     configure_file(${DATA_FILE} ${CMAKE_CURRENT_BINARY_DIR}/data/${NAME})
# endforeach()

include(GNUInstallDirs)

install(TARGETS MeshViewer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        DESTINATION .)

qt_generate_deploy_app_script(
    TARGET MeshViewer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
