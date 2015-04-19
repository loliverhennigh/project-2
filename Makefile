all: test graphics


# bad part here
graphics: lsms.c lsms_graphics.cpp spring.c particle.c force.c
	g++ $(CLAGS) lsms_graphics.cpp spring.c particle.c lsms.c force.c -o graphics -lGL -lm -lglut

test: test.o spring.o particle.o lsms.o force.o
	gcc $(CFLAGS) test.o spring.o particle.o lsms.o force.o -o test -lm

test.o: test.c
	gcc $(CFLAGS) -c test.c 

spring.o: spring.c
	gcc $(CFLAGS) -c spring.c 

particle.o: particle.c
	gcc $(CFLAGS) -c particle.c 

lsms.o: lsms.c
	gcc $(CFLAGS) -c lsms.c

force.o: force.c
	gcc $(CFLAGS) -c force.c


clean:
	rm -f *.o test graphics
