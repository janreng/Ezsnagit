cmake_minimum_required(VERSION 3.21)
project(Ezsnagit VERSION 0.1.0 LANGUAGES CXX)

# Ezsnagit — clone TechSmith Snagit bằng C++/Qt6 (Windows-first, MinGW).
# Kiến trúc: static lib per module (xem docs/specs/00-overview-architecture.md).
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# --- Core: logic thuần không phụ thuộc GUI (version, util) -> static lib, test headless.
add_library(ezsnag_core STATIC
    src/core/Version.cpp src/core/Version.h)
target_include_directories(ezsnag_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ezsnag_core PUBLIC Qt6::Core)

# --- App GUI (shell P0; các module capture/canvas/... thêm dần theo roadmap) ---
qt_add_executable(Ezsnagit WIN32
    src/main.cpp
    src/ui/MainWindow.cpp src/ui/MainWindow.h
    resources.qrc
    app.rc)                                   # app.rc: icon cho .exe (Windows)
target_include_directories(Ezsnagit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(Ezsnagit PRIVATE EZSNAGIT_VERSION="${PROJECT_VERSION}")
target_link_libraries(Ezsnagit PRIVATE ezsnag_core Qt6::Core Qt6::Gui Qt6::Widgets)

# --- Tests (console, headless) ---
enable_testing()
add_executable(test_core tests/test_core.cpp)
target_link_libraries(test_core PRIVATE ezsnag_core Qt6::Core)
add_test(NAME core COMMAND test_core)
