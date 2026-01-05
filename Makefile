CC := clang
AR := ar
CFLAGS := -Wall -Wextra -Werror -pedantic -std=c11
INCLUDE_DIR := include
SOURCE_DIR := src
BUILD_DIR := build
INCLUDES := -I$(INCLUDE_DIR)
SOURCES := $(shell find $(SOURCE_DIR) -type f -name '*.c')
TARGET := libhashmap.a

_TARGET = $(addprefix $(BUILD_DIR)/, $(TARGET))
_OBJS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

all: CFLAGS += -O2
all: $(_TARGET)
	mv $(_TARGET) .

debug: CFLAGS += -g -O0
debug: $(_TARGET)
	mv $(_TARGET) .

$(_TARGET): $(_OBJS)
	$(AR) rcs $@ $^

$(_OBJS): $(BUILD_DIR)/%.o: src/%.c
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)

test: $(_TARGET)
	make clean
	make all
	$(CC) -o build/test test/main.c -L. -lhashmap $(INCLUDES) $(CFLAGS)

lazy:
	make clean
	make all
	./$(TARGET)

clean:
	rm -rf build/*
	@if [ -f $(TARGET) ]; then rm $(TARGET); fi

setup:
	$(shell [ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR))
	$(shell [ -d $(SOURCE_DIR) ] || mkdir -p $(SOURCE_DIR))
	$(shell [ -d $(INCLUDE_DIR) ] || mkdir -p $(INCLUDE_DIR))

dev:
	bear -- make all

.PHONY: clean all test dev lazy debug setup
