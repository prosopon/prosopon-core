SHELL=/bin/bash

prefix = /usr/local
exec_prefix = $(prefix)
libdir = $(exec_prefix)/lib
includedir = $(prefix)/include


LIBTOOL = glibtool --tag="junk"
CC = gcc
DOC = doxygen

CFLAGS = -std=c99 -g -I./include
DOC_FLAGS = 

srcdir = src
TEST_DIR = test
BUILD_DIR = build
SRC_INCLUDE_DIR = include/prosopon

LIBPROSOPON = libprosopon.la
PROSOPON_HEADER_DIR = $(includedir)/prosopon


OBJS = pro_actor pro_list pro_env pro_lookup pro_object pro_state pro_constructor pro_library pro_message_queue pro_type pro_env_stack pro_user_data pro_messaging pro_lookup_list pro_lookup_table pro_binding_map pro_env_lookup

OUT_OBJS = $(addprefix $(BUILD_DIR)/,$(OBJS))

HEADERS = prosopon.h actor_type.h config.h library.h core.h
OUT_HEADERS = $(addprefix $(SRC_INCLUDE_DIR)/,$(HEADERS))


TEST_OBJS = test test-pro_alloc test-pro_constructor test-pro_env test-pro_message test-pro_state test-user_data

OUT_TEST_OBJS = $(addprefix $(BUILD_DIR)/,$(TEST_OBJS))





all : $(LIBPROSOPON)

$(LIBPROSOPON) : $(addsuffix .lo,$(OUT_OBJS))
	$(LIBTOOL) --mode=link gcc $(CFLAGS) -O -o $@  $^ -rpath $(libdir)

$(BUILD_DIR)/%.lo : $(srcdir)/%.c
	$(LIBTOOL) --mode=compile gcc $(CFLAGS) -fPIC -c $^ -o $@


install: $(LIBPROSOPON) copy_headers	
	$(LIBTOOL) --mode=install cp $< $(libdir)/$<

copy_headers: 
	if [ ! -d $(PROSOPON_HEADER_DIR) ]; then mkdir $(PROSOPON_HEADER_DIR); fi
	cp $(OUT_HEADERS) $(PROSOPON_HEADER_DIR)
	

check : $(addsuffix .o,$(OUT_OBJS)) $(addsuffix .o,$(OUT_TEST_OBJS))
	# Link test
	$(CC) $(CFLAGS) -lcunit -o $(BUILD_DIR)/test $^ 
	
	# Run test
	./$(BUILD_DIR)/test

$(BUILD_DIR)/%.o : $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -I./$(srcdir) -c -fPIC $< -o $@



.PHONY : doc
doc :
	doxygen Doxyfile



.PHONY : clean
clean :
	rm -f $(BUILD_DIR)/*
	if [ -f $(LIBPROSOPON) ]; then rm $(LIBPROSOPON); fi