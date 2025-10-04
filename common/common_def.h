// common_def.h 파일을 작성하는 이유
// c언어 프로젝트에서 헤더파일은 설계도 , 공용게시판 역할을함
// 여러 소스파일에서 공통으로 사용되는 상수, 매크로, 데이터 구조체, 함수 선언 등을 정의
// 코드의 재사용성을 높이고 유지보수를 용이하게함
// 프로젝트의 일관성을 유지하고 협업을 원활하게함


#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include <stdint.h>

#define MAX_PAYLOAD_SIZE 512
#define SDAP_HEADER_SIZE 2 //QoS Flow ID(18) + reserved(18)
#define PDCP_HEADER_SIZE 2 //seq_num(28)
#define RLC_HEADER_SIZE 0 //생략
#define MAC_HEADER_SIZE 0 //생략

// 간단한 QIF 정의 (5G 에서는 0~ 63 까지 사용)

typedef enum{
    QFI_DEFAULT = 9
}qfi_t;

//typedef enum은 열거형(enum)을 사용하여 정수형상수에
//기호 이름을 부여하는 동시에 새로운 자료형처럼사용하기위해
//typedef 키워드를 사용

// SDAP PDU 구조체 정의
typedef struct {
    uint8_t qfi; // QoS Flow ID (6 bits)
    uint8_t reserved; // alignment용
    uint8_t payload[MAX_PAYLOAD_SIZE]; // 상위 계층 데이터
    uint16_t payload_len;
} sdap_pdu_t;

// PDCP 구조체 정의
typedef struct{
    uint16_t seq_num; // Sequence Number (12 bits)
    uint8_t sdap_buf[MAX_PAYLOAD_SIZE + SDAP_HEADER_SIZE]; // 상위 계층 데이터
    uint16_t sdap_len;
} pdcp_pdu_t;

// 단순 송신 버퍼
typedef struct{
    uint8_t buf[1024];
    uint16_t len;
} mac_buffer_t;

#endif