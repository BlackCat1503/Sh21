CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -D_DEFAULT_SOURCE 

CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -std=c++17 

all: s21_grep test_s21_grep style info

s21_grep: s21_grep_info string_list.o cmd_line_args.o grep.o 
	$(CC) $(CFLAGS) string_list.o cmd_line_args.o grep.o -o s21_grep 

s21_grep_info:
	$(info                                         )
	$(info ----------------------------------------)
	$(info Build a s21_grep                        ) 
	$(info ----------------------------------------)

.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@
#	$(CC) $(CFLAGS) -g -DDEBUG -c $< -o $@

test_s21_grep: tests/test_grep.cpp
	$(info                                         )
	$(info ----------------------------------------)
	$(info Build tests for s21_grep                ) 
	$(info ----------------------------------------)
	$(CXX) $(CXXFLAGS) tests/test_grep.cpp -o tests/test_s21_grep 

test:
	cp ./tests/test_s21_grep .; ./test_s21_grep | tee log.txt; rm test_s21_grep

clean:
	rm -rf *.o s21_grep tests/test_s21_grep 
	rm -rf log.txt input1.txt input2.txt patterns1.txt patterns2.txt
	rm -rf tests/errors

rebuild: clean all

style:
	$(info                                         )
	$(info ----------------------------------------)
	$(info Check code style and formatting         )
	$(info ----------------------------------------)
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	clang-format -i *.c
	clang-format -i *.h
	clang-format -i tests/*.cpp
	cp ../../materials/linters/CPPLINT.cfg .
	python3 ../../materials/linters/cpplint.py --extensions=c *.c
	python3 ../../materials/linters/cpplint.py --extensions=c *.h
	python3 ../../materials/linters/cpplint.py --extensions=cpp tests/*.cpp

info: 
	$(info                                               )
	$(info ----------------------------------------------)
	$(info All right!                                    )
	$(info To run tests take you tea and type 'make test')
	$(info Check out all the results in file 'log.txt'   ) 
	$(info ----------------------------------------------)
