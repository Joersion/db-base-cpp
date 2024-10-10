cmake_minimum_required(VERSION 3.12)
project(dbTest)

ADD_DEFINITIONS( -O3 )

set(CMAKE_CXX_STANDARD 17)

link_directories("../build")

set(CMAKE_PREFIX_PATH "~/work/poco/x86")

find_package(MySQL REQUIRED)

find_package(Poco REQUIRED COMPONENTS Foundation Data DataSQLite DataMySQL)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(dbTest ${SOURCES})

target_link_libraries(dbTest db_base Poco::Foundation Poco::Data Poco::DataSQLite Poco::DataMySQL MySQL::client)
