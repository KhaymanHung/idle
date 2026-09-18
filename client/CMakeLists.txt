add_executable(idle_client
    main.cpp
    client.h
    client.cpp
    clientwindow.h
    clientwindow.cpp
    clientwindow.ui
)

target_include_directories(idle_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(idle_client PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
copy_qt_runtime(idle_client)
