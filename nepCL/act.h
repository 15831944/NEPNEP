/*******************************************************************************
Title :
File name : act.h
Date :

made by
*******************************************************************************/


// Function Pointer�� ���� Function Define
#define CMD(name) \
	void name(SOCKET sock, byte *recv_buf, sh_int recv_buf_len)
#define ACMD(name) \
	void name(byte *recv_buf, sh_int recv_buf_len)

//�ൿ��
CMD(cmd_req_login);
CMD(cmd_req_logout);
CMD(cmd_req_update);