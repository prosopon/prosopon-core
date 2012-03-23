SHELL=/bin/bash

CC = gcc
LINK = gcc

CFLAGS = -std=c99 -I./include

SRC_DIR = src
TEST_DIR = test
OUT_DIR = build

OBJS = pro_actor.o pro_behavior.o pro_env.o pro_lookup.o pro_object.o pro_state.o
OUT_OBJS = $(addprefix $(OUT_DIR)/,$(OBJS))


all : $(OUT_OBJS)
	$(LINK) -shared -Wl,-install_name,libprosopon.so.1 -o libprosopon.so.1.0.0 $^

$(OUT_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -fPIC $< -o $@


test : $(OUT_OBJS) $(OUT_DIR)/test.o
	# Link test
	$(CC) $(CFLAGS) -lcunit -o $(OUT_DIR)/test $^ 
	
	# Run test
	./$(OUT_DIR)/test

$(OUT_DIR)/test.o : $(TEST_DIR)/test.c
	$(CC) $(CFLAGS) -c -fPIC $< -o $@

clean :
	rm -f $(OUT_DIR)/*
	if [ -f libprosopon.so.* ]; then rm libprosopon.so.*; fi