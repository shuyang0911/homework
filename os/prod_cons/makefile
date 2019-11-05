all:prod cons

prod: prod.o 
	g++ -o prod prod.o  -lpthread -lrt
cons: cons.o 
	g++ -o cons cons.o  -lpthread -lrt

prod.o:prod.c  
	g++ -c prod.c buffer.h -lpthread -lrt
cons.o:cons.c  
	g++ -c cons.c buffer.h -lpthread -lrt

clean:
	rm -rf *.o

