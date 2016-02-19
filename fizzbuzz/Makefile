CC     = g++
CFLAGS = -Wall -pedantic -s -O2 -I../ttmath

.SUFFIXES: .cpp .o

.cpp.o:
	$(CC) -c $(CFLAGS) $<

all: fizzbuzz

fizzbuzz: fizzbuzz.o
	$(CC) -o fizzbuzz $(CFLAGS) fizzbuzz.o

fizzbuzz.o:		fizzbuzz.cpp

clean:
	rm -f *.o
	rm -f *.s
	rm -f fizzbuzz
# on MS Windows can automatically be added suffixes .exe to the names of output programs
	rm -f *.exe
