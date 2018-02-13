# ChatServerClient
Simple Python Chat Server and C++ Chat Client

NOTES:
-- Get access to executable Python file chatserve first 
-- by running chmod u+x chatserve 

-- Take note of which server chat server is running on

- USe this format to run the Chat Client  
./chatclient <server-hostname> <port#>
NOTE: <server-hostname> is the server the Chat Server is running on
NOTE: Be sure that the port number passed to chatserve is the same as the port
number passed to ./chatclient


INSTRUCTIONS:
1) Get access to execute chatserve. Type in command line:
chmod u+x chatserve
1) Run chatserve.  Type in command line:
./chatserve <port#>

2) Compile chatclient.cpp. Type in command line:
g++ chatclient.cpp -o chatclient

3) Run chatclient.cpp.  Type in command line:
./chatclient <server-hostname> <port#>

4) On the client, enter a user handle at the prompt

5) On the client, enter a message and press enter.

6) Client's message should be displayed on the server.

7) On the server, enter a message and press enter.

8) Server's message should be displayed on the client.

9) This continues until either the server or client enters "\quit".

10) When either the server or client enters "\quit", the connection with
    the client will terminate but the server will continue to wait for 
    another client to connect and send a message.

Some resources that assisted in me in completing this project are:
Python sever:
IBM: Sockets programming in Python
www.ibm.com/developerworks/linux/tutorials/l-pysocks/
Python Documentation:
https://docs.python.org/2/library/socket.html

C++ Chat client
//Some information regarding how to set up a c/c++ socket was taken from these sources
//https://www.techpowerup.com/forums/threads/c-c-sockets-faq-and-how-to-win-linux.56901/
//Beej's Guide  http://beej.us/guide/bgnet/

These are also noted in the relevant files

