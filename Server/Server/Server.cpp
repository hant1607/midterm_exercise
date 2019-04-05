#include "Server.h"



Server::Server(int port){
	WSAData wsadata;
	WORD dllVersion = MAKEWORD(2, 1);

	if (WSAStartup(dllVersion, &wsadata) != 0) {
		MessageBox(NULL, "Winsock start failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // tao dia chi may chu
	addr.sin_port = htons(port); //tao cong ket noi
	addr.sin_family = AF_INET; //dat loai dia chi ip la ipv4

	// tao socket de lang nghe ket noi den
	listening = socket(AF_INET, SOCK_STREAM, NULL);
	//ket noi dia chi toi socket 
	if (bind(listening, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::string errorMsg = "Faild to bind the address to listening socket! Winsock error: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK || MB_ICONERROR);
		exit(1);
	}
	if (listen(listening, SOMAXCONN) == SOCKET_ERROR) {
		std::string errorMsg = "Fail listening on listen socket! Winsock error: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK || MB_ICONERROR);
		exit(1);
	}
	config.loadUserData();
	config.loadKey();
}
bool Server::listenForNewConnection() {
	std::cout << "Waiting..." << std::endl;
	SOCKET newConnect = accept(listening, (SOCKADDR*)&addr, &addrlen);
	if (newConnect == 0) {
		std::cout << "Fail to accept the client connection. " << std::endl;
		return false;
	}
	else {
		char data[1024];
		ZeroMemory(data, sizeof(data));
		recv(newConnect, data, sizeof(data), 0);
		if (config.checkUsername(std::string(data))) {
			std::thread t(createHandle, newConnect, std::string(data));
			t.detach();
		}
		else {
			char errMsg[100] = "Check username again!";
			send(newConnect, errMsg, sizeof(errMsg), 0);
		}
	}
	return true;
}


Server::~Server()
{
}

void Server::createHandle(SOCKET s, std::string username) {
	Session session;
	session.addConnection(s, username);
	session.messageHandle(s);
}
