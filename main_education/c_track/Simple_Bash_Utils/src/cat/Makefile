CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -D_DEFAULT_SOURCE 

CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -std=c++17 

all: s21_cat test_s21_cat style info

s21_cat: s21_cat_info cat.o 
	$(CC) $(CFLAGS) cat.o -o s21_cat

s21_cat_info:
	$(info                                         )
	$(info ----------------------------------------)
	$(info Build a s21_cat                         ) 
	$(info ----------------------------------------)

.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@
#	$(CC) $(CFLAGS) -g -DDEBUG -c $< -o $@

test_s21_cat: tests/test_cat.cpp
	$(info                                         )
	$(info ----------------------------------------)
	$(info Build tests for s21_cat                 ) 
	$(info ----------------------------------------)
	$(CXX) $(CXXFLAGS) tests/test_cat.cpp -o tests/test_s21_cat

test:
	cp tests/test_s21_cat . ; ./test_s21_cat | tee log.txt; rm test_s21_cat;

clean:
	rm -rf *.o s21_cat tests/test_s21_cat 
	rm -rf log.txt input1.txt input2.txt

rebuild: clean all

style:
	$(info                                         )
	$(info ----------------------------------------)
	$(info Check for code style and formatting     )
	$(info ----------------------------------------)
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem tests/*.cpp
	clang-format -i *.c
	clang-format -i *.h
	clang-format -i tests/*.cpp
	cp ../../materials/linters/CPPLINT.cfg .
	python3 ../../materials/linters/cpplint.py --extensions=c *.c
	python3 ../../materials/linters/cpplint.py --extensions=c *.h
	python3 ../../materials/linters/cpplint.py --extensions=cpp tests/*.cpp

info: 
	$(info                                         )
	$(info ----------------------------------------)
	$(info All right! To run tests type 'make test')
	$(info Check out all the results in 'log.txt'  )
	$(info ----------------------------------------)
