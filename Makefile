
# Tests
TEST_DIR := test
TEST_SRCS=$(wildcard $(TEST_DIR)/*.cpp)
TEST_EXE=$(TEST_SRCS:.cpp=)
TEST_DEPS=$(TEST_SRCS:.cpp=.d)


LIB := libsimpletrace.a
# Source files
SRCS += $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS += $(SRCS:.cpp=.d)

#So that we have std::span. You can override this with higher C++ if you like.
CXXFLAGS ?= --std=c++20

LDFLAGS += -L.
CXXFLAGS += -I./third_party


-include $(TEST_DEPS)
-include $(DEPS)

all : $(TEST_EXE) 


test : $(TEST_EXE)
	./test/testall.sh

$(LIB): $(OBJS)
	ar rcs $@ $^

src/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@


$(TEST_DIR)/% : $(TEST_DIR)/%.cpp $(LIB)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -MMD $< -o $@ -lsimpletrace $(LDFLAGS)


format :
	clang-format -i ./src/*.h
	clang-format -i ./src/*.cpp
	clang-format -i ./test/*.cpp




.PHONY : clean format $(TEST_DEPS) test

clean :
	rm -rf $(TEST_EXE) $(TEST_DEPS) $(OBJS) $(DEPS)
	rm -rf $(LIB)
