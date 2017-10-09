# Makefile

CFLAGS = -std=gnu99 -O2 -pthread


.PHONY: clean


all: ep2

#ep2: ep2.o
#	$(CC) -o $@ $^ -pthread
#	rm ep2.o
	
%: %.o
	$(CC) -o $@ $^ 
	rm $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f *.o *~ ep2
	
# para criar o .tar.gz
pacotinho:
	mkdir pacote
	cp ep2.c pacote/
	cp Makefile pacote/	
	tar -czvf ep2.tar.gz pacote/
	rm -rf pacote/