cmake_minimum_required(VERSION 3.27)
project(QLink)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/qt/6.6.2/mingw_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Test
        REQUIRED)

add_executable(QLink main.cpp
        LinkGame.cpp
        LinkGame.h
        BoxOfGame.cpp
        BoxOfGame.h.
        Player.cpp
        Player.h
        SelectChecker.cpp
        SelectChecker.h
        SpecialAlgorithm.h
        Gadget.cpp
        Gadget.h
        SaveSystem.cpp
        SaveSystem.h
        MainPage.cpp
        MainPage.h
        SavePage.h
        SavePage.cpp
)
add_executable(QLinkTest
        SimpleTest.cpp
        SimpleTest.h
        LinkGame.cpp
        LinkGame.h
        BoxOfGame.cpp
        BoxOfGame.h
        Player.cpp
        Player.h
        SelectChecker.cpp
        SelectChecker.h
        SpecialAlgorithm.h
        Gadget.cpp
        Gadget.h
        SaveSystem.cpp
        SaveSystem.h
        MainPage.cpp
        MainPage.h
        SavePage.h
        SavePage.cpp
)

target_link_libraries(QLink
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Test
)
target_link_libraries(QLinkTest
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Test
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
