//macro constants
#ifndef BUFMAX
#define BUFMAX  4096
#endif
#define PC_NAME_LEN         12      // PC �̸� ����(byte)
#define NICK_NAME_LEN       20      // PC ȣĪ ����(byte)


//macro type
#ifndef tstring
#ifdef UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif
#endif



//macro function
#ifndef ASSERT
#endif
#ifndef ALERT
#endif
#ifndef MSG
#endif
#ifndef TRACE
#define TRACE(e) cout<<##e<<endl;
#define LOG(e) TRACE(##e)
#endif
#ifndef ALERT
#define ALERT(e) MessageBox(NULL,(wstring()+##e).c_str(),_T("ALERT"),MB_OK);
#endif

#ifndef safeCloseSocket
#define safeCloseSocket(fd) if(fd!=NULL){closesocket(fd);fd=NULL;}
#endif

//type
typedef unsigned char      byte;
typedef short int          sh_int;
typedef unsigned short int sh_uint;
#ifndef _SYS_TYPES_H
typedef unsigned int       uint;
typedef unsigned long      ulong;
#endif

