// gnb/gnb_main.c
#include <stdio.h>

extern void gnb_mac_receive_loop();

int main() {
    gnb_mac_receive_loop();
    return 0;
}
