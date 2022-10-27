#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;                              // �ڵ��ȣ
	char message[BUF_SIZE];
	int strLen, i;

	SOCKADDR_IN servAdr, clntAdr;                                  //  ������������ü ����
	int clntAdrSize;

	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)       // ���� ���̺귯�� �ʱ�ȭ
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);       // ���� ����
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAdr, 0, sizeof(servAdr));                           //  �������� ����ü ���� �ʱ�ȭ
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)   // ���� �ּ����� �Ҵ�
		ErrorHandling("bind() error!");

	if (listen(hServSock, 5) == SOCKET_ERROR)                       //  �����û�� ������ ���·� �����
		ErrorHandling("listen() error!");

	clntAdrSize = sizeof(clntAdr);

	for (i = 0; i < 5; i++)
	{
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);           // Ŭ���̾�Ʈ�� ����
		if (hClntSock == -1)
			ErrorHandling("accept() error!");
		else
			printf("Connected client %d \n", i + 1);

		while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)       // Ŭ���̾�Ʈ�� ���������� �о� message ������ ����
			send(hClntSock, message, strLen, 0);                                   // Ŭ���̾�Ʈ�� �ٽ� ����

		closesocket(hClntSock);          // Ŭ���̾�Ʈ ���� ���� 
	}
	closesocket(hServSock);                // �������� ����                        
	WSACleanup();                            // ���϶��̺귯�� �ʱ�ȭ ����
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}