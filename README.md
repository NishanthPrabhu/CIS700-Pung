# CIS700-Pung

~1. Set up master server~<br/>
~2. Master server - load balancer~<br/>
~3. Set up RPC between slaves~<br/>
~4. Master server - set up client facing interface - socket or rpc?~<br/>
~5. Receive and store public of clients~<br/>
~6. Send public keys of clients~<br/>
7. How to store client messages? (Vector or map)<br/>
8. How to integrate PIR into the mix? (Including the RPC, master node etc)<br/>
~9. Receive and store client public keys from pung clients~
10. Compute mailbox label (shared secret - so already done?)
11. Implement discretized rounds. Server must keep track of deposits/retrievals made by each
client and allow only 1 deposit + 1 retrieval in a single round.

------------------------------------------------------------------------

Things to do next : (Finish before 20th April)

client 1 wants to send message to client 2

client 1 has sending key k1
client 1 has receiving key k2

client 2 has receiving key k1
client 2 has sending key k2

In key exchange, if client 1 was the client, use sending key as kL and use use receiving key as kE.

Since client 2 was server, uses receiving key as kL and sending key as kE.

Client 1 computes label as PRF{kL}(round_number|client_2_id) to send message to client 2.

Client 1 computes label as PRF{kL}(round_number|client_1_id) to receive message from client 2.

PRF to use : void randombytes_buf_deterministic(void*constbuf,constsize_t size,
constunsignedcharseed[randombytes_SEEDBYTES]);

————————————————————————————————

When sending message to client 2, client 1 encrypts message as Enc_kE(m).

When sending message to client 1, client 2 encrypts message as Enc_kE(m).

————————————————————————————————

Q to ask sebastian : how to enforce rounds?


-----------------------------------------------------------------------

Dependencies 

1. rpclib - https://github.com/rpclib/rpclib, change include and lib in cmakelist.txt
2. boost - normal way, cmakelist.txt changes not required

To build

mkdir build<br/>
cd build<br/>
cmake ..<br/>
cmake --build .<br/>
