cmake_minimum_required(VERSION 3.25)
project(SkyBridgeAirways-Backend CXX)

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
set(CMAKE_CXX_STANDARD 20)


set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)
aux_source_directory(utils UTILS_SRC)
aux_source_directory(schema SCHEMA_SRC)
aux_source_directory(controllers/admin ADMINCTL_SRC)
add_executable(${PROJECT_NAME} main.cpp ${CTL_SRC} ${ADMINCTL_SRC} ${FILTER_SRC} ${PLUGIN_SRC} ${MODEL_SRC} ${UTILS_SRC} ${SCHEMA_SRC})

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

# ##############################################################################

if (CMAKE_CXX_STANDARD LESS 20)
    message(FATAL_ERROR "c++20 or higher is required")
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
               
               ${UTILS_SRC})

add_subdirectory(test)


configure_file(config.json config.json COPYONLY)
