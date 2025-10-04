#include <stdio.h>
#include <string.h>
#include "../common/common_def.h"

void ue_sdap_process(uint8_t *app_data, uint16_t app_len, sdap_pdu_t *sdap_pdu);
void ue_pdcp_process(sdap_pdu_t *sdap_pdu, pdcp_pdu_t *pdcp_pdu);
void ue_mac_send(pdcp_pdu_t *pdcp_pdu);


int main(){
    const char *msg ="Hello from UE with SDAP";
    uint8_t app_buff[MAX_PAYLOAD_SIZE];
    uint16_t app_len=strlen(msg);
    memcpy(app_buff,msg,app_len);

    //2. SDAP 계층 처리
    sdap_pdu_t sdap_pdu; //SDAP PDU 구조체 변수 선언
    ue_sdap_process(app_buff, app_len, &sdap_pdu);

    //3. PDCP 계층 처리
    pdcp_pdu_t pdcp_pdu; //PDCP PDU 구조체 변수
    ue_pdcp_process(&sdap_pdu, &pdcp_pdu);

    //4. MAC 계층 처리 (UDP 소켓으로 gNB에 PDCP PDU 전송)
    ue_mac_send(&pdcp_pdu);

    return 0;

    /*
    sdap_pdu_t sdap_pdu;
→ 스택(stack)에 구조체 메모리를 할당
→ &sdap_pdu로 주소를 함수에 전달하면 함수 안에서 원본 수정 가능

memcpy(app_buf, msg, app_len)
→ 문자열 리터럴 → app_buf로 메모리 복사

함수 호출 순서가 곧 5G UE의 사용자 평면 데이터 송신 절차와 일치  
    */
}