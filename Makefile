###########
# configs #
###########

# target name
TARGET = libttui

# directories
INC_DIR = include
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = test

# flags
CXXFLAGS = $(addprefix -I, $(INC_DIR)) -std=c++14
DBG_FLAGS = -Wall -pedantic-errors -g
REL_FLAGS = -O3
TEST_FLAGS = $(DBG_FLAGS) -I$(TEST_DIR)

#########
# files #
#########

# files
SRCS = $(shell find $(SRC_DIR) -name *.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=%.o)

# dependencies
-include $(OBJS:.o=.d)

#########
# debug #
#########
DBG_BUILD_DIR = $(BUILD_DIR)/debug
DBG_LIB = $(DBG_BUILD_DIR)/lib/$(TARGET).a
DBG_OBJS = $(addprefix $(DBG_BUILD_DIR)/obj/, $(OBJS))

debug: CXXFLAGS += $(DBG_FLAGS)
debug: $(DBG_LIB)
	@echo Target debug finished.
	@echo

# lib
$(DBG_LIB): $(DBG_OBJS)
	mkdir -p $(dir $@)
	ar rcs $@ $(DBG_OBJS)

# objects
$(DBG_BUILD_DIR)/obj/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< $(CXXFLAGS) -MMD -MP

###########
# release #
###########
REL_BUILD_DIR = $(BUILD_DIR)/release
REL_LIB = $(REL_BUILD_DIR)/lib/$(TARGET).a
REL_OBJS = $(addprefix $(REL_BUILD_DIR)/obj/, $(OBJS))

release: CXXFLAGS += $(REL_FLAGS)
release: $(REL_LIB)
	@echo Target release finished.
	@echo

# lib
$(REL_LIB): $(REL_OBJS)
	mkdir -p $(dir $@)
	ar rcs $@ $(REL_OBJS)

# objects
$(REL_BUILD_DIR)/obj/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< $(CXXFLAGS) -MMD -MP

########
# test #
########
MAIN_OBJ = main.o

TEST_BUILD_DIR = $(BUILD_DIR)/test
TEST_EXE = $(TEST_BUILD_DIR)/bin/$(TARGET)-test
TEST_CXXFLAGS = $(CXXFLAGS) $(TEST_FLAGS)

TEST_TEST_SRCS = $(shell find $(TEST_DIR) -name *.cpp)
TEST_TEST_OBJS = $(TEST_TEST_SRCS:$(TEST_DIR)/%.cpp=%.o)
TEST_TEST_BUILD_DIR = $(TEST_BUILD_DIR)/test
TEST_TOT_OBJS = $(addprefix $(TEST_TEST_BUILD_DIR)/, $(TEST_TEST_OBJS))

-include $(TEST_TOT_OBJS:.o=.d)
test_build: LDFLAGS = -L./$(DBG_BUILD_DIR)/lib -lttui
test_build: debug $(TEST_EXE)
	@echo Target test finished.
	@echo

test test_debug: LDFLAGS = -L./$(DBG_BUILD_DIR)/lib -lttui
test test_debug: debug $(TEST_EXE)
	@echo Target test finished.
	@echo
	$(TEST_EXE)

test_release: LDFLAGS = -L./$(REL_BUILD_DIR)/lib -lttui
test_release: release $(TEST_EXE)
	@echo Target test finished.
	@echo
	$(TEST_EXE)

# executable
$(TEST_EXE): $(TEST_TOT_OBJS)
	mkdir -p $(dir $@)
	$(CXX) -o $@ $(TEST_TOT_OBJS) $(TEST_CXXFLAGS) $(LDFLAGS)

# test objects
$(TEST_TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< $(TEST_CXXFLAGS) -MMD -MP

##########
# others #
##########

# clean
.PHONY: clean
clean:
	$(RM) $(TEST_LIB) $(REL_LIB) $(DBG_LIB)
	find . -type f \( -name *.o -o -name *.d \) -exec $(RM) {} +