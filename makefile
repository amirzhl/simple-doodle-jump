# ===========================================================================
#  Simple Doodle Jump - makefile
# ---------------------------------------------------------------------------
#  Cross-platform build (Windows/git-bash, Linux, macOS). Recursively compiles
#  every .cpp under src/ into build/ and links against SFML.
#
#  Usage:
#    make          build the game
#    make run      build and run
#    make clean    remove build artefacts
#
#  Requirements: g++ with C++17 and SFML (graphics, window, audio, system)
#  installed.
#  If SFML lives in a custom location, override the paths, e.g.:
#    make SFML_INC=-IC:/SFML/include SFML_LIB=-LC:/SFML/lib
# ===========================================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Iinclude -Wall -Wextra -O2
LDFLAGS  :=

TARGET   := simple-doodle-jump
SRC_DIR  := src
BUILD_DIR:= build

# Optional user-provided SFML include/lib paths (empty by default; SFML is
# assumed to be on the default search path via a normal install / pkg config).
SFML_INC ?=
SFML_LIB ?=
# Link order matters on some toolchains: dependents before dependencies
# (graphics/window/audio all rely on system).
SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

CXXFLAGS += $(SFML_INC)
LDFLAGS  += $(SFML_LIB)

# Windows executables need the .exe suffix.
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
endif

# Recursively find every source file.
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(SFML_LIBS)

# Compile each translation unit, auto-creating the mirrored build sub-folder.
# -MMD -MP generates header dependency files so edits trigger recompiles.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)
