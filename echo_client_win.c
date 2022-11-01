#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	int strLen;
	SOCKADDR_IN servAdr;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));                   // 초기화 
	servAdr.sin_family = AF_INET;                                // IPv4 주소체계
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);          // 서버 IP 주소                     --          //
	servAdr.sin_port = htons(atoi(argv[2]));                    //  서버프로그램 포트 번호   --  프로그램 실행 할 때 입력

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)        // 서버에 연결요청
		ErrorHandling("connect() error");
	else
		puts("Connected .........");

	while (1)
	{
		fputs("Input message(Q to quit) :  ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		//-----------------------------   잘못된 부분
		send(hSocket, message, strlen(message), 0);
		strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
		message[strLen] = 0;
		printf("Message from server : %s", message);
	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}