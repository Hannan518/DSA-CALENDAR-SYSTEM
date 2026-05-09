cmake_minimum_required(VERSION 3.5)

project(CalendarSystem LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

set(PROJECT_SOURCES
    main.cpp
    calendar.cpp
    event.cpp
    file_manager.cpp
    mainwindow.cpp
    addeventdialog.cpp
)

set(PROJECT_HEADERS
    calendar.h
    event.h
    file_manager.h
    ds_lib.h
    mainwindow.h
    addeventdialog.h
    eventcalendar.h
)

add_executable(CalendarSystem ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_link_libraries(CalendarSystem PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core)

if(WIN32)
    set_property(TARGET CalendarSystem PROPERTY WIN32_EXECUTABLE ON)
endif()