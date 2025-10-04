#include <stdio.h>
#include <string.h>
#include "../common/common_def.h"
#include "../common/packet_print.h"

// SDAP PDU 구조체를 입력받아, PDCP 계층에서 시퀀스 넘버를 붙이고
// PDCP PDU로 변환


// PDCP 시퀀스 번호를 전역(static) 변수로 관리
// static -> 이 파일 내에서만 접근 가능

static uint16_t pdcp_seq = 0; //12비트 PDCP 시퀀스 번호, 0~4095

/*
 UE PCDP: SDAP PDU를 입력받아 PDCP PDU로 변환
    @param sdap_pdu: SDAP 계층에서 내려온 구조체의 주소
    @param pdcp_pdu: 변환된 PDCP PDU를 저장할 구조체의 주소
*/

void ue_pdcp_process(sdap_pdu_t *sdap_pdu, pdcp_pdu_t *pdcp_pdu)
// sdap_pdu *sdap_pdu -> 이전 계층에서 생성된 SDAP 구조체의 주소
//                    -> 이 구조체 전체를 PDCP payload로 취급
// pdcp_pdu *pdcp_pdu -> 변환된 PDCP PDU를 저장할 구조체의 주소

{
    //1. PDCP 시퀀스 넘버 부여
    pdcp_pdu->seq_num = pdcp_seq++; // 전송마다 seq_num 1씩 증가

    //2. SDAP PDU 구조체 전체를 PDCP PDU 내부의 버퍼로 복사
    // - 목적지: pdcp_pdu->sdap_buf (PDCP payload 역할)
    // - 원본: sdap_pdu (SDAP PDU 전체 구조체 시작 주소)
    // - 길이: sizeof(sdap_pdu_t) (SDAP PDU 구조체 전체 크기)
    memcpy(pdcp_pdu->sdap_buf, (uint8_t *)sdap_pdu, sizeof(sdap_pdu_t));
    pdcp_pdu->sdap_len = sizeof(sdap_pdu_t);

    
    printf("[UE][PDCP] Attached PDCP header (seq=%d)\n", pdcp_pdu->seq_num);    
    print_packet_layers(pdcp_pdu);
}