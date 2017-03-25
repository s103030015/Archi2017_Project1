TARGET=single_cycle

$(TARGET): main.o 
	gcc main.o  -o $(TARGET)
main.o: main.c define.h
	gcc -c main.c
clean: 
	rm -r $(TARGET) *.o *.bin *rpt *.exe *.out
