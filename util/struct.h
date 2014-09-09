#ifndef INCLUDE_STRUCT
#define INCLUDE_STRUCT
#include <WinSock2.h>
#include "macrodef.h"

typedef struct _command_infomation_structure {
	WORD opcode;
	void(*cmd_pt) (SOCKET, byte *data_buf, sh_int data_buf_len);
} cmdStruct;



typedef struct  _persnal_data_structure{
	uint  id;                       // PC의 고유 번호
	uint  userid;                   // 접속자 id
	char  pcname[PC_NAME_LEN + 1];    // pc name
	byte type;                     // PC의 종류
	char    lastlog[20];            // 마지막 접속, 또는 종료 날짜,시간
	char    maketime[20];           // PC를 만든 날짜,시간
	char    deltime[20];            // PC를 삭제한 날짜,시간
} persnalStruct;

typedef struct _socket_client_data_struct{
	SOCKET   sockfd;                // 접속한 socket descriptor
	byte    recv_buf[BUFMAX];      // 읽기 buffer, client가 보내온 data
	sh_int   recv_buf_len;          // 현재까지 들어온 data 길이
	char     ip_addr[16];           // client가 접속해 온 IP 주소


	persnalStruct   pc;                    // 접속자 PC 관련 정보

	byte    stat;                  // Login 상태, LOGIN_STAT_???
}clientStruct;




#endif