// common/packet_print.h
#ifndef PACKET_PRINT_H
#define PACKET_PRINT_H

#include <stdio.h>
#include <string.h>
#include "../common/common_def.h"

/**
 * @brief Encapsulated Packet 상태를 계층별 블록으로 표 형태로 출력
 */
static void print_packet_layers(pdcp_pdu_t *pdcp_pdu) {
    sdap_pdu_t sdap;
    memcpy(&sdap, pdcp_pdu->sdap_buf, sizeof(sdap_pdu_t));

    printf("\n");
    printf("┌────────────┬────────────┬────────────────────────────┐\n");
    printf("│ PDCP Hdr   │ SDAP Hdr   │ Payload                   │\n");
    printf("├────────────┼────────────┼────────────────────────────┤\n");

    // PDCP Header (SEQ)
    char pdcp_str[32];
    snprintf(pdcp_str, sizeof(pdcp_str), "SEQ=%d", pdcp_pdu->seq_num);

    // SDAP Header (QFI)
    char sdap_str[32];
    snprintf(sdap_str, sizeof(sdap_str), "QFI=%d", sdap.qfi);

    // Payload
    char payload_str[64];
    snprintf(payload_str, sizeof(payload_str), "%.*s",
             (int)sdap.payload_len > 30 ? 30 : (int)sdap.payload_len,
             sdap.payload);

    printf("│ %-10s │ %-10s │ %-26s │\n", pdcp_str, sdap_str, payload_str);
    printf("└────────────┴────────────┴────────────────────────────┘\n\n");
}

#endif
