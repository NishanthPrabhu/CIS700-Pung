# CIS700-Pung

Dependencies 

1. rpclib - https://github.com/rpclib/rpclib, change include and lib in cmakelist.txt
2. boost - normal way, cmakelist.txt changes not required
3. SealPIR :

Add this to Cmakelists.txt

file(GLOB seal_src
    "src/SealPIR/*.cpp"
    "src/SealPIR/*.hpp"
)

add_executable(master_server ${MASTER_SOURCE_FILES} ${seal_src})

add_executable(slave_server ${SLAVE_SOURCE_FILES} ${seal_src})

add_executable(client ${CLIENT_SOURCE_FILES} ${seal_src})


To build

mkdir build<br/>
cd build<br/>
cmake ..<br/>
cmake --build .<br/>
