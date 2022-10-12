CXX=g++
CPPFLAGS=-lstdc++ -std=c++17
WCHECK=-Wall -Wextra -Werror
TESTFLAG=-lgtest --coverage -fprofile-arcs -ftest-coverage
DEBUGFLAG=-ggdb3

OS = $(shell uname -s)

ifeq ($(OS), Linux)
	CPPFLAGS+= -pthread
endif

HEADERS=transactions.h \
		holder.h \
		containers/b_plus_tree.h \
		containers/hash_table.h \
		containers/self_balancing_binary_search_tree.h \
		storage.h

SOURCE=transactions.cpp \
			 holder.cpp \
       storage.cpp
	   
HASHTABLE=containers/hash_table.cpp
SELFBALANCING=containers/self_balancing_binary_search_tree.cpp
BPLUS=containers/b_plus_tree.cpp

ALLSOURCE=$(HASHTABLE) $(SELFBALANCING) $(BPLUS) $(SOURCE)
MAIN=main.cpp
TESTFILE=tests.cpp

.PHONY: all clean test leaks linter check test_out transactions

all: transactions

transactions:
	@make clean
	@$(CXX) $(CPPFLAGS) $(MAIN) $(ALLSOURCE) $(WCHECK) -o program.out
	@./program.out

hash_table.a:  hash_table.o
	@ar -crs $@ $^
	@ranlib $@

self_balancing_binary_search_tree.a:  self_balancing_binary_search_tree.o
	@ar -crs $@ $^
	@ranlib $@

b_plus_tree.a:  b_plus_tree.o
	@ar -crs $@ $^
	@ranlib $@

hash_table.o: $(HASHTABLE)
	@$(CXX) $(CPPFLAGS) $(SOURCE) -c $^

self_balancing_binary_search_tree.o: $(SELFBALANCING)
	@$(CXX) $(CPPFLAGS) $(SOURCE) -c $^

b_plus_tree.o: $(BPLUS)
	@$(CXX) $(CPPFLAGS) $(SOURCE) -c $^

tests: test_out
	@./test.out
	@make clean

test_out: clean
	@$(CXX) $(TESTFILE) $(ALLSOURCE) $(CPPFLAGS) $(DEBUGFLAG) $(TESTFLAG) $(WCHECK) -o test.out

clean:
	@rm -rf test.out *.gcno *.gcda *.dSYM *.cfg
	@rm -rf program.out *.o *.a sources/test_export.data

leaks: test_out
	@CK_FORK=no leaks --atExit -- ./test.out
	@make clean

linter:
	@cp ../materials/linters/CPPLINT.cfg .
	@python3 ../materials/linters/cpplint.py --extensions=cpp $(HEADERS) $(ALLSOURCE) $(TESTFILE) $(MAIN)
	@rm CPPLINT.cfg

check:
	@cppcheck --enable=all --suppress=missingIncludeSystem $(HEADERS) $(ALLSOURCE) $(TESTFILE) $(MAIN)
