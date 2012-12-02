CC = g++
CFLAGS = -std=c++0x
OBJS = iconvpp.o
LIBS = -liconv
OUTPUT = libiconvpp.so

all:
	@echo iconvpp makefile

clean:
	rm -rf *.o *.so

iconvpp: $(OBJS)
	$(CC) -shared -o $(OUTPUT) $(OBJS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $<

iconvpp.o: iconvpp.h

