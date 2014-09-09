/*******************************************************************************
Title :
File name : protocol.h
Date :

made by
*******************************************************************************/

#ifndef _PROTOCOL_H_

#ifdef DEBUG
#define PRT_RECV      1         // 디버깅 할 때 protocol 출력용
#define PRT_SEND      2         // 디버깅 할 때 protocol 출력용
#endif

#define PRT_PH        0xF1      // protocol header byte 내용
#define PRT_PT        0xF2      // protocol tail byte 내용

#define PRT_PH_SIZ    1         // protocol header가 담기는 곳은 1 byte
#define PRT_LEN_SIZ   2         // protocol length가 담기는 곳은 2 byte
#define PRT_PT_SIZ    1         // protocol tail이 담기는 곳은 1byte
#define PRT_FCMD_SIZ  1         // protocol 첫번째 command가 담기는 곳은 1byte
#define PRT_SCMD_SIZ  1         // protocol 두번째 command가 담기는 곳은 1byte

#define PRT_LOC_LEN   1         // protocol 길이가 담기는 곳 시작 위치
#define PRT_LOC_FCMD  PRT_LOG_LEN + PRT_LEN_SIZ // protocol 첫번째 명령어 위치
#define PRT_LOC_SCMD  PRT_LOG_FCMD + PRT_FCMD_SIZ // 두번째 명령어 시작 위치
#define PRT_LOC_DATA  PRT_LOC_SCMD + PRT_SCMD_SIZ // Data 시작 위치

#define PRT_STRING_TOKEN '\0'   // protocol에서 string은 이걸로 끝난다.

///////////////////////////////////////////////////////////////////////////////
// 명령어 관련 protocol 내용 ***************************************************
// FCMD와 SCMD는 바꾸면 안된다.
// 바꾸면 Function Pointer로 바로 가는 것이기 때문에 엉망이 되어 버린다.


#define PRT_FCMD_UP						0x00
#define PRT_SCMD_UPDATE					0x01
#define		PRT_UP_VERSIONCHECK				0x00
#define		PRT_UP_


///////////////////////////////////////////////////////////////////////////////
//  Server에서 사용하는 프로토콜 ******************************************
///////////////////////////////////////////////////////////////////////////////
//  Server login 관련 프로토콜 [01][xx]
#define PRT_FCMD_LOG                    0x01

#define PRT_SCMD_LOG_SERVERLOGIN        0x01 //  Server에 접속
//  -  Server User Login(0x01 0x01)
#define   PRT_LOG_USERLOGIN_SUCCESS			0x00 // 접속 성공
#define   PRT_LOG_USERLOGIN_ERROR_MAXUSER	0x01 // 접속 불가, 사용자 초과
#define   PRT_LOG_USERLOGIN_ERROR_NOUSER	0x02 // 접속 불가, 사용자 없음
#define   PRT_LOG_USERLOGIN_ERROR_ETC		0x03 // 접속 불가, 기타
#define   PRT_LOG_USERLOGIN_UPDATE			0x04 // 버전 업데이트 
#define   PRT_LOG_USERLOGIN_UPDATEDATA		0x06

//	-	Server User Logout(0x01 0x02) 
#define PRT_SCMD_LOG_LOGOUT           0x02 // PC 로그아웃

//  -  Server Creat New PC(0x01 0x03)
#define PRT_SCMD_LOG_NEWPC              0x03 // 새 PC 만들기
#define   PRT_LOG_NEWPC_SUCCESS           0x00 // 생성 성공
#define   PRT_LOG_NEWPC_ERROR_MAXPC       0x01 // 생성 실패, 가능 개수 초과
#define   PRT_LOG_NEWPC_ERROR_EXISTNAME   0x02 // 생성 실패, 이미 존재 이름
#define   PRT_LOG_NEWPC_ERROR_ETC         0x03 // 생성 실패, 기타

//  -  Server Del PC(0x01 0x04)
#define PRT_SCMD_LOG_DELPC              0x04 // PC 삭제
#define   PRT_LOG_DELPC_SUCCESS           0x00 // 삭제 성공
#define   PRT_LOG_DELPC_ERROR_ETC         0x01 // 삭제 실패, 기타

//  -  Server Login PC(0x01 0x05)
#define PRT_SCMD_LOG_PCLOGIN            0x05 // PC 선택하여 접속
#define   PRT_LOG_PCLOGIN_SUCCESS         0x00 // 접속 성공
#define   PRT_LOG_PCLOGIN_ERROR_ETC       0x01 // 접속 실패, 기타





///////////////////////////////////////////////////////////////////////////////
//  Server Message 관련 프로토콜 [02][xx]
#define PRT_FCMD_MSG                    0x02

//  -  Server Msg to Map(0x02 0x01)
#define PRT_SCMD_MSG_TOMAP              0x01 // Map에 있는 모두에게 보내기
#define   PRT_MSG_TOMAP_SUCCESS           0x00 // 전송 성공
#define   PRT_MSG_TOMAP_ERROR_NOPC        0x01 // 전송 실패, 한명도 없다.
#define   PRT_MSG_TOMAP_ERROR_ETC         0x02 // 전송 실패, 기타

//  -  Server Msg to One(0x02 0x02)
#define PRT_SCMD_MSG_TOONE              0x02 // 특정 사람에게 전달
#define   PRT_MSG_TOONE_SUCCESS           0x00 // 전송 성공
#define   PRT_MSG_TOONE_ERROR_TOME        0x01 // 실패, 나한텐 못 보냄
#define   PRT_MSG_TOONE_ERROR_NOPC        0x02 // 실패, 그런 사용자 없다.
#define   PRT_MSG_TOONE_ERROR_ETC         0x02 // 실패, 기타



#define _PROTOCOL_H_
#endif
