CC = gcc
CFLAGS = -Wall -O2
BIN_DIR = bin

UE_SRCS = ue/ue_main.c ue/sdap.c ue/pdcp.c ue/mac.c
GNB_SRCS = gnb/gnb_main.c gnb/mac.c gnb/pdcp.c gnb/sdap.c

.PHONY: all clean

all: $(BIN_DIR)/ue_bin $(BIN_DIR)/gnb_bin

$(BIN_DIR)/ue_bin: $(UE_SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/gnb_bin: $(GNB_SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@rm -rf $(BIN_DIR)
