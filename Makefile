
EXE := test
OBJ_DIR := ./obj
RM := rm -rf
LD := g++
CC := g++
#CC_FLAG := -DUSE_STL_LIST -DUSE_STD_ALLOC -DUSE_STL_ALGO -DDEBUG
CC_FLAG := 
CC_FLAG += -g -Wall

$(EXE): $(OBJ_DIR)/main.o
	$(LD) -o $@ $<

$(OBJ_DIR)/main.o : main.cpp list.h memory_pool.h iterator_algo.h iterator_traits.h
	$(CC) $(CC_FLAG) -o $@ -c main.cpp

clean:
	$(RM) $(EXE) $(OBJ_DIR)/*.o
