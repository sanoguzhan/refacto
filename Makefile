SRC_DIR=src
INCLUDE_DIR=include
TEST_DIR=tests
BIN_DIR=bin
TEST_BIN_DIR=$(TEST_DIR)/$(BIN_DIR)

#LIBS = ""

CC=gcc

CFLAGS += -std=c11 -Wall -g -I$(INCLUDE_DIR)/
#CFLAGS := $(shell pkg-config --cflags $(LIBS)) $(CFLAGS)
#LDLIBS := $(shell pkg-config --libs $(LIBS)) $(LDLIBS)

COMPILE=$(CC) $(CFLAGS) -c $^ -o $@
LINK_EXE=$(CC) $(CFLAGS) -o $@ $^ 


SRC_FILES=$(wildcard $(SRC_DIR)/*.c)								
SRC_OBJS=$(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o, $(SRC_FILES))

TARGET = $(wildcard $(SRC_OBJS)/*.o
TEST_FILES=$(wildcard $(TEST_DIR)/*.c)
TEST_OBJS=$(patsubst $(TEST_DIR)/%.c,$(TEST_BIN_DIR)/%.o, $(TEST_FILES))
TEST_EXE=$(patsubst %.o,%, $(TEST_OBJS))
TEST_TARGETS=$(patsubst $(TEST_DIR)/%.c,$(TEST_BIN_DIR)/%, $(TEST_FILES))

$(shell if [ ! -d "${TEST_BIN_DIR}" ]; then mkdir -p ${TEST_BIN_DIR}; fi;)
$(shell if [ ! -d "${BIN_DIR}" ]; then mkdir -p ${BIN_DIR}; fi;)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(COMPILE)
$(TEST_BIN_DIR)/%.o: $(TEST_DIR)/%.c
	$(COMPILE)


.phony: all src clean clean-src clean-tests

all: src window test

src: $(SRC_OBJS)
test: $(TEST_OBJS) tests

clean-test:
	$(RM) $(TEST_OBJS)

clean-src:

	$(RM) window
	
clean:
	rm -rf $(BIN_DIR)/*

window: $(SRC_OBJS) 
	$(LINK_EXE)

TST=$(TEST_BIN_DIR)/
.SECONDEXPANSION:

tests:$(TST)test_grid clean-test

OBJS_BASE = grid
$(TST)test_grid: $$(call EXE_OBJS,$$@,$(OBJS_BASE)) $(TEST_BIN_DIR)/utils.o
	$(LINK_EXE)
define EXE_OBJS
	$(patsubst %, %.o, $(1)) $(patsubst %, $(BIN_DIR)/%.o, $(2))
endef