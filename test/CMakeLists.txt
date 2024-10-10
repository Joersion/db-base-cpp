cmake_minimum_required(VERSION 3.12)
project(dbTest)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_STANDARD 17)

link_directories("../build")

set(CMAKE_PREFIX_PATH "~/work/poco/x86")

set(CMAKE_BUILD_TYPE Release)

find_package(Poco REQUIRED COMPONENTS Foundation Data DataSQLite)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(dbTest ${SOURCES})

target_link_libraries(dbTest db_base Poco::Foundation Poco::Data Poco::DataSQLite)