CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I include
TARGET   = bin/game

SRCDIR   = src
BUILDDIR = build

SRCS = $(SRCDIR)/Main.cpp $(SRCDIR)/Entity.cpp $(SRCDIR)/Tower.cpp \
       $(SRCDIR)/Shop.cpp $(SRCDIR)/Filesystem.cpp $(SRCDIR)/TUI.cpp \
       $(SRCDIR)/Story.cpp $(SRCDIR)/Config.cpp $(SRCDIR)/Inventory.cpp \
       $(SRCDIR)/Skills.cpp $(SRCDIR)/Quest.cpp $(SRCDIR)/Bestiary.cpp

OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
endif

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR) bin:
	mkdir -p $@ 2>/dev/null || mkdir $@

clean:
ifeq ($(OS),Windows_NT)
	powershell -NoProfile -Command "Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $(BUILDDIR),$(TARGET)"
else
	rm -rf $(BUILDDIR) $(TARGET)
endif

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
