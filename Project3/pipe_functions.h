#ifndef PIPEFUNUCTIONS
#define PIPEFUNUCTIONS

//Function to write a message into the pipe using a selected protocol
void write_to_pipe(int fd,char *msg,int bufferSize);

//Function to read a message from the pipe according to the protocol
char *read_from_pipe(int fd,int bufferSize);

#endif //PIPEFUNCTIONS