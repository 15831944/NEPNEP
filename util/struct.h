#ifndef INCLUDE_STRUCT
#define INCLUDE_STRUCT
#include <WinSock2.h>
#include "macrodef.h"

typedef struct _command_infomation_structure {
	WORD opcode;
	void(*cmd_pt) (SOCKET, byte *data_buf, sh_int data_buf_len);
} cmdStruct;



typedef struct  _persnal_data_structure{
	uint  id;                       // PC�� ���� ��ȣ
	uint  userid;                   // ������ id
	char  pcname[PC_NAME_LEN + 1];    // pc name
	byte type;                     // PC�� ����
	char    lastlog[20];            // ������ ����, �Ǵ� ���� ��¥,�ð�
	char    maketime[20];           // PC�� ���� ��¥,�ð�
	char    deltime[20];            // PC�� ������ ��¥,�ð�
} persnalStruct;

typedef struct _socket_client_data_struct{
	SOCKET   sockfd;                // ������ socket descriptor
	byte    recv_buf[BUFMAX];      // �б� buffer, client�� ������ data
	sh_int   recv_buf_len;          // ������� ���� data ����
	char     ip_addr[16];           // client�� ������ �� IP �ּ�


	persnalStruct   pc;                    // ������ PC ���� ����

	byte    stat;                  // Login ����, LOGIN_STAT_???
}clientStruct;




#endif