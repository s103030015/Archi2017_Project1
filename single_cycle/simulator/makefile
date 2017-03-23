all: $(PROGRAMS)
halt.o: halt.c
  $(CC) $(CFLAGS) -c halt.c
halt.o: halt.o start.o
  $(LD) $(LDFLAGS) start.o halt.o -o halt.coff
  $(COFF2NOFF) halt.coff halt
add.o: add.c
  $(CC) $(CFLAGS) -c add.c
add.o: add.o start.o
  $(LD) $(LDFLAGS) start.o add.o -o add.coff
  $(COFF2NOFF) add.coff add
 
