/*******************************************************************************
** Author: James Meehan
** Description:  This is a chat client that connects to a chat server on a port
** identified by the client on the command line.  The user can then communicate
** with the server by sending alternating messages. The program is terminated
** by the user typing "\quit" or the server closing the connection.
*******************************************************************************/

#include <iostream>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#define MAXCHARACTERS 500


//Some information regarding how to set up a c/c++ socket was taken from these sources
//https://www.techpowerup.com/forums/threads/c-c-sockets-faq-and-how-to-win-linux.56901/
//Beej's Guide  http://beej.us/guide/bgnet/


int initiateContact(char* ipAddress, int port, string handle);
string getHandle();
void receiveMessage(int sock);
string sendMessage(int sock, string handle);

int main(int argc, char *argv[]) {
	string ipAddress, userInput;
	char* serverIP = argv[1];

	// argc should equal 3 if the user entered client [hostname] [port] to execute program
	if (argc != 3) {
		cout << "Please use this format to execute the program: client [hostname] [port]" << endl;
		return -1;
	}
	
	// check that a valid port number was entered
	istringstream ss(argv[2]);
	int port;
	if (!(ss >> port)) {
		cout << "Invalid port number" << endl;	
		return -1;
	}

	
	// Get user handle
	string handle = getHandle();

	// Create a socket
	int sock = initiateContact(serverIP, port, handle);

	// Begin two-way chat until client or Server closes the  connection
	while (userInput != "\\quit") {

		receiveMessage(sock);
		userInput = sendMessage(sock, handle);
	}
	
	// Close connection upon user quitting	 
	close(sock);
	
	// Display goodbye message
	cout << "You have logged off of chat" << endl;
	cout << "Log in again to reconnect" << endl;
	cout << "Goodbye " << endl;

	return 0;
}	


/*********************************************************
** Description: initiateContact() take's the client's
** ipAddress and creates a socket.  It then attempts to
** establish a connection with the server and, if 
** successful, has the user send an initial contact message
** Sources for setting up hint structure: Beej's Guide
** beej.us/guide/bgnet
**********************************************************/ 
int initiateContact(char *serverIP, int port, string handle) {
	
	struct hostent* host = gethostbyname(serverIP);
	sockaddr_in hint;
	bzero((char*)&hint, sizeof(hint));
	hint.sin_family = AF_INET;
	hint.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
	hint.sin_port = htons(port);
		
	//Create socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		cout << "Can't create socket." << endl;
		return -1;
	} 
	//Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult != 0) {
		cout << "Connection to server failed." << endl;
		close(sock);
		return -1;	
	}
	
	cout << "You've connected to James's Python Chat Server! Please send your first message." << endl << endl;  
	
	//Send initial message
	string userInput, userMsg; 
	cout << handle;
	getline(cin, userInput);
	
	//Make sure message is less than 500 characters
	while (userInput.length() > 500)
	{
			cout << "Please keep the messages to 500 characters or less." << endl;
			cout << "Please retype your message" << endl;
			cout << handle;
			getline(cin, userInput);
	}
	
	//Prepend handle to client meesage
	userMsg = handle + userInput;

	//Send initial message
	int sendMsg = send(sock, userMsg.c_str(), userMsg.size() + 1, 0);

return sock;
}

/*******************************************************
** Description: getHandle() has the user specify
** their handle with the requirement that it be
** one word (if they enter more than one, then just the
** first one is chosen) and no longer than 10 characters
********************************************************/ 
string getHandle() {
	string handle;
	cout << "Please enter your handle (one word): " << endl;
	getline(cin, handle);
	
	// Make sure handle is no more than 10 characters
	while (handle.length() > 10) {
		cout << "Your handle must be less than 10 characters" << endl;
		cout << "Please enter your handle (one word): ";
		getline(cin, handle);
	}

	// Use only first word for handle if user entered more than one
	if (handle.length() <= 10)
	{	
		for (int i = 0; i < handle.length(); i++)
		{
			if (handle[i] == ' ') {
				handle.erase(handle.find(' ')); 
				break;
			}
		}			
	}

	// Append "> " to handle for messaging
	handle += "> ";
	cout << endl;

	return handle;	
}

/*******************************************************
** Description: receiveMessage() receives a message from 
** the Server and displays it to the client.  If the Server
** disconnects it alerts the user and then exits.
********************************************************/ 
void receiveMessage(int sock) {
	// Set buffer to MAXCHARACTERS (set at 500 for this assignment)
	char buff[MAXCHARACTERS];

	// Clear the buffer
	memset(buff, 0, MAXCHARACTERS);

	// Get message from Server
	int bytesReceived = recv(sock, buff, 500, 0);
	if (bytesReceived > 0)
		// Display message from Server
		cout << string(buff, 0, bytesReceived);
	// Else Server terminated the session
	else {
		cout << "Chat Session Terminated by Server" << endl;
		cout << "Log in again to attempt reconnect" << endl;
		cout << "Goodbye" << endl;
		exit(0);
	}

}

/*******************************************************
** Description: sendMessage() prompts the client for a message
** and the sends that message to the client.
**********************************************************/
string sendMessage(int sock, string handle) {
	//Prompt user for message
	string userInput;
	cout << handle;
	getline(cin, userInput);
		
	//Make sure message is 500 characters or less	
	while (userInput.length() >= 500)
	{
		cout << "Please keep the messages to 500 characters or less." << endl;
		cout << "Please retype your message" << endl;
		cout << handle;
		getline(cin, userInput);
	}
	
	string userMsg = handle + userInput;
		
	int sendMsg = send(sock, userMsg.c_str(), userMsg.size() + 1, 0);

	return userInput;
}



