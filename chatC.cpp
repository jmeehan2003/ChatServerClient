#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#define MAXCHARACTERS 500

//int getadddrinfo(const char *node, const char *service,
//				 const struct addrinfo *hints, struct addrinfo **results);

//https://www.techpowerup.com/forums/threads/c-c-sockets-faq-and-how-to-win-linux.56901/

int main(int argc, char *argv[]) {

	// argc should equal 3 if the user entered client [hostname] [port] to execute program
	if (argc != 3) {
		cout << "Please use this format to execute the program: client [hostname] [port]" << endl;
		return -1;
	}
	
	istringstream ss(argv[2]);
	int port;
	if (!(ss >> port)) {
		cout << "Invalid port number" << endl;	
		return -1;
	}

	string ipAddress = argv[1];
	

	//Create Socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		cout << "Can't create socket." << endl;
		return -1;
	} 
	
	//Fill in hint structure 
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	
	//Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult != 0) {
		cout << "Connection to server failed." << endl;
		close(sock);
		return -1;	
	}
	
	//Do-while loop to send and receive data
	char buff[MAXCHARACTERS];
	string userInput;
	string handle;
	cout << "Please enter your handle: " << endl;
	getline(cin, handle);	
	
	do {	
		memset(buff, 0, MAXCHARACTERS);
		int bytesReceived = recv(sock, buff, 4096, 0);
		if (bytesReceived > 0) {
		cout << "Server> " << string(buff, 0, bytesReceived) << endl;
		}

		//cout << userInput << endl;
		cout << handle + "> ";
		string first = handle + "> ";
		getline(cin, userInput);
	   	userInput = first + userInput;
		int sendMsg = send(sock, userInput.c_str(), userInput.size() + 1, 0);

		/*if (userInput.size() > 0) {
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult > 0) {
				//Wait for response
				memset(buff, 0, MAXCHARACTERS);
				bytesReceived = recv(sock, buff, 4096, 0);
				if (bytesReceived > 0) {
					cout << "Server>" << string(buff, 0, bytesReceived) << endl;
				}
		}
	}*/
	} while(userInput != "/quit");

	close(sock);


	return 0;
}	



