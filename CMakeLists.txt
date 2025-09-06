cmake_minimum_required(VERSION 3.16)
project(rss VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(rss WIN32 MACOSX_BUNDLE
    articlewidget.cpp articlewidget.h articlewidget.ui
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    post.cpp post.h
    postwidget.cpp postwidget.h postwidget.ui
)
target_link_libraries(rss PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(rss PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS rss
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET rss
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
