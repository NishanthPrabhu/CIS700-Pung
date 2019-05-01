cmake_minimum_required(VERSION 3.8)
project(CIS700_Pung)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(SEAL 3.2.0 EXACT REQUIRED)

set(CMAKE_BUILD_TYPE Debug)

include_directories($ENV{RPCLIB_INCLUDE_DIRS})
include_directories("src/")
include_directories("src/SealPIR")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

file(GLOB seal_utils
    "src/SealPIR/utils/*.cpp"
)

file(GLOB seal_client
    "src/SealPIR/client/*.cpp"
)

file(GLOB seal_server
    "src/SealPIR/server/*.cpp"
)

set(MASTER_SOURCE_FILES src/master/master_server.cc src/master/master_callbacks.cc src/utils/server_info.cc) 

set(SLAVE_SOURCE_FILES src/utils/client_info.cc src/slave/slave_server.cc src/utils/server_info.cc src/slave/slave_callbacks.cc) 

set(CLIENT_SOURCE_FILES src/utils/client_info.cc src/client/msg_client.cc src/client/client.cc)

add_executable(master_server ${MASTER_SOURCE_FILES})

add_executable(slave_server ${SLAVE_SOURCE_FILES} ${seal_utils} ${seal_server})

add_executable(client ${CLIENT_SOURCE_FILES} ${seal_utils} ${seal_client})

add_executable(test_master src/test/test_master.cc)

if(Boost_FOUND)
    target_link_libraries(master_server ${Boost_LIBRARIES} $ENV{RPCLIB_LIB} $ENV{SODIUM_LIB} SEAL::seal pthread)
endif()

if(Boost_FOUND)
    target_link_libraries(slave_server ${Boost_LIBRARIES}  $ENV{RPCLIB_LIB} $ENV{SODIUM_LIB} SEAL::seal pthread)
endif()

if(Boost_FOUND)
    target_link_libraries(client ${Boost_LIBRARIES}  $ENV{RPCLIB_LIB} $ENV{SODIUM_LIB} SEAL::seal pthread)
endif()

if(Boost_FOUND)
    target_link_libraries(test_master ${Boost_LIBRARIES}  $ENV{RPCLIB_LIB} pthread)
endif()
