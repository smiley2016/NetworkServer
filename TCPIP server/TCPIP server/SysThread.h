#pragma once
#include <limits.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

class SysThread
{
public:
	SysThread();
	virtual ~SysThread();
	virtual bool start(void);
	virtual bool stop(unsigned int timeout = 0);
	inline volatile bool& isRunning(void)
	{
		return m_bRunning;
	}
	inline volatile bool& isExited(void)
	{
		return m_bExited;
	}
	
private:
	friend DWORD WINAPI runStub(LPVOID mthread);
protected:
	virtual void run(void); //Ezt a metodust a származtatott	
public:
	static const unsigned int INFINIT_WAIT;
private:
	volatile bool m_bRunning;
	volatile bool m_bExited;
	HANDLE m_thread;

};

