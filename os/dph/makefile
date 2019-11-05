all:dph

dph: dph.o 
	g++ -o dph dph.o  -lpthread -lrt

dph.o:dph.c  
	g++ -c dph.c -lpthread -lrt

clean:
	rm -rf *.o

