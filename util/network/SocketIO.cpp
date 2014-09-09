

#include "SocketIO.h"
#include "..\struct.h"
using namespace SocketSpace;


SocketIOClass::SocketIOClass()
{
	FD_ZERO(&read_fds);
	FD_ZERO(&exc_fds);
	lastorder = 0;
	thread = 0;

	//WSA Init
	WORD wversionRequested;
	WSADATA wsadata;
	int err;
	wversionRequested = MAKEWORD(2, 2);
	err = ::WSAStartup(wversionRequested, &wsadata);
	TCHAR BUF[256];
	if (err != 0)
	{
		_itow_s(err, BUF, 256, 10);
		ALERT(_T("WSAStartup Err: ") + BUF);
	}
	if (LOBYTE(wsadata.wVersion) != 2 ||
		HIBYTE(wsadata.wVersion) != 2)
	{
		ALERT(_T("wVersion invalid"));
	}
}

SocketIOClass::~SocketIOClass()
{
	safeCloseSocket(server);
	safeCloseSocket(lastorder);

	for (auto iter = fdVec.begin(); iter != fdVec.end(); iter++)
	{
		closesocket(iter->first);
	}

	WSACleanup();
}
void SocketIOClass::onConnect()
{
		SOCKET sSock;
		struct sockaddr_in cli_addr;

}
void SocketIOClass::onAdmindatarecived(SOCKET s, BYTE *b, int l)
{

}

int SocketIOClass::run(const TCHAR *ip, int port)
{
	return 0;
}
void SocketIOClass::onProcess(void *)
{
	struct timeval tval;
	int isFdSet = 0;


	tval.tv_sec = 5;
	tval.tv_usec = 0;
	int result;

	int cnt = 0;


	struct timeval rtn_time;
	ZeroMemory(&rtn_time, sizeof(timeval));
	serverloop = LoopState::LOOPSTART;
	int nread = 0;

	BYTE recvbuf[BUFSIZ];

	while (serverloop == LOOPSTART)
	{

		fd_set localread_fds = read_fds;
		fd_set localexcept_fds = exc_fds;
		rtn_time.tv_sec = 1;
		rtn_time.tv_usec = 0;


		result = select(fdVec.size() + 1, &localread_fds, (fd_set  *)0, &localexcept_fds, &tval);     // select setting
		if (result == SOCKET_ERROR)
		{

			ALERT(_T("Select error : "));
			continue;
		}
		if (FD_ISSET(server, &localread_fds))
		{
			onConnect();
		}
		else if (FD_ISSET(lastorder, &localread_fds))
		{
			nread = recv(lastorder, (char*)recvbuf, sizeof(recvbuf), 0);
			if (nread <= 0)
			{
				throw "LASTORDER";
				break;
				//	onLogout(lastorder);
			}
			else
				onAdmindatarecived(lastorder, recvbuf, nread);
		}
		else
		{
			for (auto iter = fdVec.begin(); iter != fdVec.end(); iter++)
			{
				if (FD_ISSET(iter->first, &localread_fds))
				{
					nread = recv(iter->first, (char*)recvbuf, sizeof(recvbuf), 0);
					if (nread <= 0)
					{
						onLogout(iter->first);
					}
					else
						onDatarecived(iter->first, recvbuf, nread);
					if (--result <= 0) break; // no more readable descriptors
				}
				else if (FD_ISSET(iter->first, &localexcept_fds))
				{
					onLogout(iter->first);

				}

			}
		}

	}
	LOG(_T("SERVER LOOP END"));

}