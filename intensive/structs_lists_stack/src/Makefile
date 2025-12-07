CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra
DOOR_STRUCT_SOURCES = dmanager_module.c
DOOR_STRUCT_OBJECTS=$(DOOR_STRUCT_SOURCES: .c=.o)
DOOR_STRUCT_EXECUTABLE=../build/Quest_1
LIST_SOURCES = list.c list_test.c
LIST_OBJECTS=$(LIST_SOURCES: .c=.o)
LIST_EXECUTABLE=../build/Quest_2
STACK_SOURCES = stack.c stack_test.c
STACK_OBJECTS=$(STACK_SOURCES: .c=.o)
STACK_EXECUTABLE=../build/Quest_3

all: door_struct list_test stack_test

door_struct: $(DOOR_STRUCT_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(DOOR_STRUCT_EXECUTABLE)

list_test: $(LIST_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(LIST_EXECUTABLE)

stack_test: $(STACK_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(STACK_EXECUTABLE)

clean: 
	rm -f *.o ../build/Quest_*