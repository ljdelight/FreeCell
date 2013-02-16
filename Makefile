CC=g++
FLAGS=-O2 -Wall
PROJECT=FreeCell
FILES=src/main.cc src/Card.h src/FreeCell.h src/Tableau.h Makefile $(PROJECT)

$(PROJECT): main.o
	$(CC) $(FLAGS) main.o -o $@

main.o: src/main.cc
	$(CC) $(FLAGS) -c $^

tar: $(PROJECT)
	tar -cvzf $(PROJECT)_source.tar $(FILES)

clean:
	rm -f *~ *.o *.gch $(PROJECT)
