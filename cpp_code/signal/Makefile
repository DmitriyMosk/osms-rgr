CXX = g++

SRCDIR = src
OBJDIR = build

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

LIBNAME = libsignal.a

.PHONY: all clean

all: $(LIBNAME)

$(LIBNAME): $(OBJS) ./../io/libi.a
	ar rcs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(LIBNAME) \
	rm -f ./$(LIBNAME)
