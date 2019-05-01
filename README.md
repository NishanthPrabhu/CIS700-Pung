# CIS700-Pung

Dependencies 

1. boost - normal way, cmakelist.txt changes not required
2. Microsoft Seal : https://github.com/Microsoft/SEAL
3. Libsodium : https://libsodium.gitbook.io
4. rpclib - https://github.com/rpclib/rpclib

Set the following environment variables in you ~/.bashrc file after compiling libsodium and rpclib

SODIUM_LIB, RPCLIB_LIB, RPCLIB_INCLUDE_DIRS

 - Example : 
	Add the following lines to your ~/.bashrc
	
    	export SODIUM_LIB="path to libsodium.so"
    	export RPCLIB_LIB="path to librpc.a"
    	export RPCLIB_INCLUDE_DIRS="path to rpclib/include"
	
    After adding these lines issue the command

        source ~/.bashrc
	
To build this project :

    mkdir build<br/>
    cd build<br/>
    cmake ..<br/>
    cmake --build .<br/>
    
To run :
- For Master Server

        ./bin/master_server [server_list_file] [length of round(in milliseconds)]
        
    - Example : ./bin/master_server server_list 2000
- For Slave Server

        ./bin/slave_server [server_list_file] [slave{slave id}]
        
    - Example : ./bin/slave_server server_list slave1, ./bin/slave_server server_list slave2 and so on... 
- For Client

        ./client [client_id] [master_ip] [master_port]
        
    - Example : ./bin/client 1 localhost 30000
    
Sample server_list_file is provided in the root directory of the repo.

Once client is started the following commands are supported :

- /join : to enter conversation with a particular client
- /end  : to end chat with current peer
- /quit : to quit the client
- /help : to display list of available commands

