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
set(rpclib_LIBRARIES "/Users/nishanth/Documents/Penn/rpclib/build/librpc.a")

set(SOURCE_FILES src/master_server.cc src/server_info.cc src/server_info.h)

add_executable(master_server ${SOURCE_FILES})

if(Boost_FOUND)
  target_link_libraries(master_server ${Boost_LIBRARIES} ${rpclib_LIBRARIES})
endif()
