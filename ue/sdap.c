#include <stdio.h>
#include <string.h>
#include "../common/common_def.h"

/*
 UE 측 SDAP 계층에서 Application 데이터를 SDAP PDU로 변환
 @param app_data: 상위 계층 에서 전달된 원본 데이터 버퍼의 시작 주소
 @param app_data_len: 상위 계층에서 전달된 원본 데이터의 길이(바이트 단위)
 @param sdap_pdu: 변환된 SDAP PDU를 저장할 구조체의 주소
*/

void ue_sdap_process(uint8_t *app_data, uint16_t app_len, sdap_pdu_t *sdap_pdu)
{
    //1. QFI 헤더 세팅
    //QFI 는 SDAP 계층에서 트래픽의 QoS 흐름을 식별하는데 사용
    //여기서는 common_def.h 에서 정의한 QFI_DEFAULT 값 사용
    sdap_pdu->qfi=QFI_DEFAULT; //구조체 멤버 접근: sdap_pdu는 포인터이므로 -> 사용
    sdap_pdu->reserved=0; //alignment 용도(사용 안함)

    //2. Application 데이터를 SDAP payload에 복사
    //memcpy(목적지, 원본, 길이)
    //- 목적지: sdap_pdu 구조체 내부의 payload 배열의 시작주소
    //- 원본: app_data 포인터가 가리키는 메모리
    //- 길이: app_data_len 바이트 만큼

    memcpy(sdap_pdu -> payload, app_data, app_len);

    //3. SDAP PDU 길이 설정
    sdap_pdu -> payload_len = app_len;

    //4. 로그 출력
    printf("[UE][SDAP] Enacapsulated App data(len=%d) with QFI=%d\n", 
        app_len, sdap_pdu->qfi);
}