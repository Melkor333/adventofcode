### If you wish to use extra libraries (math.h for instance),
### add their flags here (-lm in our case) in the "LIBS" variable.

LIBS = -lm

# All .c files except the test_* files
FILES = $(filter-out $(wildcard test_*),$(wildcard *.c))
SOLUTION_ARGS = input
SOLUTION2_ARGS = input

TEST_FILES = $(filter-out $(wildcard main.c),$(wildcard *.c))
TEST_HEADERS = $(wildcard *.h)
###
#CFLAGS  = -std=c99
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Werror
CFLAGS += -Wmissing-declarations

TESTFLAGS += -DUNITY_SUPPORT_64 -DUNITY_OUTPUT_COLOR -DMACRO=_GNU_SOURCE

ASANFLAGS  = -fsanitize=address
ASANFLAGS += -fno-common
ASANFLAGS += -fno-omit-frame-pointer

.PHONY: test
test: tests.out
	@./tests.out

.PHONY: memcheck
memcheck: ./*.c ./*.h
	@echo Compiling $@
	@$(CC) $(ASANFLAGS) $(CFLAGS) ../test-framework/unity.c ./*.c -o memcheck.out $(LIBS)
	@./memcheck.out
	@echo "Memory check passed"

.PHONY: clean
clean:
	rm -rf *.o *.out *.out.dSYM

tests.out: $(TEST_FILES) $(TEST_HEADERS)
	@echo Compiling $@
	@$(CC) $(CFLAGS) $(TESTFLAGS) ../test-framework/unity.c $(TEST_FILES) -o tests.out $(LIBS)

.PHONY: solve
solve:
	@true

.PHONY: solution solution2
solution: $(FILES) solve
	$(CC) $(CFLAGS) $(FILES) -o solution $(LIBS)
	@echo solving it:
	./$@ $(SOLUTION_ARGS) >> solutions
	@tail -1 solutions

solution2: $(FILES) solve
	$(CC) $(CFLAGS) $(FILES) -o solution $(LIBS)
	@echo solving it:
	./solution $(SOLUTION2_ARGS) >> solutions
	@tail -1 solutions
