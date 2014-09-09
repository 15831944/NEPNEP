#include "stdafx.h"
#include "sock.h"

#include "..\util\network\SocketIO.cpp"

socketClass::socketClass()
{

}
socketClass::~socketClass()
{

}

void socketClass::onProcess(void *f)
{
	ALERT(_T("f"));
}