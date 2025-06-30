#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[MAX_BUFFER_SIZE];
    int clientAddrSize;

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize winsock.\n");
        return -1;
    }

    // 소켓 생성
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        return -1;
    }

    // 서버 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(5001);

    // 소켓 바인딩
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Failed to bind socket.\n");
        return -1;
    }

    // 클라이언트의 연결 대기
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        printf("Failed to listen on socket.\n");
        return -1;
    }

    printf("서버가 실행 중입니다...\n");

    // 클라이언트 연결 수락
    clientAddrSize = sizeof(clientAddr);
    if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize)) == INVALID_SOCKET) {
        printf("Failed to accept client connection.\n");
        return -1;
    }

    /*struct in_addr ip_addr;

    // IP 정수값을 in_addr 구조체로 설정
    ip_addr.s_addr = htonl(clientAddr.sin_addr.S_un.S_addr); // 네트워크 바이트 순서로 변환

    // in_addr 구조체를 문자열로 변환
    char* ip_str = inet_ntoa(ip_addr);
    if (ip_str == NULL) {
        perror("inet_ntoa failed");
        return 1;
    }   

    printf("IP Address: %s\n", ip_str);*/

    printf("클라이언트가 연결되었습니다.\n");

    while (1) {
        // 클라이언트로부터 메시지 수신
        memset(buffer, 0, MAX_BUFFER_SIZE);
        int bytesRead = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
        if (bytesRead == SOCKET_ERROR || bytesRead == 0) {
            break;
        }

        printf("수신한 메시지: %s\n", buffer);

        // 클라이언트에게 응답 전송
        char response[MAX_BUFFER_SIZE];
        snprintf(response, MAX_BUFFER_SIZE, "서버가 메시지를 수신했습니다: %s", buffer);
        send(clientSocket, response, strlen(response), 0);
    }

    // 연결 종료
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
