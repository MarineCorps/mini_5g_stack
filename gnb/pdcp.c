// gnb/pdcp.c
#include <stdio.h>
#include <string.h>
#include "../common/common_def.h"
#include "../common/packet_print.h"
// 다음 계층 함수 선언
extern void gnb_sdap_process(sdap_pdu_t *sdap_pdu);

/**
 * @brief gNB PDCP 계층: PDCP PDU에서 SDAP PDU를 추출
 */
void gnb_pdcp_receive(pdcp_pdu_t *pdcp_pdu)
{
    printf("[gNB][MAC] Received Encapsulated Packet:\n");
    print_packet_layers(pdcp_pdu);

    printf("[gNB][PDCP] ── Parsing PDCP PDU ──\n");
    printf("  seq_num : %d\n", pdcp_pdu->seq_num);
    printf("  sdap_len: %d bytes\n", pdcp_pdu->sdap_len);

    // SDAP PDU 복원
    sdap_pdu_t sdap_pdu;
    memcpy(&sdap_pdu, pdcp_pdu->sdap_buf, sizeof(sdap_pdu_t));

    // SDAP 계층으로 전달
    gnb_sdap_process(&sdap_pdu);
}
