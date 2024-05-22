CXX = ccache distcc c++
CXXFLAGS = -Wall -Wextra -pedantic -ggdb -std=c++20 -fpartial-inlining -o
OBJECT = bin/beam
SRC = $(shell find src/ -type f -name *.cpp)
INSTALLATION_TARGET = /usr/bin/

export CCACHE_DIR = .cache
export CCACHE_MAXSIZE = 20G
export CCACHE_COMPRESSLEVEL=9
export CCACHE_COMPRESSOR="zstd -15"
export CCACHE_NOHASHDIR=1

default: $(SRC)
	@mkdir -p bin
	@$(CXX) $(CXXFLAGS) $(OBJECT) $(SRC)

clean: $(OBJECT)
	@rm $(OBJECT)

install: $(INSTALLATION_TARGET)
	@make default
	@sudo mv $(OBJECT) $(INSTALLATION_TARGET)

uninstall: $(INSTALLATION_TARGET)
	@sudo rm $(INSTALLATION_TARGET)
