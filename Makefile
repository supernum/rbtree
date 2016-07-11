
CC = gcc -std=c99 -m64 -g -O0 -Wall 
TESTNAME = tree


INC = -I./
OBJ = rbtree.o
OBJ += main.o

$(TESTNAME): $(OBJ)
	$(CC) -o $@ $^
    
%.o : %.c 
	$(CC) -c $< $(INC)

.PHONY:clean
clean:
	rm -f $(OBJ) $(TESTNAME)


