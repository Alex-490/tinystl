CC = g++
CFLAGS = -g -Wall -MMD -MP
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp,build/%.o,$(SRC))
TARGET = main

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

build/%.o:%.cpp | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

.PHONY:clean all rebuild
clean:
	rm -rf build

rebuild:clean all

-include $(OBJ:%.o=%.d)