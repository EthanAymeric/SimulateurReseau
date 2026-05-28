TARGET_EXEC ?= SAE
TEST_EXEC ?= SAE_test
BUILD_DIR ?= ./build
TARGET_DIR ?= ./bin
SRC_DIRS ?= .

# All .c files, then exclude test/ dir and main.c for normal build
ALL_SRCS := $(shell find $(SRC_DIRS) -name "*.c")
SRCS := $(filter-out ./test/% ./main.c, $(ALL_SRCS))

# Test build: exclude main.c, include test/ dir
TEST_SRCS := $(filter-out ./main.c, $(ALL_SRCS))

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
TEST_OBJS := $(TEST_SRCS:%=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)
TEST_DEPS := $(TEST_OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CFLAGS ?= $(INC_FLAGS) -MMD -MP -O0 -Wall -Wextra -Wno-unused-parameter -g

# --- Default build (excludes test/ and main.c) ---
$(TARGET_DIR)/$(TARGET_EXEC): $(OBJS)
	$(MKDIR_P) $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# --- Test build (excludes main.c, includes test/) ---
.PHONY: test
test: $(TARGET_DIR)/$(TEST_EXEC)

$(TARGET_DIR)/$(TEST_EXEC): $(TEST_OBJS)
	$(MKDIR_P) $(dir $@)
	$(CC) $(TEST_OBJS) -o $@ $(LDFLAGS)

# --- Compile rule ---
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR) $(TARGET_DIR)

-include $(DEPS)
-include $(TEST_DEPS)
MKDIR_P ?= mkdir -p
