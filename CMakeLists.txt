cmake_minimum_required(VERSION 3.21)
project(Ezsnagit VERSION 0.1.0 LANGUAGES CXX)

# Ezsnagit — phần mềm chụp & chú thích màn hình, C++/Qt6 (Windows-first, MinGW).
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

# --- Capture: chụp màn hình (geometry thuần + lớp platform GDI) -> static lib.
add_library(ezsnag_capture STATIC
    src/capture/ScreenGeometry.cpp src/capture/ScreenGeometry.h
    src/capture/ScreenCapture.cpp src/capture/ScreenCapture.h
    src/capture/MonitorMap.cpp src/capture/MonitorMap.h)
target_include_directories(ezsnag_capture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ezsnag_capture PUBLIC Qt6::Core Qt6::Gui)
if(WIN32)
    target_link_libraries(ezsnag_capture PUBLIC gdi32 user32 dwmapi)
endif()

# --- Hotkey: phím tắt toàn cục (RegisterHotKey) -> static lib.
add_library(ezsnag_hotkey STATIC src/hotkey/GlobalHotkey.cpp src/hotkey/GlobalHotkey.h)
target_include_directories(ezsnag_hotkey PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ezsnag_hotkey PUBLIC Qt6::Core Qt6::Gui)
if(WIN32)
    target_link_libraries(ezsnag_hotkey PUBLIC user32)
endif()

# --- Share/output: copy clipboard + lưu PNG -> static lib.
add_library(ezsnag_share STATIC src/share/ImageClipboard.cpp src/share/ImageClipboard.h)
target_include_directories(ezsnag_share PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ezsnag_share PUBLIC Qt6::Core Qt6::Gui)

# --- App GUI (shell P0; các module capture/canvas/... thêm dần theo roadmap) ---
qt_add_executable(Ezsnagit WIN32
    src/main.cpp
    src/ui/MainWindow.cpp src/ui/MainWindow.h
    src/ui/RegionOverlay.cpp src/ui/RegionOverlay.h
    src/ui/WindowPickerOverlay.cpp src/ui/WindowPickerOverlay.h
    resources.qrc
    app.rc)                                   # app.rc: icon cho .exe (Windows)
target_include_directories(Ezsnagit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(Ezsnagit PRIVATE EZSNAGIT_VERSION="${PROJECT_VERSION}")
target_link_libraries(Ezsnagit PRIVATE ezsnag_core ezsnag_capture ezsnag_hotkey ezsnag_share Qt6::Core Qt6::Gui Qt6::Widgets)

# --- Tests (console, headless) ---
enable_testing()
add_executable(test_core tests/test_core.cpp)
target_link_libraries(test_core PRIVATE ezsnag_core Qt6::Core)
add_test(NAME core COMMAND test_core)

add_executable(test_capture tests/test_capture.cpp)
target_link_libraries(test_capture PRIVATE ezsnag_capture Qt6::Core Qt6::Gui)
add_test(NAME capture COMMAND test_capture)

add_executable(test_monitormap tests/test_monitormap.cpp)
target_link_libraries(test_monitormap PRIVATE ezsnag_capture Qt6::Core Qt6::Gui)
add_test(NAME monitormap COMMAND test_monitormap)

add_executable(test_share tests/test_share.cpp)
target_link_libraries(test_share PRIVATE ezsnag_share Qt6::Core Qt6::Gui)
add_test(NAME share COMMAND test_share)
