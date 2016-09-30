#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int main(int argc, char *argv[]) {

  //Array to hold unique IPs
	char *ipAddrs[MAXSTRINGLENGTH];
	int ipCounter = 0;

  if (argc != 3) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)", "<Server Port>");

  // Parse command line and initialize variables
	char *portnum;
	int c;
	//opterr = 0;
	while ((c = getopt(argc, argv, "p:")) != -1) {
		switch (c) {
			case 'p':
				portnum = optarg;
				break;
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c needs an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
			default:
				abort ();
		}
	}

  in_port_t servPort = atoi(portnum);
  
  // Create socket for incoming connections
  int servSock; // Socket descriptor for server
  if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithSystemMessage("socket() failed");

  // Construct local address structure
  struct sockaddr_in servAddr;                  // Local address
  memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
  servAddr.sin_family = AF_INET;                // IPv4 address family
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
  servAddr.sin_port = htons(servPort);          // Local port

  // Bind to the local address
  if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("bind() failed");

  // Mark the socket so it will listen for incoming connections
  if (listen(servSock, MAXPENDING) < 0)
    DieWithSystemMessage("listen() failed");
  
  // Code necessary for printing output when ended with CTRL+C
	int msgs_recvd = 0;
  
  //Print number of messages and the unique client ips after CTRL+C
	void ctrlHandle(int sig) {
		char c;
		signal(sig, SIG_IGN);
		printf("\n%d	", msgs_recvd-1);
		int y;		
		for(y = 0; y < ipCounter; y++){
			printf("%s, ",ipAddrs[y]);
		}
		printf("\n");
		exit(1);
	}
	signal(SIGINT, ctrlHandle);


  for (;;) { // Run forever
    msgs_recvd++;
    int unique = 0;
    int y;

    struct sockaddr_in clntAddr; // Client address
    // Set length of client address structure (in-out parameter)
    socklen_t clntAddrLen = sizeof(clntAddr);

    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
    if (clntSock < 0)
      DieWithSystemMessage("accept() failed");

    // clntSock is connected to a client!

    char clntName[INET_ADDRSTRLEN]; // String to contain client address
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
        sizeof(clntName)) != NULL)
      printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
    else
      puts("Unable to get client address");
    
    //If it is the first clients connected save ip
		if (ipCounter == 0){			
			ipAddrs[ipCounter] = clntName;
			ipCounter++;
		}
		
   //Check if client ip is unique. If unique save in ipAddrs
   else {
	   for(y = 0; y < ipCounter; y++){
	   	if (strcmp(ipAddrs[y], clntName) == 0){
			unique = 1;
	   	}
	   	else
			unique = 0;
	   	}
	   if(unique == 0){
		ipAddrs[ipCounter] = clntName;
		ipCounter++;
	   }
	}
	
    
    char buffer[BUFSIZE]; // Buffer for echo string
    char inverted[BUFSIZE];
  

  // Receive message from client
  ssize_t numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);
  if (numBytesRcvd < 0)
    DieWithSystemMessage("recv() failed");
  
  //Invert message case 
  int x;
  for (x=0; x<clntAddrLen; x++) {
  if ((int)buffer[x] >= 65 && (int)buffer[x] <= 90)
	inverted[x] = buffer[x]+32;
	else if ((int)buffer[x] >= 97 && (int)buffer[x] <= 122)
	inverted[x] = buffer[x]-32;
	else
	inverted[x] = buffer[x];
		}
  // Send received string and receive again until end of stream
  while (numBytesRcvd > 0) { // 0 indicates end of stream
    // Echo message back to client
    ssize_t numBytesSent = send(clntSock, inverted, numBytesRcvd, 0);
    if (numBytesSent < 0)
      DieWithSystemMessage("send() failed");
    else if (numBytesSent != numBytesRcvd)
      DieWithUserMessage("send()", "sent unexpected number of bytes");

    // See if there is more data to receive
    numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);
    if (numBytesRcvd < 0)
      DieWithSystemMessage("recv() failed");
  }
  }
  // NOT REACHED
}

