CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -std=c++14 -O2 -I vendor/catch

LD=g++
LDFLAGS=

BUILD=build

CXX_SOURCES=$(shell find src test -name '*.cxx')
CXX_HEADERS=$(shell find src test -name '*.hxx')
CXX_OBJECTS=$(patsubst %.cxx,${BUILD}/%.cxx.o,${CXX_SOURCES})

all: build

build: ${CXX_OBJECTS}

.PHONY: clean
clean:
	rm -rf ${BUILD}

.PHONY: test
test: ${BUILD}/bin/dicec-test
	${BUILD}/bin/dicec-test

${BUILD}/bin/dicec-test: ${CXX_OBJECTS}
	mkdir -p $(dir $@)
	${LD} -o $@ $^ ${LDFLAGS}

${BUILD}/%.cxx.o: %.cxx | ${CXX_HEADERS}
	mkdir -p $(dir $@)
	${CXX} -c -o $@ $< ${CXXFLAGS}
