CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -pedantic -std=c99 -Isrc
LDFLAGS  =

SRC_DIR  = src
BUILD    = build
TEST_DIR = tests

SRCS     = $(wildcard $(SRC_DIR)/*.c)
OBJS     = $(patsubst $(SRC_DIR)/%.c, $(BUILD)/%.o, $(SRCS))

NAME     = kompressor
TARGET   = $(BUILD)/$(NAME)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)

.PHONY: all clean
