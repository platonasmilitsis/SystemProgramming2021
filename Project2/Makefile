all: travelMonitor Monitor

travelMonitor: travelMonitor.o travelMonitorQueries.o processList.o bstlist.o bst.o SkipList.o HashTable.o BloomList.o StringList.o VirusTable.o Record.o BloomFilter.o parser.o LinkedList.o CountryList.o queries.o helperFunctions.o pipe_functions.o
	gcc -g3 travelMonitor.o SkipList.o travelMonitorQueries.o processList.o bstlist.o bst.o HashTable.o BloomList.o StringList.o VirusTable.o Record.o BloomFilter.o  parser.o  LinkedList.o CountryList.o queries.o helperFunctions.o pipe_functions.o -o travelMonitor

Monitor: Monitor.o travelMonitorQueries.o  bstlist.o bst.o SkipList.o monitorFunctions.o processList.o BloomList.o HashTable.o StringList.o VirusTable.o Record.o BloomFilter.o parser.o LinkedList.o CountryList.o queries.o pipe_functions.o
	gcc -g3 Monitor.o travelMonitorQueries.o  bstlist.o bst.o SkipList.o monitorFunctions.o processList.o BloomList.o HashTable.o StringList.o VirusTable.o Record.o BloomFilter.o  parser.o  LinkedList.o CountryList.o queries.o pipe_functions.o -o Monitor

travelMonitor.o : travelMonitor.c
	gcc -c -g3 travelMonitor.c

Monitor.o : Monitor.c
	gcc -c -g3 Monitor.c

pipe_functions.o : pipe_functions.c
	gcc -c -g3 pipe_functions.c

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

helperFunctions.o : helperFunctions.c
	gcc -c -g3 helperFunctions.c

StringList.o : StringList.c
	gcc -c -g3 StringList.c

BloomList.o : BloomList.c
	gcc -c -g3 BloomList.c

processList.o : processList.c
	gcc -c -g3 processList.c

bst.o : bst.c
	gcc -c -g3 bst.c

bstlist.o : bstlist.c
	gcc -c -g3 bstlist.c

monitorFunctions.o : monitorFunctions.c
	gcc -c -g3 monitorFunctions.c

travelMonitorQueries.o : travelMonitorQueries.c
	gcc -c -g3 travelMonitorQueries.c


clean:
	rm travelMonitor.o travelMonitorQueries.o Monitor.o  bst.o bstlist.o StringList.o processList.o monitorFunctions.o BloomList.o helperFunctions.o SkipList.o HashTable.o VirusTable.o Record.o BloomFilter.o parser.o LinkedList.o CountryList.o queries.o pipe_functions.o travelMonitor Monitor nakos.*
