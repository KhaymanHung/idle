add_executable(idle_server
    main.cpp
    server.h
    server.cpp
    serverwindow.h
    serverwindow.cpp
    serverwindow.ui
)

target_include_directories(idle_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(idle_server PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
copy_qt_runtime(idle_server)
