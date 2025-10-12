#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../common/common_def.h"
#include "../common/packet_print.h"

#define GNB_PORT 9000

extern void gnb_pdcp_receive(pdcp_pdu_t *pdcp_pdu);

/*
gNB 측 MAC 계층에서 UDP 소켓으로 PDCP PDU 수신
*/ 

void gnb_mac_receive_loop(){
    int sockfd=socket(AF_INET, SOCK_DGRAM, 0); //IPv4, UDP
    if(sockfd<0){
        perror("socket() failed");
        return;     
    }

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET; //IPv4
    addr.sin_port=htons(GNB_PORT); //포트 번호 설정
    addr.sin_addr.s_addr=INADDR_ANY; //모든 인터페이스에서 수

    if(bind(sockfd,(struct sockaddr *)&addr,sizeof(addr))<0){
        perror("bind() failed");
        close(sockfd);
        return;
    }
     printf("[gNB][MAC] Listening on UDP port %d...\n", GNB_PORT);

    pdcp_pdu_t pdcp_buf;
    while (1) {
        ssize_t n = recvfrom(sockfd, &pdcp_buf, sizeof(pdcp_buf), 0, NULL, NULL);
        if (n > 0) {
            printf("[gNB][MAC] Received UDP PDU (len=%ld)\n", n);
            gnb_pdcp_receive(&pdcp_buf);
        }
    }

    close(sockfd);
}