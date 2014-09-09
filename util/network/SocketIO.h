
//소켓 생성 템플레이트 헤더.

#ifndef INCLUDE_SOSCKETIO_CLASS
#define INCLUDE_SOSCKETIO_CLASS
#include <WinSock2.h>
#include <map>
#include <tchar.h>
#include <iostream>

#include "..\macrodef.h"
#include "..\struct.h"
#pragma comment(lib,"ws2_32")


using namespace std;
namespace SocketSpace
{
	class SocketIOClass
	{
	private:
		SOCKET lastorder; //클라단 소켓
		SOCKET server; //서버단소켓

		HANDLE thread;
		map<SOCKET, clientStruct> fdVec;
		fd_set read_fds;
		fd_set exc_fds;
		enum LoopState { LOOPSTOP, LOOPSTART, LOOPSTOPPING };
		LoopState serverloop;




		bool nonblock(SOCKET fd)
		{
			unsigned long arg = 1;
			return ioctlsocket(fd, FIONBIO, &arg) == 0;
		}

	public:

		SocketIOClass();
		~SocketIOClass();

		virtual void onLogout(SOCKET s)
		{
			FD_CLR(s, &read_fds);
			FD_CLR(s, &exc_fds);
			fdVec.erase(s);
			safeCloseSocket(s);
		};
		virtual void onConnect();
		  virtual void SocketIOClass::onAdmindatarecived(SOCKET s, BYTE *b, int l);
		virtual void onDatarecived(SOCKET s, BYTE *b, int l)
		{

		};
		int run(const TCHAR *ip, int port);



		virtual void SocketIOClass::onProcess(void *);
	};	

}


#endif

