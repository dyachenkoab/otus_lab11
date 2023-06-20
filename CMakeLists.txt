cmake_minimum_required(VERSION 3.5)

project(lab11 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project( lab11 )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/" )
include( ODB )

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# List headers with persistent classes
set( odbcrs_headers
        src/person.h
        src/cat.h
        src/models.h
        src/creature.h
        )


# Run ODB compiler for all headers
odb_compile(
        HEADERS ${odbcrs_headers}
        DB pgsql
        STANDARD c++11
        GENERATE_QUERY GENERATE_SESSION GENERATE_SCHEMA
        HEADER_SUFFIX ".hpp"
        INLINE_SUFFIX ".ipp"
        SOURCE_SUFFIX ".cpp"
        FILE_SUFFIX ".odb"
#        DEFAULT_POINTER "std::shared_ptr"
)


# List all CRS source files
set( odbcrs_sources
        src/person.h
        src/cat.h
        src/person.cpp
        src/cat.cpp

        src/postgresdb.h
        src/postgresdb.cpp
        ${ODB_GENERATED_SOURCES}
        src/models.h
#        src/models.cpp
        src/creature.h src/creature.cpp
        src/dbhandler.h src/dbhandler.cpp
#        src/dbaccessor.h src/dbaccessor.cpp
)

set( odbcrs_libraries
        odb
        odb-pgsql
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unknown-pragmas" )

include_directories( src )

# Main executable
#add_executable( odbcrs src/main.cpp ${odbcrs_sources} )
#target_link_libraries( odbcrs ${odbcrs_libraries} )

#--------------------------------
add_executable(asio_server
    ${odbcrs_sources}
    src/asio_async_server.cpp
    src/asio_async_server.h
)
target_link_libraries(asio_server PUBLIC
        ${Boost_LIBRARIES}
        Threads::Threads
        ${odbcrs_libraries}
)

add_executable(asio_client src/asio_client.cpp)
target_link_libraries(asio_client PUBLIC
        ${Boost_LIBRARIES}
        Threads::Threads
)
#--------------------------------
