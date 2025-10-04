#include <stdio.h>
#include <string.h>
#include <arpa/inet.h> // inet_ptan, sockadd_in 함수 사용을 위한 헤더
#include <unistd.h> // close()
#include "../common/common_def.h"

#define GNB_IP "127.0.0.1"
#define GNB_PORT 9000

/*
PDCP PDU를 UDP 소켓으로 gNB에 송신

pdcp_pdu 상위(pdcp)계층에서 내려온 PDCP PDU 구조체의 주소
*/

void ue_mac_send(pdcp_pdu_t *pdcp_pdu){

    //1. UDP 소켓 생성

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); //IPv4, UDP
    if(sockfd < 0){
        perror("socket() failed");
        return;
    }

    //2. gNB 주소 설정

    struct sockaddr_in gnb_addr;
    memset(&gnb_addr,0,sizeof(gnb_addr));
    gnb_addr.sin_family = AF_INET; //IPv4
    gnb_addr.sin_port = htons(GNB_PORT); //포트 번호 설정, htons
    inet_pton(AF_INET, GNB_IP, &gnb_addr.sin_addr); //IP 주소 설정
    
    //3. UDP 소켓으로 PDCP PDU 송신

    ssize_t sent_len =sendto(sockfd,
    (uint8_t *)pdcp_pdu, //보낼 데이터의 시작 주소
    sizeof(pdcp_pdu_t), //보낼 데이터의 길이
    0, //플래그 (일반적으로 0)
    (struct sockaddr *)&gnb_addr, //gNB 주소 구조체의 포인터
    sizeof(gnb_addr) //주소 구조체의 크기  
);

if(sent_len<0){
    perror("sendto() failed");
}
else{
    printf("[UE][MAC] Sent PDCP PDU (len=%ld) to gNB %s:%d\n",
    sent_len, GNB_IP, GNB_PORT);
}
    //4. 소켓 닫기
    close(sockfd);
}