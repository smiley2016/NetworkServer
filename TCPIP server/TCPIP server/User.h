#pragma once
#include <string>
#include "SysThread.h"

class User {
private:
	int id;
	std::string userName;
	sockaddr_in addr;

public:
	User() {
		this->id = 0;
		this->addr = sockaddr_in();
	}

	User(int id, sockaddr_in addr) {
		this->id = id;
		this->addr = addr;
	}

	~User() {
		id = NULL;
	}

	sockaddr_in getAddr() {
		return this->addr;
	}

	std::string getUserName() {
		return this->userName;
	}
	
	void setUserName(std::string userName) {
		this->userName = userName;
	}

	int getId() {
		return this->id;
	}

	void setUserNameFromClient(std::string clientUserName) {
		this->userName = clientUserName;
	}
};
