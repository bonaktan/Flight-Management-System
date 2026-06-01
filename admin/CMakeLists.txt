cmake_minimum_required (VERSION 3.25)

project(SkyBridgeAirways-AdminTUI
  LANGUAGES CXX
  VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# --- Fetch FTXUI --------------------------------------------------------------

# ------------------------------------------------------------------------------
find_package(ftxui CONFIG REQUIRED)

add_executable(SkyBridgeAirways-AdminTUI main.cpp)

target_link_libraries(SkyBridgeAirways-AdminTUI
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component  # Not needed for this example.
)