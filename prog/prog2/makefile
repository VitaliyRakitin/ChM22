CC=g++
CFLAGS=-c -Wall -O3 -Werror -g
LDFLAGS=
SOURCES=main.cpp matrix.cpp matrix_det.cpp matrix_operator.cpp matrix_back.cpp Newton.cpp Euler.cpp functions.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=p

all: $(SOURCES) $(EXECUTABLE)
	rm -rf *.o 	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf *.o 