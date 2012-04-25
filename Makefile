SHELL=/bin/bash

LIBTOOL = glibtool --tag="junk"
CC = gcc

DOC = doxygen

CFLAGS = -std=c99 -g -I./include
DOC_FLAGS = 

SRC_DIR = src
TEST_DIR = test
OUT_DIR = build


OUT_LIB = libprosopon.la
RPATH = /usr/local/lib\

OBJS = pro_actor pro_list pro_env pro_lookup pro_object pro_state pro_constructor pro_library pro_message_queue pro_type pro_env_stack pro_user_data pro_messaging pro_lookup_list pro_lookup_table pro_binding_map pro_env_lookup

OUT_OBJS = $(addprefix $(OUT_DIR)/,$(OBJS))

TEST_OBJS = test test-pro_alloc test-pro_constructor test-pro_env test-pro_message test-pro_state test-user_data

OUT_TEST_OBJS = $(addprefix $(OUT_DIR)/,$(TEST_OBJS))


all : $(OUT_LIB)

$(OUT_LIB) : $(addsuffix .lo,$(OUT_OBJS))
	$(LIBTOOL) --mode=link gcc -g -O -o $(OUT_LIB)  $^ -rpath $(RPATH)

$(OUT_DIR)/%.lo : $(SRC_DIR)/%.c
	$(LIBTOOL) --mode=compile gcc $(CFLAGS) -fPIC -c $^ -o $@


test : $(addsuffix .o,$(OUT_OBJS)) $(addsuffix .o,$(OUT_TEST_OBJS))
	# Link test
	$(CC) $(CFLAGS) -lcunit -o $(OUT_DIR)/test $^ 
	
	# Run test
	./$(OUT_DIR)/test

$(OUT_DIR)/%.o : $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -I./$(SRC_DIR) -c -fPIC $< -o $@


install:
	$(LIBTOOL) --mode=install cp $(OUT_LIB) /usr/local/lib/$(OUT_LIB)


.PHONY : doc
doc :
	doxygen Doxyfile


.PHONY : clean
clean :
	rm -f $(OUT_DIR)/*
	if [ -f libprosopon.so.* ]; then rm libprosopon.so.*; fi