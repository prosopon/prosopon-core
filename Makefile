SHELL=/bin/bash

CC = gcc
LINK = gcc
DOC = doxygen

CFLAGS = -std=c99 -I./include
DOC_FLAGS = 

SRC_DIR = src
TEST_DIR = test
OUT_DIR = build

OBJS = pro_actor.o pro_message.o pro_env.o pro_lookup.o pro_object.o pro_state.o pro_constructor.o
OUT_OBJS = $(addprefix $(OUT_DIR)/,$(OBJS))


all : $(OUT_OBJS) 
	$(LINK) -shared $^ -lc -Wl,-install_name,libprosopon.so.1 -o libprosopon.so.1

$(OUT_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c $^ -o $@


test : $(OUT_OBJS) $(OUT_DIR)/test.o
	# Link test
	$(CC) $(CFLAGS) -lcunit -o $(OUT_DIR)/test $^ 
	
	# Run test
	./$(OUT_DIR)/test

$(OUT_DIR)/test.o : $(TEST_DIR)/test.c
	$(CC) $(CFLAGS) -c -fPIC $< -o $@


install:
	cp libprosopon.so.1 /usr/local/lib/

.PHONY : doc
doc :
	doxygen Doxyfile


.PHONY : clean
clean :
	rm -f $(OUT_DIR)/*
	if [ -f libprosopon.so.* ]; then rm libprosopon.so.*; fi