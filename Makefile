SHELL=/bin/bash

CC = gcc
LINK = glibtool --tag="junk"
DOC = doxygen

CFLAGS = -std=c99 -g -I./include
DOC_FLAGS = 

SRC_DIR = src
TEST_DIR = test
OUT_DIR = build



OBJS = pro_actor pro_list pro_env pro_lookup pro_object pro_state pro_constructor pro_library pro_message_queue pro_type pro_env_stack pro_user_data pro_messaging pro_lookup_list pro_lookup_table pro_binding_map pro_env_lookup

OUT_OBJS = $(addprefix $(OUT_DIR)/,$(OBJS))

TEST_OBJS = test test-pro_alloc test-pro_constructor test-pro_env test-pro_message test-pro_state test-user_data

OUT_TEST_OBJS = $(addprefix $(OUT_DIR)/,$(TEST_OBJS))


all : libprosopon.la

libprosopon.la : $(addsuffix .lo,$(OUT_OBJS))
	glibtool --tag="junk" --mode=link gcc -g -O -o libprosopon.la  $^ -rpath /usr/local/lib

$(OUT_DIR)/%.lo : $(SRC_DIR)/%.c
	glibtool --tag="junk" --mode=compile gcc $(CFLAGS) -fPIC -c $^ -o $@


test : $(addsuffix .o,$(OUT_OBJS)) $(addsuffix .o,$(OUT_TEST_OBJS))
	# Link test
	$(CC) $(CFLAGS) -lcunit -o $(OUT_DIR)/test $^ 
	
	# Run test
	./$(OUT_DIR)/test

$(OUT_DIR)/%.o : $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -I./$(SRC_DIR) -c -fPIC $< -o $@


install:
	glibtool --mode=install cp libprosopon.la /usr/local/lib/libprosopon.la


.PHONY : doc
doc :
	doxygen Doxyfile


.PHONY : clean
clean :
	rm -f $(OUT_DIR)/*
	if [ -f libprosopon.so.* ]; then rm libprosopon.so.*; fi