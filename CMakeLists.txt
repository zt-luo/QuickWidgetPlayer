cmake_minimum_required(VERSION 3.10)

# project name
project("QuickWidgetPlayer")

# find includes in the corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

####################
######## Qt ########

# run moc automatically
# set(CMAKE_AUTOMOC ON)
# run uic automatically
# set(CMAKE_AUTOUIC ON)
# run rcc automatically
# set (CMAKE_AUTORCC ON)

message("Looking for Qt...")
# Qt modules (https://doc.qt.io/qt-5/qtmodules.html)
find_package(Qt5 REQUIRED Widgets Core Gui QuickWidgets Qml)
if (${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message("Couldn't find Qt")
endif()

link_libraries(
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::QuickWidgets
    Qt5::Qml
    )

qt5_wrap_ui(ui_list 
    "mainwindow.ui"
    )

qt5_wrap_cpp(moc_list
    "mainwindow.h"
)

qt5_add_resources(qrc_list 
    "ui.qrc")

######## Qt ########
####################


# check and search moudles
find_package(PkgConfig REQUIRED)
pkg_search_module(GST REQUIRED gstreamer-1.0)
pkg_search_module(GST_VIDEO REQUIRED gstreamer-video-1.0)
pkg_search_module(GST_APP REQUIRED gstreamer-app-1.0)


include_directories(
    ${GST_INCLUDE_DIRS} ${GST_VIDEO_INCLUDE_DIRS} ${GST_APP_INCLUDE_DIRS} 
    )

link_libraries(
    ${GST_LDFLAGS} ${GST_VIDEO_LDFLAGS} ${GST_APP_LDFLAGS} 
    )

# source files
set(src_list
    "main.cpp"
    "mainwindow.cpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${src_list} ${ui_list} ${moc_list} ${qrc_list})
