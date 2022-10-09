#빡공팟 #TeamH4C #P4C #세번째과제

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#pragma comment(lib, "ws2_32.lib")


WSADATA     wsaData;
SOCKET      sock0;
SOCKET      sockw;
struct      sockaddr_in addr;
struct      sockaddr_in client;
#define PORT_NUM 7514 


int getAddrHost(void) {
    int i = 0;
    HOSTENT* lpHost;      
    IN_ADDR inaddr;      
    char szBuf[256], szIP[16];  

    
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        printf("WSAStartup 에러\n");
        return -1;
    }

   
    gethostname(szBuf, (int)sizeof(szBuf));

   
    lpHost = gethostbyname(szBuf);
    
    while (lpHost->h_addr_list[i]) {
        memcpy(&inaddr, lpHost->h_addr_list[i], 4);
        i++;
    }
    strcpy(szIP, inet_ntoa(inaddr));
    printf("build server: ", szIP, PORT_NUM);

    WSACleanup();

    return 0;
}

int main() {
    int len;
    int n;
    int sockaddr_in_size = sizeof(struct sockaddr_in);
    int recv_len = 0;
    unsigned char buf[1024];
    unsigned char path[1024];
    unsigned char html[1024];
    
    if (getAddrHost() != 0) {
        printf("IP 주소 얻는데에 실패하였습니다.");
        _getch();
        return -1;
    }
    
    if (WSAStartup(MAKEWORD(2, 0), &wsaData)) {
        puts("윈속 리셋 실패하였습니다.");
        _getch();
        return -1;
    }
  
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock0 == INVALID_SOCKET) {
        printf("socket : %d\n", WSAGetLastError());
        _getch();
        return -1;
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUM);
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    
    if (bind(sock0, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        printf("bind : %d\n", WSAGetLastError());
        _getch();
        return -1;
    }
    
    if (listen(sock0, 5) != 0) {
        printf("listen : %d\n", WSAGetLastError());
        _getch();
        return -1;
    }
   

    while (1) {
        len = sizeof(client);
        sockw = accept(sock0, (struct sockaddr*)&client, &len);
        if (sockw == INVALID_SOCKET) {
            printf("accept : %d\n", WSAGetLastError());
            break;
        }
       
        memset(path, 0, 1024);
        memset(html, 0, 1024);
        
        recv_len = recvfrom(sockw, buf, 1024, 0, (struct sockaddr*)&client, &sockaddr_in_size);
        buf[recv_len - 1] = 0;
        if (buf[0] == '\0')
            strcpy(buf, NULL);
        
        printf("%s \n", buf);
       
        for (int i = 0; i < strlen(buf); i++) {
            printf("%d\n", i);
            if (buf[i] == 'G' && buf[i + 1] == 'E' && buf[i + 2] == 'T' && buf[i + 3] == ' ') {
                for (int j = 0; buf[i + 4 + j] != ' '; j++) {
                    path[j] = buf[i + 4 + j];
                }
                break;
            }
            else if (buf[i] == 'P' && buf[i + 1] == 'O' && buf[i + 2] == 'S' && buf[i + 3] == 'T' && buf[i + 4] == ' ') {
                for (int j = 0; buf[i + 4 + j] != ' '; j++)
                    path[j] = buf[i + 4 + j];
                break;
            }
        }
        printf("request: %s \n", path);
       
        unsigned char* header =
            "HTTP/1.0 200 OK\n"
            "Content-type: text/html\n"
            "\n";
        send(sockw, header, strlen(header), 0);
        

        if (strcmp(path, "/index") == 0) {
            strcpy(html,
                "<!DOCTYPE html>\n"
                "<html lang = \"ko\">\n"
                "<head>\n"
                "<meta http-equiv=\"Content - Type\" content=\"text / html; charset = utf - 8\" />\n"
                "<title>Hard Study</title>"
                "</head>\n"
                "<body>\n"
                "<h1>빡공팟 3주차 과제 제출 - 김지우</h1>\n"
                "빡공팟 3주차 과제 C언어로 서버구현<br>\n"
                "<a href=\"/page2\">->page2</a>\n"
                "</body>\n"
                "</html>");
        }
        else if (strcmp(path, "/page2") == 0) {
            strcpy(html,
                "<!DOCTYPE html>\n"
                "<html lang = \"ko\">\n"
                "<head>\n"
                "<meta http-equiv=\"Content - Type\" content=\"text / html; charset = utf - 8\" />\n"
                "</head>\n"
                "<body>\n"
                "<h1>C언어로 HTTP 서버 구현</h1>\n"
                "<a href=\"/index\">->index</a>\n"
                "</body>\n"
                "</html>");
        }
        else {
            strcpy(html,
                "<!DOCTYPE html>\n"
                "<html lang = \"ko\">\n"
                "<head>\n"
                "<meta http-equiv=\"Content - Type\" content=\"text / html; charset = utf - 8\" />\n"
                "</head>\n"
                "<body>\n"
                "<h1>404 - Not Found</h1>\n"
                "</body>\n"
                "</html>");
        }
       
        if (send(sockw, html, strlen(html), 0) < 1) {
            printf("send : %d\n", WSAGetLastError());
            break;
        }
      
        closesocket(sockw);
    }
  
    closesocket(sock0);
    WSACleanup();
    return 0;
}
