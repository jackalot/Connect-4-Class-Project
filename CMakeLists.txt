cmake_minimum_required(VERSION 3.16)

project(COP_II VERSION 0.1 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt automatic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt — Qt6 or Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Source files (common to both Qt5 and Qt6)
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    battleship.cpp
    battleship.h
    battleship.ui

    boardgrid.cpp
    boardgrid.h

    Connect4.cpp
    Connect4.h
    Connect4.ui

    tictactoe.cpp
    tictactoe.h
    tictactoe.ui

    # Files previously only in Qt5 path:
    Coordinates.h
    ShipSystem.h
    ShipSystem.cpp
    SetOfCoords.h
)

# TARGET
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(COP_II
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(COP_II
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(COP_II PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(COP_II)
endif()
