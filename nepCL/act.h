/*******************************************************************************
Title :
File name : act.h
Date :

made by
*******************************************************************************/


// Function Pointer를 위한 Function Define
#define CMD(name) \
	void name(SOCKET sock, byte *recv_buf, sh_int recv_buf_len)
#define ACMD(name) \
	void name(byte *recv_buf, sh_int recv_buf_len)

//행동들
CMD(cmd_req_login);
CMD(cmd_req_logout);
CMD(cmd_req_update);