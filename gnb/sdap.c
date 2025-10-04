// gnb/sdap.c
#include <stdio.h>
#include <string.h>
#include "../common/common_def.h"

/**
 * @brief 헥사 덤프 함수 (디버깅용)
 */
/*
static void hexdump(const void *data, size_t size) {
    const unsigned char *p = (const unsigned char*)data;
    for (size_t i = 0; i < size; i++) {
        if (i % 16 == 0) {
            printf("%04zx: ", i);
        }
        printf("%02x ", p[i]);
        if (i % 16 == 15 || i == size - 1) {
            // ASCII도 같이 출력
            size_t start = i / 16 * 16;
            size_t end = i;
            printf(" ");
            for (size_t j = start; j <= end; j++) {
                unsigned char c = p[j];
                printf("%c", (c >= 32 && c <= 126) ? c : '.');
            }
            printf("\n");
        }
    }
}
*/

/**
 * @brief gNB SDAP 계층: SDAP PDU 파싱 및 Payload 출력
 */
void gnb_sdap_process(sdap_pdu_t *sdap_pdu)
{
    printf("[gNB][SDAP] ── Parsing SDAP PDU ──\n");
    printf("  QFI      : %d\n", sdap_pdu->qfi);
    printf("  Reserved : %d\n", sdap_pdu->reserved);
    printf("  Payload  : \"%.*s\"\n", sdap_pdu->payload_len, sdap_pdu->payload);

}
