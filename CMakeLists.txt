cmake_minimum_required(VERSION 3.25)
project(Richcord)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(RICHCORD_ROOT_PATH "${CMAKE_SOURCE_DIR}/src")
set(RICHCORD_CORE_INCLUDE_PATH "${RICHCORD_ROOT_PATH}/core")
set(RICHCORD_WINDOW_INCLUDE_PATH "${RICHCORD_ROOT_PATH}/window")

set(RICHCORD_CORE_SOURCES
        "${RICHCORD_CORE_INCLUDE_PATH}/app.cpp"
        )
set(RICHCORD_WINDOW_SOURCES
        "${RICHCORD_WINDOW_INCLUDE_PATH}/mainwindow.cpp"
        )
set(RICHCORD_UI_FILES
        "${RICHCORD_WINDOW_INCLUDE_PATH}/mainwindow.ui"
        )
set(RICHCORD_TARGET "richcord")

add_subdirectory(gamesdk)

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS
        ${RICHCORD_ROOT_PATH}
        ${RICHCORD_CORE_INCLUDE_PATH}
        ${RICHCORD_WINDOW_INCLUDE_PATH}
        )
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
        )
qt6_wrap_ui(${RICHCORD_WINDOW_SOURCES}
        ${RICHCORD_UI_SOURCES}
        )
add_executable(${RICHCORD_TARGET}
        ${RICHCORD_UI_FILES}
        ${RICHCORD_CORE_SOURCES}
        ${RICHCORD_WINDOW_SOURCES}
        "${RICHCORD_ROOT_PATH}/main.cpp"
        "${RICHCORD_ROOT_PATH}/version.rc"
        )
set_target_properties(${RICHCORD_TARGET} PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
        )
target_include_directories(${RICHCORD_TARGET} PRIVATE
        "${CMAKE_AUTOGEN_BUILD_DIR}/include"
        ${RICHCORD_CORE_INCLUDE_PATH}
        ${RICHCORD_WINDOW_INCLUDE_PATH}
        )
target_link_libraries(${RICHCORD_TARGET}
        richcord-gamesdk
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

if (WIN32)
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
        add_custom_command(TARGET ${RICHCORD_TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${RICHCORD_TARGET}>/plugins/platforms/")
        add_custom_command(TARGET ${RICHCORD_TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${RICHCORD_TARGET}>/plugins/platforms/")
    endif ()
    add_custom_command(TARGET ${RICHCORD_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DISCORD_GAMESDK_BIN_RUNTIME}"
            $<TARGET_FILE_DIR:${RICHCORD_TARGET}>)
    add_custom_command(TARGET ${RICHCORD_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DISCORD_GAMESDK_LIB_BIN_RUNTIME}"
            $<TARGET_FILE_DIR:${RICHCORD_TARGET}>)
    # CMake crashes at this point, temporarily commenting for now.
    # foreach (QT_LIB Core Gui Widgets)
    #     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    #             COMMAND ${CMAKE_COMMAND} -E copy
    #             "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
    #             "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    # endforeach (QT_LIB)
endif ()
