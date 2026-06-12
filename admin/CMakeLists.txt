cmake_minimum_required(VERSION 3.25)
project(SkyBridgeAirways-AdminTUI
    LANGUAGES CXX
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Cross-Compoilation Options -----------------------------------------------
set(CPR_USE_SYSTEM_CURL OFF)
set(CURL_USE_SCHANNEL ON) 

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CPR_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CURL_STATICLIB ON CACHE BOOL "" FORCE)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9  # Replace with a version, tag, or commit hash
)
FetchContent_MakeAvailable(ftxui)

FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG f091b2c061b307ee89b164c39976fc9202a1c79d
)
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
    api/client.cpp
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
target_link_options(SkyBridgeAirways-AdminTUI 
    PRIVATE -static -static-libgcc -static-libstdc++
)
target_link_libraries(SkyBridgeAirways-AdminTUI
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
    PRIVATE cpr::cpr
    PRIVATE nlohmann_json::nlohmann_json)
