CXXFLAGS = \
  $(WARNFLAGS) \
  $(OPTFLAGS) \
  $(INCLUDES)

WARNFLAGS = \
  -std=c++20 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion

OPTFLAGS = \
  -O2

INCLUDES = \
  -isystem ../../submodules/github.com/snsinfu/h5/include

LDFLAGS = \
  -lhdf5

PRODUCT = \
  main


.PHONY: all clean depends

all: $(PRODUCT)
	@:

clean:
	rm -f $(ARTIFACTS)

-include _overrides.mk
