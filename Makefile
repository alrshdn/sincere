CC=cc

LIBS=-lrt

write: sincere.c sincere.h write.c
	$(CC) sincere.c write.c -o $@ $(LIBS)

read: sincere.c sincere.h read.c
	$(CC) sincere.c read.c -o $@ $(LIBS)
