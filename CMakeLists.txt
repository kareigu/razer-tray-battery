cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_FLAGS "--target=x86_64-pc-windows-msvc")


project(razer-tray-battery VERSION 0.1 LANGUAGES CXX)
add_compile_definitions(PROJECT_VERSION="v${PROJECT_VERSION}")
add_compile_definitions(FMT_EXCEPTIONS=0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif()

message("Compiler: ${CMAKE_CXX_COMPILER_ID}")
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
link_directories(hidapi/x64)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.0.0
)
FetchContent_MakeAvailable(fmt)

set(PROJECT_SOURCES
        src/main.cpp
        src/window.cpp
        src/tray_icon.cpp
        src/usb_device.cpp
        src/razer_report.cpp
)

set(PROJECT_RESOURCES
        res/resources.qrc
)

qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_RESOURCES}
)
target_include_directories(${PROJECT_NAME} PRIVATE hidapi/include)


target_link_libraries(${PROJECT_NAME} PRIVATE 
        Qt6::Widgets 
        Qt6::Network
        hidapi
        fmt
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

qt_finalize_executable(${PROJECT_NAME})

add_custom_target(run 
        COMMAND ${CMAKE_COMMAND} -E env QT_WIN_DEBUG_CONSOLE="attach" "./${PROJECT_NAME}.exe"
        USES_TERMINAL
        DEPENDS ${PROJECT_NAME}
)

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "${PROJECT_SOURCE_DIR}/hidapi/x64/hidapi.dll" 
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "${PROJECT_SOURCE_DIR}/hidapi/x64/hidapi.pdb" 
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                $<TARGET_FILE:fmt>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
