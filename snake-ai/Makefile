
CC = g++
CFLAGS = -g -std=c++11 -pthread
OBJS =  main.o snake-ai.o snake-pop.o source.o
INCLUDES = -I/home/smathews/dlib/
LDFLAGS =  -lcurses -lpthread -ldl -lpng -ljpeg -lblas -llapack -lX11 -lXrender -lXext -lXfixes -lfltk  -ldlib    -L/home/smathews/dlib/examples/build/dlib_build/

snake-ai: ${OBJS}
	${CC} ${CFLAGS} -o snake-ai ${OBJS} ${LDFLAGS}
	
main.o: main.cpp snake-ai.h
	${CC} ${CFLAGS} ${INCLUDES} -o main.o -c main.cpp

snake-ai.o: snake-ai.cpp snake-ai.h
	${CC} ${CFLAGS} ${INCLUDES} -o snake-ai.o -c snake-ai.cpp

snake-pop.o: snake-pop.cpp snake-pop.h
	${CC} ${CFLAGS} ${INCLUDES} -o snake-pop.o -c snake-pop.cpp

source.o: /home/smathews/dlib/dlib/all/source.cpp
	${CC} ${CFLAGS} ${INCLUDES} -o source.o -c $?


test: test.o source.o
	${CC} ${CFLAGS} -o test test.o source.o ${LDFLAGS}
	
test.o: test.cpp snake-ai.h 
	${CC} ${CFLAGS} ${INCLUDES} -o test.o -c test.cpp

clean:
	rm *.o snake-ai
	
all:
	make clean; make
