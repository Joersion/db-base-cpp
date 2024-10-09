cmake_minimum_required(VERSION 3.12)

project(db_base)

add_definitions(-O3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_PREFIX_PATH "~/work/poco/x86")

find_package(Poco REQUIRED COMPONENTS Foundation Data DataSQLite DataMySQL)

find_package(MySQL REQUIRED)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(db_base ${SOURCES})

target_link_libraries(db_base Poco::Foundation Poco::Data Poco::DataSQLite Poco::DataMySQL MySQL::client)