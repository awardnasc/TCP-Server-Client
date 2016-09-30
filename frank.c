#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char *argv[]) {

  /*if (argc < 3 || argc > 4) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)",
        "<Server Address> <Echo Word> [<Server Port>]");
*/
  char *servIP;     // First arg: server IP address (dotted quad)
  char *echoString; // Second arg: string to echo

  // Third arg (optional): server port (numeric).  7 is well-known echo port
  in_port_t servPort;

  // Parse command line arguments and initialize variables
        int c;
	opterr = 0;
	while ((c = getopt(argc, argv, "s:p:m:")) != -1) {
		switch (c) {
			case 's':
				servIP = optarg;
				break;
			case 'p':
				servPort = atoi(optarg);
				break;
			case 'm':
				echoString = optarg;
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

  // Create a reliable, stream socket using TCP
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
    DieWithSystemMessage("socket() failed");

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET;          // IPv4 address family
  

  // Convert address
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");
  servAddr.sin_port = htons(servPort);    // Server port

  // Establish the connection to the echo server
  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("connect() failed");

  size_t echoStringLen = strlen(echoString); // Determine input length


  // Declare new variables for sending/receiving and clock time
  _Bool send_success = false, recv_success = false;
  int attempts = 0;
  clock_t begin,end;
  begin = clock();

  // Send the string to the server
  ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
  if (numBytes < 0)
    DieWithSystemMessage("send() failed");
  else if (numBytes != echoStringLen)
    DieWithUserMessage("send()", "sent unexpected number of bytes");

  // Receive the same string back from the server
  unsigned int totalBytesRcvd = 0; // Count of total bytes received
  

  struct sockaddr_storage fromAddr; // Source address of server
	socklen_t fromAddrLen = sizeof(fromAddr);
	char inverted[MAXSTRINGLENGTH + 1];
	while (!recv_success) {
		numBytes = recv(sock, inverted, BUFSIZE - 1, 0);
	        attempts++;

		if (numBytes == echoStringLen) {
			end = clock();
			recv_success = true;
		}
		else if (numBytes != echoStringLen) {
			printf("recvfrom() error: received unexpected number of bytes; ");
			printf("attempting to receive again...\n");
		}
		//else if (!SockAddrsEqual(servAddr->ai_addr, (struct sockaddr *)&fromAddr))
		//	printf("recvfrom() error: received a packet from unknown source");
		else
			printf("recvfrom() failed; attempting to receive again...\n");
  }



  // Send inverted string back to caseInverter
	size_t invertedLen = strlen(inverted);
	send_success = false;
	while (!send_success) {
		attempts++;
		numBytes = send(sock, inverted, invertedLen, 0);
		//numBytes = sendto(sock, inverted, invertedLen, 0,
		//				servAddr->ai_addr, servAddr->ai_addrlen);
		if (numBytes == invertedLen)
			send_success = true;
		else if (numBytes != invertedLen) {
			printf("sendto() error: sent unexpected number of bytes; ");
			printf("sending message again...\n");
		}
		else
			printf("sendto() failed; sending message again...\n");
	}

   // Receive final re-inverted string back from caseInverter
	char final[MAXSTRINGLENGTH + 1];
	recv_success = false;
	while (!recv_success) {
                numBytes = recv(sock, final, BUFSIZE - 1, 0);
		if (numBytes == invertedLen) 
			recv_success = true;
		else if (numBytes != invertedLen) {
			printf("recvfrom() error: received unexpected number of bytes; ");
			printf("attempting to receive again...\n");
		}
		else
			printf("recvfrom() failed; attempting to receive again...\n");
	}

  // Verify that initial msg and final doubly-inverted msg are identical
  inverted[echoStringLen] = '\0';
  final[echoStringLen] = '\0';
  
  _Bool verified = !strcmp(echoString, final);



  end = clock();
  double time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC;
  printf(" %d	%.6f	%s	%s	%s", attempts, 
			time_spent, echoString, inverted,
			 verified ? "Verified" : "Not Verified");

  fputc('\n', stdout); // Print a final linefeed

  close(sock);
  exit(0);
}

