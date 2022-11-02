vaccineMonitor: vaccineMonitor.o SkipList.o HashTable.o	VirusTable.o Record.o BloomFilter.o parser.o LinkedList.o CountryList.o queries.o 
	gcc -g3 vaccineMonitor.o SkipList.o HashTable.o VirusTable.o Record.o BloomFilter.o  parser.o  LinkedList.o CountryList.o queries.o -o vaccineMonitor

vaccineMonitor.o : vaccineMonitor.c
	gcc -c -g3 vaccineMonitor.c

SkipList.o : SkipList.c
	gcc -c -g3 SkipList.c

HashTable.o : HashTable.c
	gcc -c -g3 HashTable.c

VirusTable.o : VirusTable.c
	gcc -c -g3 VirusTable.c

Record.o : Record.c
	gcc -c -g3 Record.c
	
BloomFilter.o : BloomFilter.c
	gcc -c -g3 BloomFilter.c
		
parser.o : parser.c
	gcc -c -g3 parser.c

LinkedList.o : LinkedList.c
	gcc -c -g3 LinkedList.c

CountryList.o : CountryList.c
	gcc -c -g3 CountryList.c

queries.o : queries.c
	gcc -c -g3 queries.c

clean:
	rm vaccineMonitor.o SkipList.o HashTable.o VirusTable.o Record.o BloomFilter.o parser.o LinkedList.o CountryList.o queries.o vaccineMonitor