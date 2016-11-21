cmake_minimum_required(VERSION 3.3)
project(feednet)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# Tell CMake to fetch info from resources.qrc
set(CMAKE_AUTORCC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Specify directory with Qt cmake-configs location
set(CMAKE_PREFIX_PATH /usr/local/Qt5.7.0/5.7/clang_64/lib/cmake)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Svg_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Svg_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Svg_EXECUTABLE_COMPILE_FLAGS}")

# Bundle settigns
SET(MACOSX_BUNDLE_ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/icon.icns)

set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon.icns)
set_source_files_properties(${myApp_ICON} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")


set(SOURCE_FILES
        src/main.cpp
        src/MainWindow.cpp
        src/MainWindow.h
        src/DrawingArea.cpp
        src/DrawingArea.h
        src/CharactPanel.cpp
        src/CharactPanel.h
        src/gui_widgets.cpp
        src/gui_widgets.h
        src/gui_properties.h
        src/Network.cpp
        src/Network.h
        src/dataset_widget.cpp
        src/dataset_widget.h
        src/data_api.cpp
        src/data_api.h
        src/windows.cpp
        src/windows.h
        src/DSPreviewWidget.cpp
        src/DSPreviewWidget.h
        src/visualization.cpp
        src/visualization.h
        src/DatasetHouse.cpp
        src/DatasetHouse.h
        src/VisualNet.cpp
        src/VisualNet.h
        src/gui_properties.cpp
        src/matrix_api.cpp
        src/matrix_api.h
        resources/resources.qrc)

# Remove MACOSX_BUNDLE when working from inside Clion
add_executable(feednet MACOSX_BUNDLE ${SOURCE_FILES} ${ICON_PATH})

# Comment the block below when working from inside Clion

# block starts
#[[
set( MACOSX_BUNDLE_ICON_FILE icon.icns )

add_custom_target( osx_bundle_dirs
        COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/feednet.app/Contents/Resources
        COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/feednet.app/Contents/MacOS
        COMMAND cp ../../${MACOSX_BUNDLE_ICON_FILE}
        ${CMAKE_CURRENT_BINARY_DIR}/feednet.app/Contents/Resources/${MACOSX_BUNDLE_ICON_FILE}
        )

add_dependencies( feednet osx_bundle_dirs )

# block ends
]]

set_source_files_properties( ${ProjectName_RESOURCES} ${feednet_TRANSLATIONS}
        PROPERTIES MACOSX_PACKAGE_LOCATION Resources )

# The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
target_link_libraries(feednet ${Qt5Widgets_LIBRARIES})
target_link_libraries(feednet ${Qt5Svg_LIBRARIES})