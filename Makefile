CC=cc 
LIBS=-lrt
CFLAGS=-std=c99 -Wall -Wextra -pedantic -D_XOPEN_SOURCE=500

TESTS=write read

all: ${TESTS}

${TESTS}: sincere.c sincere.h write.c
	${CC} ${CFLAGS} sincere.c $@.c -o $@ ${LIBS}

clean:
	rm -f ${TESTS}
