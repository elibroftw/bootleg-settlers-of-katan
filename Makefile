CXX=g++
CXXFLAGS=-std=c++14 -Wall -O -g -MMD -Werror=vla # use -MMD to generate dependencies
SOURCES=$(wildcard *.cc)   # list of all .cc files in the current directory
OBJECTS=${SOURCES:.cc=.o}  # .o files depend upon .cc files with same names
DEPENDS=${OBJECTS:.o=.d}   # .d file is list of dependencies for corresponding .cc file
EXEC=constructor
# for creating a test harness executable to test the modules
TEST_SOURCES=test_harness.cc textdisplay.cc
TEST_OBJECTS=${TEST_SOURCES:.cc=.o}
TEST_DEPENDS=${TEST_OBJECTS:.o=.d}
TEST_EXEC=test_harness

ifeq ($(OS),Windows_NT)
    RUN_EXE := ${TEST_EXEC}
else
    RUN_EXE := ./$(TEST_EXEC)
endif

# First target in the makefile is the default target.
# Note that the LIBFLAGS must come last in the command
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)

${TEST_EXEC} test: ${TEST_OBJECTS}
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $(TEST_EXEC) && ${RUN_EXE}


%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

-include ${DEPENDS}

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DEPENDS) $(EXEC)
