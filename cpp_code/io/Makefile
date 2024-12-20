CXX = g++

SRCS = ascii.cpp corr.cpp crc.cpp analog.cpp
OBJDIR = build
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

LIBNAME = libi.a

TEST_SRCS = tests/tests.cpp
TEST_BINARY = $(OBJDIR)/io_tests

.PHONY: all clean test run_tests check

all: $(LIBNAME) check

$(LIBNAME): $(OBJS)
	ar rcs $@ $^

test: $(TEST_BINARY)

$(TEST_BINARY): $(LIBNAME) $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SRCS) $(LIBNAME)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

run_tests: test
	@echo "Running tests..."
	@./$(TEST_BINARY)

check: test
	@echo "Checking library via tests..."
	@./$(TEST_BINARY) || { echo "Tests failed!"; exit 1; }

clean:
	rm -rf $(OBJDIR) $(LIBNAME) \
	rm -f ./$(LIBNAME)
