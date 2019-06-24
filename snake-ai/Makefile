
CC = g++
CFLAGS = -g -std=c++11 
OBJS =  window.o snake-ai.o snake-view.o source.o
INCLUDES = -I/home/smathews/dlib/
LDFLAGS =  -lcurses -lpthread -ldl -lpng -ljpeg -lblas -llapack -lX11 -lXrender -lXext -lXfixes -lfltk  -ldlib    -L/home/smathews/dlib/examples/build/dlib_build/

snake-ai: ${OBJS}
	${CC} ${CFLAGS} -o snake-ai ${OBJS} ${LDFLAGS}
	
window.o: window.cpp snake-ai.h snake-view.h
	${CC} ${CFLAGS} ${INCLUDES} -o window.o -c window.cpp

snake-ai.o: snake-ai.cpp snake-ai.h
	${CC} ${CFLAGS} ${INCLUDES} -o snake-ai.o -c snake-ai.cpp

snake-view.o: snake-ai.h snake-view.cpp snake-view.h
	${CC} ${CFLAGS} ${INCLUDES} -o snake-view.o -c snake-view.cpp


source.o: /home/smathews/dlib/dlib/all/source.cpp
	${CC} ${CFLAGS} ${INCLUDES} -o source.o -c $?


test: test.o snake-ai.o source.o
	${CC} ${CFLAGS} -o test test.o snake-ai.o source.o ${LDFLAGS}
	
test.o: test.cpp snake-ai.h 
	${CC} ${CFLAGS} ${INCLUDES} -o test.o -c test.cpp

clean:
	rm window.o snake-ai.o snake-view.o snake-ai
	
cleanall:
	rm source.o window.o snake-ai.o snake-view.o snake-ai
	
all:
	make clean; make
