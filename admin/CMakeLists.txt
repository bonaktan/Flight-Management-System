cmake_minimum_required(VERSION 3.25)
project(SkyBridgeAirways-AdminTUI
    LANGUAGES CXX
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Fetch FTXUI --------------------------------------------------------------
find_package(ftxui CONFIG REQUIRED)

# --- Source Files -------------------------------------------------------------
set(SOURCES
    main.cpp
    data.cpp

    # API layer
    api/account.cpp
    api/airplane.cpp
    api/airport.cpp
    api/booking.cpp
    api/flight.cpp
    api/passenger.cpp
    api/seatclass.cpp
    api/staff.cpp

    # Controls
    controls/escape.cpp
    controls/input.cpp

    # Display
    display/helper.cpp
    display/menu.cpp
)

add_executable(SkyBridgeAirways-AdminTUI ${SOURCES})

# --- Include Directories ------------------------------------------------------
target_include_directories(SkyBridgeAirways-AdminTUI
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/api
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/controls
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/display
)

# --- Link Libraries -----------------------------------------------------------
target_link_libraries(SkyBridgeAirways-AdminTUI
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)