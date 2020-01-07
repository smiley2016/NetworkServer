#pragma once
#include "SysThread.h"
#include "User.h"
#include <string>
#include <vector>

class ServerThread : public SysThread {
public:
	ServerThread(SOCKET acceptSocket) {
		this->acceptSocket = acceptSocket;
	}

	bool stop();

	bool makeThread(int length, sockaddr_in addr, ServerThread * thread) {
		initUser(User(length + 1, addr));
		pushThread(thread);
		return SysThread::start();
	}

	void sendMessage(SOCKET sendTo, const char* message) {
		send(sendTo, message, strlen(message), 0);
	}

	void initUser(User user) {
		this->user = user;
	}
	void pushThread(ServerThread* thread);
	void fillBufferWithClients();
	std::string getIntention();
	void eraseBuffer(size_t pos, std::string delimiter);
	std::string getUserNameBySocket(SOCKET socket);
	std::string getUserName();
	SOCKET getUserSocketByNameFromThreadList(std::string userName);
	
	User getUser() {
		return user;
	}

	SOCKET getSocket() {
		return acceptSocket;
	}
protected:
	virtual void run();
	std::string getMessage();
private:
	User user;
	SOCKET acceptSocket;
	std::string buffer = "";
};