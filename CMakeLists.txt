cmake_minimum_required(VERSION 3.21)
project(Ezsnagit VERSION 0.9.1 LANGUAGES CXX)

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
    src/core/Version.cpp src/core/Version.h
    src/core/Settings.cpp src/core/Settings.h)
target_include_directories(ezsnag_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ezsnag_core PUBLIC Qt6::Core)

# --- Capture: chụp màn hình (geometry thuần + lớp platform GDI) -> static lib.
add_library(ezsnag_capture STATIC
    src/capture/ScreenGeometry.cpp src/capture/ScreenGeometry.h
    src/capture/ScreenCapture.cpp src/capture/ScreenCapture.h
    src/capture/MonitorMap.cpp src/capture/MonitorMap.h
    src/capture/Stitch.cpp src/capture/Stitch.h
    src/capture/StitchMany.cpp src/capture/StitchMany.h)
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

# --- Canvas: document model editor (object graph vector + undo/redo + .ezsnagx) -> static lib.
add_library(ezsnag_canvas STATIC
    src/canvas/CanvasDocument.cpp src/canvas/CanvasDocument.h src/canvas/AnnotationObject.h
    src/canvas/EzsnagxFile.cpp src/canvas/EzsnagxFile.h
    src/canvas/QuickStyle.cpp src/canvas/QuickStyle.h)
target_include_directories(ezsnag_canvas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ezsnag_canvas PUBLIC Qt6::Core Qt6::Gui)

# --- Effects: biến đổi ảnh (crop/rotate/flip/resize/trim) thuần -> static lib.
add_library(ezsnag_effects STATIC
    src/effects/Transforms.cpp src/effects/Transforms.h
    src/effects/Blur.cpp src/effects/Blur.h
    src/effects/Filters.cpp src/effects/Filters.h
    src/effects/Frame.cpp src/effects/Frame.h
    src/effects/RoundedCorners.cpp src/effects/RoundedCorners.h)
target_include_directories(ezsnag_effects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ezsnag_effects PUBLIC Qt6::Core Qt6::Gui)

# --- App GUI (shell P0; các module capture/canvas/... thêm dần theo roadmap) ---
qt_add_executable(Ezsnagit WIN32
    src/main.cpp
    src/ui/MainWindow.cpp src/ui/MainWindow.h
    src/ui/RegionOverlay.cpp src/ui/RegionOverlay.h
    src/ui/WindowPickerOverlay.cpp src/ui/WindowPickerOverlay.h
    src/ui/Magnifier.cpp src/ui/Magnifier.h
    src/ui/CanvasWidget.cpp src/ui/CanvasWidget.h
    src/ui/StyleBar.cpp src/ui/StyleBar.h
    src/ui/PenWidthPicker.cpp src/ui/PenWidthPicker.h
    src/ui/PanoramicDialog.cpp src/ui/PanoramicDialog.h
    src/ui/RecentTray.cpp src/ui/RecentTray.h
    src/ui/PreferencesDialog.cpp src/ui/PreferencesDialog.h
    resources.qrc
    app.rc)                                   # app.rc: icon cho .exe (Windows)
target_include_directories(Ezsnagit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(Ezsnagit PRIVATE EZSNAGIT_VERSION="${PROJECT_VERSION}")
target_link_libraries(Ezsnagit PRIVATE ezsnag_core ezsnag_capture ezsnag_hotkey ezsnag_share ezsnag_effects ezsnag_canvas Qt6::Core Qt6::Gui Qt6::Widgets)

# --- Tests (console, headless) ---
enable_testing()
add_executable(test_core tests/test_core.cpp)
target_link_libraries(test_core PRIVATE ezsnag_core Qt6::Core)
add_test(NAME core COMMAND test_core)

add_executable(test_settings tests/test_settings.cpp)
target_link_libraries(test_settings PRIVATE ezsnag_core Qt6::Core)
add_test(NAME settings COMMAND test_settings)

add_executable(test_capture tests/test_capture.cpp)
target_link_libraries(test_capture PRIVATE ezsnag_capture Qt6::Core Qt6::Gui)
add_test(NAME capture COMMAND test_capture)

add_executable(test_monitormap tests/test_monitormap.cpp)
target_link_libraries(test_monitormap PRIVATE ezsnag_capture Qt6::Core Qt6::Gui)
add_test(NAME monitormap COMMAND test_monitormap)

add_executable(test_share tests/test_share.cpp)
target_link_libraries(test_share PRIVATE ezsnag_share Qt6::Core Qt6::Gui)
add_test(NAME share COMMAND test_share)

add_executable(test_canvas tests/test_canvas.cpp)
target_link_libraries(test_canvas PRIVATE ezsnag_canvas Qt6::Core Qt6::Gui)
add_test(NAME canvas COMMAND test_canvas)

add_executable(test_effects tests/test_effects.cpp)
target_link_libraries(test_effects PRIVATE ezsnag_effects Qt6::Core Qt6::Gui)
add_test(NAME effects COMMAND test_effects)

add_executable(test_blur tests/test_blur.cpp)
target_link_libraries(test_blur PRIVATE ezsnag_effects Qt6::Core Qt6::Gui)
add_test(NAME blur COMMAND test_blur)

add_executable(test_filters tests/test_filters.cpp)
target_link_libraries(test_filters PRIVATE ezsnag_effects Qt6::Core Qt6::Gui)
add_test(NAME filters COMMAND test_filters)

add_executable(test_frame tests/test_frame.cpp)
target_link_libraries(test_frame PRIVATE ezsnag_effects Qt6::Core Qt6::Gui)
add_test(NAME frame COMMAND test_frame)

add_executable(test_rounded tests/test_rounded.cpp)
target_link_libraries(test_rounded PRIVATE ezsnag_effects Qt6::Core Qt6::Gui)
add_test(NAME rounded COMMAND test_rounded)

add_executable(test_stitch tests/test_stitch.cpp)
target_link_libraries(test_stitch PRIVATE ezsnag_capture Qt6::Core Qt6::Gui)
add_test(NAME stitch COMMAND test_stitch)

add_executable(test_stitchmany tests/test_stitchmany.cpp)
target_link_libraries(test_stitchmany PRIVATE ezsnag_capture Qt6::Core Qt6::Gui)
add_test(NAME stitchmany COMMAND test_stitchmany)

add_executable(test_ezsnagx tests/test_ezsnagx.cpp)
target_link_libraries(test_ezsnagx PRIVATE ezsnag_canvas Qt6::Core Qt6::Gui)
add_test(NAME ezsnagx COMMAND test_ezsnagx)

add_executable(test_quickstyle tests/test_quickstyle.cpp)
target_link_libraries(test_quickstyle PRIVATE ezsnag_canvas Qt6::Core Qt6::Gui)
add_test(NAME quickstyle COMMAND test_quickstyle)
