#include "Serial.h"
#include <string>


BOOL Serial::Close() {
	return CloseHandle(osReader.hEvent);
	
}



bool Serial::Read(char** buf) {

	bool flag = false;

	if (!fWaitingOnRead) {
		// Issue read operation.
		if (!ReadFile(hCom, rdBuf, BUFFERSIZE, &dwRead, &osReader)) {
			if (GetLastError() != ERROR_IO_PENDING)     // read not delayed?
														// Error in communications; report it.
				printf("error in communication read\n");
			else
				fWaitingOnRead = TRUE;
		}
		else {
			// read completed immediately
			
			flag = true;
			rdBuf[dwRead] = '\0';
			//memcpy(buf, rdBuf, BUFFERSIZE * sizeof(char));
			//memset(buf, *rdBuf, BUFFERSIZE);
			*buf = rdBuf;
			//PurgeComm(hCom, PURGE_RXCLEAR);
			//*str = HandleASuccessfulRead(rdBuf, dwRead);
			
		}
	}



	if (fWaitingOnRead) {
		dwRes = WaitForSingleObject(osReader.hEvent, 500);
		switch (dwRes)
		{
			// Read completed.
		case WAIT_OBJECT_0:
			if (!GetOverlappedResult(hCom, &osReader, &dwRead, FALSE))
				// Error in communications; report it.
				printf("error in communication read\n");
			else {
				// Read completed successfully.
				//flag = HandleASuccessfulRead(rdBuf, dwRead);
				flag = true;
				rdBuf[dwRead] = '\0';
				//*str = HandleASuccessfulRead(rdBuf, dwRead);
				//memcpy(buf, rdBuf, BUFFERSIZE *sizeof(char));
				//memcpy()
				*buf = rdBuf;
				//PurgeComm(hCom, PURGE_RXCLEAR);
			}
			//  Reset flag so that another opertion can be issued.
			fWaitingOnRead = FALSE;
			break;

		case WAIT_TIMEOUT:
			// Operation isn't complete yet. fWaitingOnRead flag isn't
			// changed since I'll loop back around, and I don't want
			// to issue another read until the first one finishes.
			//
			// This is a good time to do some background work.
			break;

		default:
			// Error in the WaitForSingleObject; abort.
			// This indicates a problem with the OVERLAPPED structure's
			// event handle.
			break;
		}
	}

	return flag;
}

bool Serial::InitSerial(char* comport, int baudrate) {
	command[0] = '\0';
	//lpBuf[] = "wrfiohwefjf\nl1\n\0";
	
	hCom = CreateFileA(comport,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0);
	if (hCom == INVALID_HANDLE_VALUE) {
		printf("serial open fked up\n");
		return false;
	}

	dcb.DCBlength = sizeof(DCB);

	if (!::GetCommState(hCom, &dcb))
	{
		printf("comm state not gotten\n");
		return false;
	}

	dcb.BaudRate = baudrate;
	dcb.ByteSize = 8;
	dcb.Parity = 0;
	dcb.StopBits = ONESTOPBIT;


	if (!::SetCommState(hCom, &dcb))
	{
		printf("Couldnt set comm state\n");
		return false;
	}


	
	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osReader.hEvent == NULL) {
		// error creating overlapped event handle
		printf("error creating overlapped read event handle\n");
		return false;
	}
	
	return true;

}

bool Serial::CFind(char* bigstring, char* substring) {

	std::string s1(bigstring);
	std::string s2(substring);


	if (s1.find(s2) != std::string::npos) {
		return true;
	} else return false;
}

std::string Serial::HandleASuccessfulRead(){
	
	//rdBuf[dwRead] = '\0';
	
	std::string s1(rdBuf);

	//if (CFind(rdBuf, baby)) {
	//	printf("%s", rdBuf);
	//	memset(rdBuf, 0, strlen(rdBuf));
	//	//rdBuf[0] = '\0';	
	//	return true;
	//}
	//memset(rdBuf, 0, strlen(rdBuf));
	return s1;
}

BOOL Serial::WriteToCom(char* lpBuf){

	DWORD dwToWrite = (DWORD)strlen(lpBuf);

	//OVERLAPPED osWrite = { 0 };
	//DWORD dwWritten;
	//DWORD dwRes;
	//BOOL fRes;

	
	// Create the overlapped event. Must be closed before exiting
	// to avoid a handle leak.

	// Create this write operation's OVERLAPPED structure's hEvent.
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osWrite.hEvent == NULL){
		// error creating overlapped event handle
		printf("error creating overlapped write event handle\n");
		return FALSE;
	}

	// Issue write.
	if (!WriteFile(hCom, lpBuf, dwToWrite, &dwWritten, &osWrite)) {
		if (GetLastError() != ERROR_IO_PENDING) {
			// WriteFile failed, but isn't delayed. Report error and abort.
			printf("WriteFile failed, but isn't delayed.\n");
			fRes = FALSE;
		}
		else
			// Write is pending.
			dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
		switch (dwRes)
		{
			// OVERLAPPED structure's event has been signaled. 
		case WAIT_OBJECT_0:
			if (!GetOverlappedResult(hCom, &osWrite, &dwWritten, FALSE)){
				fRes = FALSE;
				//printf("Write operation success.\n");
			}
			else {
				printf("Write operation success.\n");
				// Write operation completed successfully.
				fRes = TRUE;
			}
			break;

		default:
			// An error has occurred in WaitForSingleObject.
			// This usually indicates a problem with the
			// OVERLAPPED structure's event handle.
			fRes = FALSE;
			break;
		}
	}
	else{
		fRes = TRUE;
		// WriteFile completed immediately.

	}
	CloseHandle(osWrite.hEvent);
	return fRes;
}