cmake_minimum_required(VERSION 3.8)
project(CIS700_Pung)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.69.0 COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

include_directories("/Users/nishanth/Documents/Penn/rpclib/include")

set(CMAKE_BUILD_TYPE Debug)

set(rpclib_LIBRARIES "/Users/nishanth/Documents/Penn/rpclib/build/librpc.a")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(MASTER_SOURCE_FILES src/master_server.cc src/master_callbacks.cc src/server_info.cc src/server_info.h src/master_callbacks.h src/master_server.h src/slave_server.h src/slave_callbacks.cc src/slave_callbacks.h)

set(SLAVE_SOURCE_FILES src/slave_server.cc src/slave_server.h src/server_info.cc src/server_info.h src/slave_callbacks.cc src/slave_callbacks.h)

add_executable(master_server ${MASTER_SOURCE_FILES})

add_executable(slave_server ${SLAVE_SOURCE_FILES})

add_executable(test_master src/test_master.cc)

if(Boost_FOUND)
  target_link_libraries(master_server ${Boost_LIBRARIES} ${rpclib_LIBRARIES})
endif()

if(Boost_FOUND)
    target_link_libraries(slave_server ${Boost_LIBRARIES} ${rpclib_LIBRARIES})
endif()

if(Boost_FOUND)
    target_link_libraries(test_master ${Boost_LIBRARIES} ${rpclib_LIBRARIES})
endif()
