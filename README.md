# CIS700-Pung

~1. Set up master server~<br/>
~2. Master server - load balancer~<br/>
~3. Set up RPC between slaves~<br/>
~4. Master server - set up client facing interface - socket or rpc?~<br/>
~5. Receive and store public of clients~<br/>
~6. Send public keys of clients~<br/>
7. How to store client messages? (Vector or map)<br/>
8. How to integrate PIR into the mix? (Including the RPC, master node etc)<br/>
9. Receive and store client public keys from pung clients

~TODO : test build infra on linux (working on osx)~
TODO: Figure out how to send unsigned char array without casting to/from std::string using msgpack


Dependencies 

1. rpclib - https://github.com/rpclib/rpclib, change include and lib in cmakelist.txt
2. boost - normal way, cmakelist.txt changes not required

To build

mkdir build<br/>
cd build<br/>
cmake ..<br/>
cmake --build .<br/>
