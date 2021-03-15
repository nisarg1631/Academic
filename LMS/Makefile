CXX = g++
CXX_FLAGS = -std=c++03 -o library
ARGS = books/

all: run clean

compile:
	@$(CXX) library.cpp $(CXX_FLAGS)

run: compile
	@./library $(ARGS)

clean:
	@rm library