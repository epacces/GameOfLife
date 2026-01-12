SWIFT = swift
SWIFTC = swiftc
SWIFTFLAGS_COMMON = -warnings-as-errors

# Debug build flags (default)
SWIFTFLAGS_DEBUG = $(SWIFTFLAGS_COMMON) -g -Onone -DDEBUG

# Release build flags
SWIFTFLAGS_RELEASE = $(SWIFTFLAGS_COMMON) -O

# Default to debug
SWIFTFLAGS = $(SWIFTFLAGS_DEBUG)

.PHONY: all debug release run test clean

all: debug

# Debug builds (using Swift Package Manager)
debug:
	$(SWIFT) build

# Release builds (using Swift Package Manager)
release:
	$(SWIFT) build -c release

# Run the executable
run: debug
	$(SWIFT) run

run-release: release
	$(SWIFT) run -c release

# Run tests
test:
	$(SWIFT) test

# Standalone builds (single file, no SPM)
standalone-debug: SWIFTFLAGS = $(SWIFTFLAGS_DEBUG)
standalone-debug: game_of_life

standalone-release: SWIFTFLAGS = $(SWIFTFLAGS_RELEASE)
standalone-release: game_of_life

game_of_life: game.swift
	$(SWIFTC) $(SWIFTFLAGS) -o $@ $<

run-standalone: standalone-debug
	./game_of_life

clean:
	rm -rf .build game_of_life
