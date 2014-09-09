/*******************************************************************************
Title :
File name : protocol.h
Date :

made by
*******************************************************************************/

#ifndef _PROTOCOL_H_

#ifdef DEBUG
#define PRT_RECV      1         // ����� �� �� protocol ��¿�
#define PRT_SEND      2         // ����� �� �� protocol ��¿�
#endif

#define PRT_PH        0xF1      // protocol header byte ����
#define PRT_PT        0xF2      // protocol tail byte ����

#define PRT_PH_SIZ    1         // protocol header�� ���� ���� 1 byte
#define PRT_LEN_SIZ   2         // protocol length�� ���� ���� 2 byte
#define PRT_PT_SIZ    1         // protocol tail�� ���� ���� 1byte
#define PRT_FCMD_SIZ  1         // protocol ù��° command�� ���� ���� 1byte
#define PRT_SCMD_SIZ  1         // protocol �ι�° command�� ���� ���� 1byte

#define PRT_LOC_LEN   1         // protocol ���̰� ���� �� ���� ��ġ
#define PRT_LOC_FCMD  PRT_LOG_LEN + PRT_LEN_SIZ // protocol ù��° ��ɾ� ��ġ
#define PRT_LOC_SCMD  PRT_LOG_FCMD + PRT_FCMD_SIZ // �ι�° ��ɾ� ���� ��ġ
#define PRT_LOC_DATA  PRT_LOC_SCMD + PRT_SCMD_SIZ // Data ���� ��ġ

#define PRT_STRING_TOKEN '\0'   // protocol���� string�� �̰ɷ� ������.

///////////////////////////////////////////////////////////////////////////////
// ��ɾ� ���� protocol ���� ***************************************************
// FCMD�� SCMD�� �ٲٸ� �ȵȴ�.
// �ٲٸ� Function Pointer�� �ٷ� ���� ���̱� ������ ������ �Ǿ� ������.


#define PRT_FCMD_UP						0x00
#define PRT_SCMD_UPDATE					0x01
#define		PRT_UP_VERSIONCHECK				0x00
#define		PRT_UP_


///////////////////////////////////////////////////////////////////////////////
//  Server���� ����ϴ� �������� ******************************************
///////////////////////////////////////////////////////////////////////////////
//  Server login ���� �������� [01][xx]
#define PRT_FCMD_LOG                    0x01

#define PRT_SCMD_LOG_SERVERLOGIN        0x01 //  Server�� ����
//  -  Server User Login(0x01 0x01)
#define   PRT_LOG_USERLOGIN_SUCCESS			0x00 // ���� ����
#define   PRT_LOG_USERLOGIN_ERROR_MAXUSER	0x01 // ���� �Ұ�, ����� �ʰ�
#define   PRT_LOG_USERLOGIN_ERROR_NOUSER	0x02 // ���� �Ұ�, ����� ����
#define   PRT_LOG_USERLOGIN_ERROR_ETC		0x03 // ���� �Ұ�, ��Ÿ
#define   PRT_LOG_USERLOGIN_UPDATE			0x04 // ���� ������Ʈ 
#define   PRT_LOG_USERLOGIN_UPDATEDATA		0x06

//	-	Server User Logout(0x01 0x02) 
#define PRT_SCMD_LOG_LOGOUT           0x02 // PC �α׾ƿ�

//  -  Server Creat New PC(0x01 0x03)
#define PRT_SCMD_LOG_NEWPC              0x03 // �� PC �����
#define   PRT_LOG_NEWPC_SUCCESS           0x00 // ���� ����
#define   PRT_LOG_NEWPC_ERROR_MAXPC       0x01 // ���� ����, ���� ���� �ʰ�
#define   PRT_LOG_NEWPC_ERROR_EXISTNAME   0x02 // ���� ����, �̹� ���� �̸�
#define   PRT_LOG_NEWPC_ERROR_ETC         0x03 // ���� ����, ��Ÿ

//  -  Server Del PC(0x01 0x04)
#define PRT_SCMD_LOG_DELPC              0x04 // PC ����
#define   PRT_LOG_DELPC_SUCCESS           0x00 // ���� ����
#define   PRT_LOG_DELPC_ERROR_ETC         0x01 // ���� ����, ��Ÿ

//  -  Server Login PC(0x01 0x05)
#define PRT_SCMD_LOG_PCLOGIN            0x05 // PC �����Ͽ� ����
#define   PRT_LOG_PCLOGIN_SUCCESS         0x00 // ���� ����
#define   PRT_LOG_PCLOGIN_ERROR_ETC       0x01 // ���� ����, ��Ÿ





///////////////////////////////////////////////////////////////////////////////
//  Server Message ���� �������� [02][xx]
#define PRT_FCMD_MSG                    0x02

//  -  Server Msg to Map(0x02 0x01)
#define PRT_SCMD_MSG_TOMAP              0x01 // Map�� �ִ� ��ο��� ������
#define   PRT_MSG_TOMAP_SUCCESS           0x00 // ���� ����
#define   PRT_MSG_TOMAP_ERROR_NOPC        0x01 // ���� ����, �Ѹ� ����.
#define   PRT_MSG_TOMAP_ERROR_ETC         0x02 // ���� ����, ��Ÿ

//  -  Server Msg to One(0x02 0x02)
#define PRT_SCMD_MSG_TOONE              0x02 // Ư�� ������� ����
#define   PRT_MSG_TOONE_SUCCESS           0x00 // ���� ����
#define   PRT_MSG_TOONE_ERROR_TOME        0x01 // ����, ������ �� ����
#define   PRT_MSG_TOONE_ERROR_NOPC        0x02 // ����, �׷� ����� ����.
#define   PRT_MSG_TOONE_ERROR_ETC         0x02 // ����, ��Ÿ



#define _PROTOCOL_H_
#endif
