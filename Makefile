SHELL=/bin/bash

CC = gcc
LINK = gcc
LEX = flex
BISON = bison

CFLAGS = -std=c99 -I./include

SRC_DIR = src

OBJS = pro_actor.o pro_behavior.o pro_env.o pro_lookup.o pro_object.o pro_state.o

OUT_DIR = build
OUT_OBJS = $(addprefix $(OUT_DIR)/,$(OBJS))


all : $(OUT_OBJS)
	gcc -shared -Wl,-install_name,libprosopon.so.1 -o libprosopon.so.1.0.0 $<

$(OUT_DIR)/%.o : $(SRC_DIR)/%.c
	gcc $(CFLAGS) -c -fPIC $< -o $@

clean :
	rm -f $(OUT_DIR)/*
	rm libprosopon.so.*

