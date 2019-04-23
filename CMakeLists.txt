cmake_minimum_required(VERSION 3.8)
project(CIS700_Pung)

set(CMAKE_CXX_STANDARD 11)

find_package(SEAL 3.2.0 EXACT REQUIRED)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(CMAKE_BUILD_TYPE Debug)

set(rpclib_LIBRARIES "/home/vagrant/rpclib/build/librpc.a")

include_directories("/home/vagrant/rpclib/include")

set(sodium_LIBRARIES "/usr/local/lib/libsodium.so")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

file(GLOB seal_src
    "src/SealPIR/*.cpp"
    "src/SealPIR/*.hpp"
)    

set(MASTER_SOURCE_FILES src/master_server.cc src/master_callbacks.cc src/server_info.cc src/server_info.h src/master_callbacks.h src/master_server.h) 

set(SLAVE_SOURCE_FILES src/client_info.cc src/client_info.h src/slave_server.cc src/slave_server.h src/server_info.cc src/server_info.h src/slave_callbacks.cc src/slave_callbacks.h) 

set(CLIENT_SOURCE_FILES src/client_info.cc src/client_info.h src/msg_client.cc src/msg_client.h src/client.cc src/client.h)

add_executable(master_server ${MASTER_SOURCE_FILES} ${seal_src})

add_executable(slave_server ${SLAVE_SOURCE_FILES} ${seal_src})

add_executable(client ${CLIENT_SOURCE_FILES} ${seal_src})

add_executable(test_master src/test_master.cc)

if(Boost_FOUND)
    target_link_libraries(master_server ${Boost_LIBRARIES} ${rpclib_LIBRARIES} ${sodium_LIBRARIES} SEAL::seal pthread)
endif()

if(Boost_FOUND)
    target_link_libraries(slave_server ${Boost_LIBRARIES}  ${rpclib_LIBRARIES} ${sodium_LIBRARIES} SEAL::seal pthread)
endif()

if(Boost_FOUND)
    target_link_libraries(client ${Boost_LIBRARIES}  ${rpclib_LIBRARIES} ${sodium_LIBRARIES} SEAL::seal pthread)
endif()

if(Boost_FOUND)
    target_link_libraries(test_master ${Boost_LIBRARIES}  ${rpclib_LIBRARIES} pthread)
endif()
