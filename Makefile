CC = gcc
CFLAGS = -Wall -O2

COMMON = common/common_def.h

UE_OBJS = ue/ue_main.c ue/sdap.c ue/pdcp.c ue/mac.c
GNB_OBJS = gnb/gnb_main.c gnb/mac.c gnb/pdcp.c gnb/sdap.c

all: ue gnb

ue: $(UE_OBJS) $(COMMON)
	$(CC) $(CFLAGS) -o ue_bin $(UE_OBJS)

gnb: $(GNB_OBJS) $(COMMON)
	$(CC) $(CFLAGS) -o gnb_bin $(GNB_OBJS)

clean:
	rm -f ue_bin gnb_bin
