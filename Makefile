#  CITS2002 Project 2 2021
#  Name(s):            Hidden for Privacy.
#  Student number(s):  

# A Makefile to build our 'duplicates' project

PROJECT = duplicates
HEADERS = $(PROJECT).h
OBJ = duplicates.o hash.o support.o directory.o

C11    =  cc -std=c11
CFLAGS =  -Wall -Werror

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ) 

duplicates.o : duplicates.c $(HEADERS)
	$(C11) $(CFLAGS) -c duplicates.c

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)
