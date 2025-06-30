#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    struct in_addr ipAddr;

    char buffer[MAX_BUFFER_SIZE];
    char message[MAX_BUFFER_SIZE];

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize winsock.\n");
        return -1;
    }

    // 소켓 생성
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        return -1;
    }

    // 서버 정보 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5001);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported.\n");
        return -1;
    }

    // 서버에 연결
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Failed to connect to server.\n");
        return -1;
    }

    while (1) {
        // 사용자로부터 메시지 입력
        printf("메시지를 입력하세요 (종료하려면 q 또는 Q): ");
        fgets(message, MAX_BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        if (strcmp(message, "q") == 0 || strcmp(message, "Q") == 0) {
            break;
        }

        // 서버로 메시지 전송
        send(clientSocket, message, strlen(message), 0);

        // 서버로부터 응답 수신
        memset(buffer, 0, MAX_BUFFER_SIZE);
        int bytesRead = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
        if (bytesRead == SOCKET_ERROR || bytesRead == 0) {
            break;
        }

        printf("서버로부터 받은 응답: %s\n", buffer);
    }

    // 연결 종료
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}