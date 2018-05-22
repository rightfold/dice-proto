CXX=g++
CXXFLAGS=                                                                \
	-Wall -Wextra -Wpedantic -fmax-errors=1                              \
	-std=c++14                                                           \
	-D_GLIBCXX_DEBUG_PEDANTIC                                            \
	-isystem vendor/catch -isystem $(shell llvm-config --includedir)     \
	-fsanitize=address -fsanitize=leak -fsanitize=undefined

LD=g++
LDFLAGS=                                                                 \
	-L $(shell llvm-config --libdir)                                     \
	$(shell llvm-config --libs all)                                      \
	-fsanitize=address -fsanitize=leak -fsanitize=undefined

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
	LSAN_OPTIONS='max_leaks=1' ${BUILD}/bin/dicec-test

${BUILD}/bin/dicec-test: ${CXX_OBJECTS}
	mkdir -p $(dir $@)
	${LD} -o $@ $^ ${LDFLAGS}

${BUILD}/%.cxx.o: %.cxx ${CXX_HEADERS}
	mkdir -p $(dir $@)
	${CXX} -c -o $@ $< ${CXXFLAGS}
