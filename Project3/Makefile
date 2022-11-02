all: travelMonitorClient monitorServer

travelMonitorClient: travelMonitorClient.o  helpMonitorClient.o processList.o travelMonitorQueries.o bstlist.o bst.o SkipList.o HashTable.o BloomList.o StringList.o VirusTable.o Record.o BloomFilter.o parser.o LinkedList.o CountryList.o queries.o helperFunctions.o pipe_functions.o Threads.o
	gcc -g3 travelMonitorClient.o Threads.o helpMonitorClient.o  SkipList.o  processList.o travelMonitorQueries.o bstlist.o bst.o HashTable.o BloomList.o StringList.o VirusTable.o Record.o BloomFilter.o  parser.o  LinkedList.o CountryList.o queries.o helperFunctions.o pipe_functions.o -o travelMonitorClient 

monitorServer: monitorServer.o helpMonitorClient.o travelMonitorQueries.o  bstlist.o bst.o SkipList.o monitorFunctions.o processList.o BloomList.o HashTable.o StringList.o VirusTable.o Record.o BloomFilter.o parser.o LinkedList.o CountryList.o queries.o pipe_functions.o helperFunctions.o Threads.o
	gcc -g3 monitorServer.o Threads.o helpMonitorClient.o travelMonitorQueries.o  bstlist.o bst.o SkipList.o monitorFunctions.o processList.o BloomList.o HashTable.o StringList.o VirusTable.o Record.o BloomFilter.o  parser.o -lpthread  LinkedList.o CountryList.o queries.o pipe_functions.o helperFunctions.o -o monitorServer 

travelMonitorClient.o : travelMonitorClient.c
	gcc -c -g3 travelMonitorClient.c -lpthread

monitorServer.o : monitorServer.c
	gcc -c -g3 monitorServer.c -lpthread

travelMonitor.o : travelMonitor.c
	gcc -c -g3 travelMonitor.c

helpMonitorClient.o : helpMonitorClient.c
	gcc -c -g3 helpMonitorClient.c 

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

Threads.o : Threads.c
	gcc -c -g3  Threads.c -lpthread

travelMonitorQueries.o : travelMonitorQueries.c
	gcc -c -g3 travelMonitorQueries.c

clean:
	rm travelMonitorClient.o Threads.o monitorServer.o travelMonitorQueries.o helpMonitorClient.o bst.o bstlist.o StringList.o processList.o monitorFunctions.o BloomList.o helperFunctions.o SkipList.o HashTable.o VirusTable.o Record.o BloomFilter.o parser.o LinkedList.o CountryList.o queries.o pipe_functions.o travelMonitorClient monitorServer 
