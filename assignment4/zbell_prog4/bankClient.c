#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "banking.h"


sBANK_PROTOCOL createTransaction(char type, int acctNum, int value) {

    int transType = -1;

    switch (type) {
        case 'B':
            transType = BANK_TRANS_INQUIRY;
            value = 0;
            break;
        case 'D':
            transType = BANK_TRANS_DEPOSIT;
            break;
        case 'W':
            transType = BANK_TRANS_WITHDRAW;
            break;
    }

    sBANK_PROTOCOL transaction = { transType, acctNum, value };

    return transaction;
}

sBANK_PROTOCOL* sendTransaction(int sock, sBANK_PROTOCOL transaction) { 
    
    write(sock, &transaction, sizeof(transaction));

    char* buffer = malloc(sizeof(transaction));
    read(sock, buffer, sizeof(transaction));

    return (sBANK_PROTOCOL*) buffer;
} 

int setupTCPClient(char *servIPAddr, unsigned int portNum)
{
    int clientSocket;
    struct sockaddr_in servAddr;

    /* Setup address of server */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIPAddr);
    servAddr.sin_port = htons(portNum);

    /* Create socket */
    if((clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("Failed to create socket; Error %d: %s\n", errno, strerror(errno));
        return -1;
    }

    /* Connect socket to server */
    if(connect(clientSocket,(struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    {
        printf("Failed to connect socket to %s:%d; Error %d: %s\n", servIPAddr, portNum, errno, strerror(errno));
        return -1;
    }

    return clientSocket;
}

int main(int argc, char **argv)
{
    int mySocket;
    char serverIP[15];
    unsigned int portNum;

    if(argc != 6)
    {
        printf("Usage: bankClient servIPAddr servPortNum command acctNum value\n");
        return -1;
    }

	/* Setup the IP address */
	strcpy(serverIP, argv[1]);
	
	/* Setup TCP port number */
	portNum = atoi(argv[2]);

    /* Setup the client socket */
    if((mySocket = setupTCPClient(serverIP, portNum)) < 0)
    {
        return -1;
    }

    /* Create transaction */
    sBANK_PROTOCOL transaction = createTransaction(argv[3][0], atoi(argv[4]), atoi(argv[5])); 
    if (transaction.trans == -1) return -1;

    sBANK_PROTOCOL* response = sendTransaction(mySocket, transaction);

    printf("\n%d\n", response->value);
    close(mySocket);
}
