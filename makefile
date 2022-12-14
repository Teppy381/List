CXX_FLAGS = -Wall -Wextra -fsanitize=address -g -ggdb3

all: List

List: main.o functions.o
	g++ main.o functions.o $(CXX_FLAGS) -o List.out

main.o: main.cpp
	g++ -c -o main.o main.cpp $(CXX_FLAGS)

functions.o: functions.cpp
	g++ -c -o functions.o functions.cpp	$(CXX_FLAGS)
