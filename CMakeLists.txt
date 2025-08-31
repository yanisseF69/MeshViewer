cmake_minimum_required(VERSION 3.19)
project(MeshViewer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGL Widgets OpenGLWidgets)

qt_standard_project_setup()

qt_add_executable(MeshViewer
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    openGLWidget.h
    openGLWidget.cpp
    camera.h
    camera.cpp
    vertex.h
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
        Qt::Widgets
        Qt::OpenGLWidgets
)

file(GLOB SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/shaders")
foreach(SHADER_FILE ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    configure_file(${SHADER_FILE} ${CMAKE_CURRENT_BINARY_DIR}/shaders/${NAME})
endforeach()

include(GNUInstallDirs)

install(TARGETS MeshViewer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MeshViewer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
