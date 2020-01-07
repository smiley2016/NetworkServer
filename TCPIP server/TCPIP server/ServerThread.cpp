#include "ServerThread.h"
#include "SysThread.h"
#include "User.h"
#include <string>
#include <iostream>
#include <cstdlib>

#pragma warning(disable:4996)

std::vector<ServerThread*> threadList;

bool ServerThread::stop() {
	return SysThread::stop();
}

void ServerThread::run() {

	SOCKET sendTo;
	while (true)
	{
		char* buf =(char*) buffer.c_str();
		if (recv(acceptSocket, buf, sizeof(buffer), 0) > 0 && buf != nullptr) {
			buffer = buf;
			if (getIntention() == "5") {
				fillBufferWithClients();
				sendMessage(this->acceptSocket, this->buffer.c_str());
				this->buffer.clear();
				continue;
			}
			if (getIntention() == "0") {
				eraseBuffer(0, ",");
				this->user.setUserName(getUserName());
				std::cout << "\n" << this->user.getUserName() << " connected with IP: " << inet_ntoa(this->user.getAddr().sin_addr) << "\n";
				continue;
			}
			if (getIntention() == "1") {
				if ((sendTo = getUserSocketByNameFromThreadList(getUserName())) != -1) {
					std::string message = getMessage() + "\n";
					sendMessage(sendTo,message.c_str());
					//sendMessage(this->acceptSocket, "The Message was sent!\n");
					printf("\nMessage sent to %s\n", getUserNameBySocket(sendTo).c_str());
				}
				else {
					sendMessage(this->acceptSocket, "NO_USER_WITH_SOCKET");
				}
				continue;
			}
			if (getIntention() == "2") {
				continue;
			}
			if (getIntention() == "3") {
				stop();
				for (int i = 0; i < threadList.size(); ++i) {
					if (threadList.at(i)->user.getUserName() == this->user.getUserName()) {
						threadList.erase(threadList.begin()+i);
					}
				}
				continue;
			}
		}
	}
}

std::string ServerThread::getMessage() {
	size_t pos = 0;
	if (this->buffer.size() > 0 && (pos = this->buffer.find_first_of(",") != std::string::npos)) {
		std::string data = this->buffer.substr(0, this->buffer.find(","));
		eraseBuffer(data.size(), ",");
	}
	return this->buffer;
}

std::string ServerThread::getUserName() {
	size_t pos = 0;
	std::string delimiter = ",";
	if ((pos = this->buffer.find(delimiter)) != std::string::npos) {
		eraseBuffer(pos, delimiter);
		std::string name = this->buffer.substr(0, buffer.find(delimiter));
		return name;
	}
	eraseBuffer(pos, delimiter);
	return this->buffer.substr(0, this->buffer.size()-1);
}

SOCKET ServerThread::getUserSocketByNameFromThreadList(std::string userName) {
	for (ServerThread * temp: threadList){
		if (temp->user.getUserName() == userName) {
			eraseBuffer(0, ",");
			return temp->acceptSocket;
		}
	}
	return -1;
}

std::string ServerThread::getIntention() {
	if (this->buffer.length() > 0) {
		std::string intention;
		if (this->buffer.find(",") != std::string::npos) {
			return this->buffer.substr(0, this->buffer.find(","));
		}
		return std::string("init");
	}
}

void ServerThread::eraseBuffer(size_t pos, std::string delimiter) {
	if (pos != std::string::npos) {
		this->buffer.erase(0, pos + 1);
	}
	else {
		this->buffer.erase(0, this->buffer.length()-1);
	}
}

std::string ServerThread::getUserNameBySocket(SOCKET socket) {
	for (ServerThread* temp : threadList) {
		if (temp->acceptSocket == socket) {
			return temp->user.getUserName();
		}
	}
}

void ServerThread::fillBufferWithClients() {
	this->buffer = "";
	for (ServerThread* client : threadList) {
		if (this->user.getUserName() == client->user.getUserName()) {
			continue;
		}
		buffer.append(client->user.getUserName() + ",");
	}
	buffer.append("\n");
}

void ServerThread::pushThread(ServerThread* thread) {
	threadList.push_back(thread);
}