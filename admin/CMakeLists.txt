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
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG f091b2c061b307ee89b164c39976fc9202a1c79d) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(json)

# --- Source Files -------------------------------------------------------------
set(SOURCES
    main.cpp
    data.cpp

    # API layer
    api/account.cpp
    api/authentication.cpp
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
    PRIVATE cpr::cpr
    PRIVATE nlohmann_json::nlohmann_json)
