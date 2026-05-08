cmake_minimum_required(VERSION 3.15)
project(DrogonApp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "C:\\FlightMgmtTools\\VC\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"
    CACHE STRING "vcpkg toolchain")
set(VCPKG_INSTALLED_DIR "C:\\FlightMgmtTools\\vcpkg\\installed" CACHE PATH "vcpkg installed dir")
list(APPEND CMAKE_PREFIX_PATH "C:\\FlightMgmtTools\\vcpkg\\installed\\x64-windows")
find_package(Drogon CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
