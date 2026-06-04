cmake_minimum_required(VERSION 3.15)
project(SkyBridgeAirways-Backend CXX)

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)
aux_source_directory(utils UTILS_SRC)
aux_source_directory(schema SCHEMA_SRC)
aux_source_directory(controllers/admin ADMINCTL_SRC)
aux_source_directory(controllers/auth AUTHCTL_SRC)
add_executable(${PROJECT_NAME} main.cpp ${CTL_SRC} ${ADMINCTL_SRC} ${AUTHCTL_SRC} ${FILTER_SRC} ${PLUGIN_SRC} ${MODEL_SRC} ${UTILS_SRC} ${SCHEMA_SRC})

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

include(FetchContent)
FetchContent_Declare(jwt-cpp 
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG 08bcf77a687fb06e34138e9e9fa12a4ecbe12332 # v0.7.0 release
)
set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "disable building examples" FORCE)
Fetchcontent_MakeAvailable(jwt-cpp)
 
target_link_libraries(${PROJECT_NAME} PRIVATE jwt-cpp::jwt-cpp)

# nlohhman_json is also used but its already included in jwt-cpp
# FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
# FetchContent_MakeAvailable(json)
# target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

FetchContent_Declare(
  valijson
  GIT_REPOSITORY https://github.com/tristanpenman/valijson.git
  GIT_TAG v1.0.6
  GIT_SHALLOW TRUE
  GIT_SUBMODULES ""
)

set(valijson_BUILD_TESTS OFF CACHE BOOL "Don't build valijson tests" FORCE)
set(valijson_BUILD_EXAMPLES OFF CACHE BOOL "Don't build valijson examples" FORCE)

FetchContent_MakeAvailable(valijson)

target_link_libraries(${PROJECT_NAME} PRIVATE ValiJSON::valijson)

# find_package(libbcrypt REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE bcrypt)

# ##############################################################################

if (CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "c++17 or higher is required")
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
                    ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                   ${CMAKE_CURRENT_SOURCE_DIR}/models)
target_sources(${PROJECT_NAME}
               PRIVATE
               ${SRC_DIR}
               ${CTL_SRC}
               ${ADMINCTL_SRC}
               ${FILTER_SRC}
               ${PLUGIN_SRC}
               ${MODEL_SRC}
               ${SCHEMA_SRC}
               ${UTILS_SRC}
               ${AUTHCTL_SRC})

add_subdirectory(test)


configure_file(config.json config.json COPYONLY)
