#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;                              // 핸들번호
	char message[BUF_SIZE];
	int strLen, i;

	SOCKADDR_IN servAdr, clntAdr;                                  //  소켓정보구조체 변수
	int clntAdrSize;

	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)       // 소켓 라이브러리 초기화
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);       // 소켓 생성
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAdr, 0, sizeof(servAdr));                           //  소켓정보 구조체 변수 초기화
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)   // 소켓 주소정보 할당
		ErrorHandling("bind() error!");

	if (listen(hServSock, 5) == SOCKET_ERROR)                       //  연결요청이 가능한 상태로 만들기
		ErrorHandling("listen() error!");

	clntAdrSize = sizeof(clntAdr);

	for (i = 0; i < 5; i++)
	{
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);           // 클라이언트와 연결
		if (hClntSock == -1)
			ErrorHandling("accept() error!");
		else
			printf("Connected client %d \n", i + 1);

		while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)       // 클라이언트의 전송정보를 읽어 message 변수에 저장
			send(hClntSock, message, strLen, 0);                                   // 클라이언트에 다시 전송

		closesocket(hClntSock);          // 클라이언트 소켓 종료 
	}
	closesocket(hServSock);                // 서버소켓 종료                        
	WSACleanup();                            // 소켓라이브러리 초기화 해제
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}