CXX := clang++
CC := clang

# Directories
IDIR := include
SDIR := src
BUILDDIR := build
RELEASEDIR := build_release

# Flags
LDFLAGS := -lGL -lX11 -lglfw -lXrandr -lpthread -lXi -ldl
CDEBUGFLAGS := -ggdb -O0 -I$(IDIR)
CRELEASEFLAGS := -O2 -DNDEBUG -I$(IDIR)
CXXDEBUGFLAGS := $(CDEBUGFLAGS) -std=c++23
CXXRELEASEFLAGS := $(CRELEASEFLAGS) -std=c++23


# Source and object file handling
SRCS := $(wildcard $(SDIR)/*.c) $(wildcard $(SDIR)/*.cpp)
DEBUG_OBJS := $(patsubst $(SDIR)/%, $(BUILDDIR)/%, $(SRCS:.c=.o))
DEBUG_OBJS := $(patsubst $(SDIR)/%, $(BUILDDIR)/%, $(DEBUG_OBJS:.cpp=.o))
RELEASE_OBJS := $(patsubst $(SDIR)/%, $(RELEASEDIR)/%, $(SRCS:.c=.o))
RELEASE_OBJS := $(patsubst $(SDIR)/%, $(RELEASEDIR)/%, $(RELEASE_OBJS:.cpp=.o))
HEADERS := $(wildcard $(SDIR)/*.h)

# Target executable
TARGET := build/logl
RELEASE_TARGET := build_release/logl_release

# Debug build target
$(TARGET): $(DEBUG_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

# Release build target
.PHONY: release
release: $(RELEASE_TARGET)

$(RELEASE_TARGET): $(RELEASE_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

# Build object files for debug
$(BUILDDIR)/%.o: $(SDIR)/%.c 
	@mkdir -p $(BUILDDIR)
	$(CC) -c $(CDEBUGFLAGS) $< -o $@

$(BUILDDIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) -c $(CXXDEBUGFLAGS) $< -o $@

# Build object files for release
$(RELEASEDIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(RELEASEDIR)
	$(CC) -c $(CRELEASEFLAGS) $< -o $@

$(RELEASEDIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(RELEASEDIR)
	$(CXX) -c $(CXXRELEASEFLAGS) $< -o $@

.PHONY: run
run: $(TARGET)
	./$(TARGET)
# Generate .clangd configuration
CLANGDDIR := .clangd
.PHONY: clangd
clangd:
	@echo "Generating $(CLANGDDIR) configuration..."
	@echo "CompileFlags:" > $(CLANGDDIR)
	@echo "  Add: [" >> $(CLANGDDIR)
	@for flag in $(CXXDEBUGFLAGS); do echo "    \"$$flag\"," >> $(CLANGDDIR); done
	@for flag in $(LDFLAGS); do echo "    \"$$flag\"," >> $(CLANGDDIR); done
	@echo "  ]" >> $(CLANGDDIR)
	@echo "  Compiler: clang++" >> $(CLANGDDIR)
	@echo "Diagnostics:" >> $(CLANGDDIR)
	@echo "  UnusedIncludes: Strict" >> $(CLANGDDIR)
	@echo "Created $(CLANGDDIR) file with compilation flags."

# Clean target
.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(TARGET) $(RELEASEDIR) $(RELEASE_TARGET) $(CLANGDDIR)
