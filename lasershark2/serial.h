#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>

#define BUFFERSIZE 7

class Serial{


public:

	HANDLE hCom;
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[BUFFERSIZE] = { 0 };

	BOOL dingdong = TRUE;
	BOOL fWaitingOnRead = FALSE;
	DWORD dwRead;
	OVERLAPPED osReader = { 0 };

	OVERLAPPED osWrite = { 0 };
	DWORD dwWritten;
	DWORD dwRes;
	BOOL fRes;

	char rdBuf[BUFFERSIZE];


	char command[10];

	//char lpBuf[7];
	
	bool InitSerial(char* comport, int baudrate);

	std::string HandleASuccessfulRead();
	
	bool CFind(char* bigstring, char* substring);

	bool Read(char** buf);

	BOOL WriteToCom(char* lpBuf);

	BOOL Close();

	DCB dcb = { 0 };
	

};