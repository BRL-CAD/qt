cmake_minimum_required(VERSION 3.16)
project(shortcuteditor LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_executable(shortcuteditor
    actionmanager.cpp actionmanager.h
    application.cpp application.h
    main.cpp
    mainwindow.cpp mainwindow.h
    shortcuteditordelegate.cpp shortcuteditordelegate.h
    shortcuteditormodel.cpp shortcuteditormodel.h
    shortcuteditorwidget.cpp shortcuteditorwidget.h
)

set_target_properties(shortcuteditor PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(shortcuteditor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

install(TARGETS shortcuteditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET shortcuteditor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
