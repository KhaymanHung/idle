cmake_minimum_required(VERSION 3.16)

project(idle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network)

function(copy_qt_runtime target_name)
    if(WIN32)
        get_filename_component(QT_PREFIX "${Qt5_DIR}/../../.." ABSOLUTE)
        set(QT_PLUGIN_DIR "${QT_PREFIX}/plugins")

        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target_name}>/platforms"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_PLUGIN_DIR}/platforms/qwindows.dll"
                "$<TARGET_FILE_DIR:${target_name}>/platforms/qwindows.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_PLUGIN_DIR}/platforms/qminimal.dll"
                "$<TARGET_FILE_DIR:${target_name}>/platforms/qminimal.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:Qt5::Core>"
                "$<TARGET_FILE_DIR:${target_name}>"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:Qt5::Gui>"
                "$<TARGET_FILE_DIR:${target_name}>"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:Qt5::Widgets>"
                "$<TARGET_FILE_DIR:${target_name}>"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:Qt5::Network>"
                "$<TARGET_FILE_DIR:${target_name}>"
        )
    endif()
endfunction()

add_subdirectory(server)
add_subdirectory(client)
