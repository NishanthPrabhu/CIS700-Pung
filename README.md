# CIS700-Pung

IMPORTANT

I've added the SealPIR code locally since i had trouble linking it. You can try linking it,
if not, just add this to Cmakelists.txt

file(GLOB seal_src
    "src/SealPIR/*.cpp"
    "src/SealPIR/*.hpp"
)

add_executable(master_server ${MASTER_SOURCE_FILES} ${seal_src})

add_executable(slave_server ${SLAVE_SOURCE_FILES} ${seal_src})

add_executable(client ${CLIENT_SOURCE_FILES} ${seal_src})

--------------------------------------------------------------------------

~1. Set up master server~<br/>
~2. Master server - load balancer~<br/>
~3. Set up RPC between slaves~<br/>
~4. Master server - set up client facing interface - socket or rpc?~<br/>
~5. Receive and store public of clients~<br/>
~6. Send public keys of clients~<br/>
7. How to store client messages? (Vector or map)<br/>
8. How to integrate PIR into the mix? (Including the RPC, master node etc)<br/>
~9. Receive and store client public keys from pung clients~
~10. Compute mailbox label (shared secret - so already done?)~
~11. Implement discretized rounds. Server must keep track of deposits/retrievals made by each
client and allow only 1 deposit + 1 retrieval in a single round.~

------------------------------------------------------------------------

Dependencies 

1. rpclib - https://github.com/rpclib/rpclib, change include and lib in cmakelist.txt
2. boost - normal way, cmakelist.txt changes not required

To build

mkdir build<br/>
cd build<br/>
cmake ..<br/>
cmake --build .<br/>
