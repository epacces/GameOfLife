RUBY = ruby

# Debug flags (warnings and debug mode)
RUBYFLAGS_DEBUG = -w -d

# Release flags (no extra output)
RUBYFLAGS_RELEASE =

# Default to release (standard Ruby behavior)
RUBYFLAGS = $(RUBYFLAGS_RELEASE)

.PHONY: all run run-debug test test-verbose lint clean

all: run

# Run the game
run:
	$(RUBY) $(RUBYFLAGS_RELEASE) game.rb

# Run with debug output and warnings
run-debug:
	$(RUBY) $(RUBYFLAGS_DEBUG) game.rb

# Run tests with RSpec
test:
	rspec

# Run tests with detailed output
test-verbose:
	rspec --format doc

# Syntax check
lint:
	$(RUBY) -c game.rb
	$(RUBY) -c spec/*.rb

# Check for Gemfile and install dependencies
setup:
	@if [ -f Gemfile ]; then bundle install; else echo "No Gemfile found"; fi

clean:
	rm -rf .rspec_status coverage/
