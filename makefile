COMP = g++-5.3.0
OBJS = jigsawPuzzle.o puzzleElement.o main.o
EXEC = ex1
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
puzzleElement.o: puzzleElement.h puzzleElement.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
jigsawPuzzle.o: jigsawPuzzle.h jigsawPuzzle.cpp puzzleElement.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
main.o: main.cpp puzzleElement.h jigsawPuzzle.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)