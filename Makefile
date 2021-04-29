CXX=g++
SOURCES=$(filter-out test_harness.cc, $(wildcard *.cc))
OBJECTS=${SOURCES:.cc=.o}
DEPENDS=${OBJECTS:.o=.d}
EXEC=constructor
# for creating a test harness executable to test the modules
TEST_SOURCES=$(filter-out main.cc, $(wildcard *.cc))
TEST_OBJECTS=${TEST_SOURCES:.cc=.o}
TEST_DEPENDS=${TEST_OBJECTS:.o=.d}
TEST_EXEC=tconstructor

ifeq ($(OS),Windows_NT)
	EXE := ${EXEC}.exe
    TEST_EXE := ${TEST_EXEC}.exe
	CXXFLAGS=-Wall -lstdc++ -Os -MMD -Werror=vla -static
else
	EXE := ./${EXEC}
    TEST_EXE := ./$(TEST_EXEC)
	CXXFLAGS=-Wall -lstdc++ -Os -MMD -Werror=vla
endif

# Note that the LIBFLAGS must come last in the command
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXE)

${TEST_EXEC}: ${TEST_OBJECTS}
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $(TEST_EXE)

all: ${EXEC} ${TEST_EXEC}

%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

-include ${DEPENDS}

.PHONY: clean test vg zip

clean:
	rm -f $(wildcard *.o) $(wildcard *.d) $(EXE) ${TEST_EXE}

test: all
	${TEST_EXE}

zip:
	rm -f *.zip && zip constructor.zip Makefile *.cc *.h

vg: all  # valgrind
	valgrind --leak-check=full -v --track-origins=yes --log-file=vg_test.log ${TEST_EXE}
	valgrind --leak-check=full -v --track-origins=yes --log-file=vg.log ${EXE}
