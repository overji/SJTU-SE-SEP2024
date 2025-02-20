cmake_minimum_required(VERSION 3.27)
project(MiniBasic)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/qt/6.6.2/mingw_64/bin")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(TestMidFix
        Expression.cpp
        Expression.h
        Variable.h
        Variable.cpp
        Statement.cpp
        Statement.h
        Program.cpp
        Program.h
)

add_executable(MiniBasic
        main.cpp
        mainwindow.cpp
        mainwindow.ui
        CodesInCodeDisplay.h
        CodesInCodeDisplay.cpp
        cmdInput.h
        cmdInput.cpp
        util.h
        Expression.h
        Expression.cpp
        Variable.h
        Variable.cpp
        Statement.h
        Statement.cpp
        Program.h
        Program.cpp
        util.cpp
)

add_executable(TestStatement
        Statement.h
        Statement.cpp
        Expression.h
        Expression.cpp
        Variable.h
        Variable.cpp
)

target_link_libraries(TestMidFix
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_link_libraries(MiniBasic
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_link_libraries(TestStatement
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()