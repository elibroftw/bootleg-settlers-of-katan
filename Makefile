CXX=g++
CXXFLAGS=-std=c++14 -Wall -O -g -MMD -Werror=vla
SOURCES=$(filter-out test_harness.cc, $(wildcard *.cc))
OBJECTS=${SOURCES:.cc=.o}
DEPENDS=${OBJECTS:.o=.d}
EXEC=constructor
# for creating a test harness executable to test the modules
TEST_SOURCES=$(filter-out main.cc, $(wildcard *.cc))
TEST_OBJECTS=${TEST_SOURCES:.cc=.o}
TEST_DEPENDS=${TEST_OBJECTS:.o=.d}
TEST_EXEC=test_harness

ifeq ($(OS),Windows_NT)
	RUN_EXE := ${EXEC}.exe
    TEST_EXE := ${TEST_EXEC}.exe
else
	RUN_EXE := ./${EXEC}
    TEST_EXE := ./$(TEST_EXEC)

endif

# First target in the makefile is the default target.
# Note that the LIBFLAGS must come last in the command
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)

${TEST_EXEC} test: ${TEST_OBJECTS} run_test
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $(TEST_EXEC)

run_test:
	${TEST_EXE}

%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

-include ${DEPENDS}

.PHONY: clean
clean:
	rm -f $(wildcard *.o) $(wildcard *.d) $(RUN_EXE) ${TEST_EXE}
