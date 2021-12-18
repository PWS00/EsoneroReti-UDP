#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h> /* for memset() */
#include "header.h"
void ErrorHandler(char *errorMessage) {
printf(errorMessage);
}
void ClearWinSock() {
#if defined WIN32
WSACleanup();
#endif
}








int main(int argc, char *argv[]) {
#if defined WIN32
WSADATA wsaData;
int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
if (iResult != 0) {
printf ("error at WSASturtup\n");
return EXIT_FAILURE;
}
#endif

	const char * name = "localhost";
	struct hostent *host;
	host = gethostbyname(name);
	if (host == NULL) {
	fprintf(stderr, "gethostbyname() failed.\n");
	exit(EXIT_FAILURE);
	} else {
	struct in_addr* ina = (struct in_addr*) host->h_addr_list[0];
	printf("Nome client : %s, ip client : %s, ", name, inet_ntoa(*ina));
	}


	const char* ip = "127.0.0.1";
	struct in_addr addr;
	addr.s_addr = inet_addr(ip);
	host = gethostbyaddr((char *) &addr, 4, AF_INET);
	char* canonical_name = host->h_name;
	fprintf(stdout, "nome simbolico client : %s\n", canonical_name);







int sock;
struct sockaddr_in echoServAddr;
struct sockaddr_in fromAddr;
int fromSize;
int echoStringLen;

char resultchar[PRECISION], achar[PRECISION], bchar[PRECISION];
	char function;
	float result=0;
	int input_error, a, b, resulcharleng;



// CREATION OF THE SOCKET
if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) ErrorHandler("socket() failed");


// CONSTRUCTION OF THE SERVER ADDRESS
memset(&echoServAddr, 0, sizeof(echoServAddr));
echoServAddr.sin_family = PF_INET;
echoServAddr.sin_port = htons(PORT);
echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.2");


host = gethostbyaddr((char *) &fromAddr, 4, AF_INET);
char* canonical_name_server = host->h_name;
printf("%s:%d\n", canonical_name_server,htons(PORT));
char* ip_server =  inet_ntoa(echoServAddr.sin_addr);


//CALCULATOR

do{
		do{
	input_error=0;
	printf("Inserisci operazione : ");
	scanf("%c %d %d", &function, &a, &b); // the application recognizes negative numbers in input
	if(function!='+' && function!='-' && function!='x' && function!='/' && function!='=') {
		printf("Errore! Funzione non riconosciuta.\n");
		input_error=1;
		fflush(stdin);
	}
	if(function=='/' && b==0) {
		printf("Risultato non appartenente all'insieme dei numeri Reali.\n");
		input_error=1;
		fflush(stdin);
		}
	if(a<-9999 || a>9999 || b<-9999 || b>9999)
	{
		printf("Valori numerici troppo grandi.\n");
		input_error=1;
		fflush(stdin);
	}
	}while(input_error==1);

	//CASTING
		itoa(a, achar, 10);
		itoa(b, bchar, 10);
		if ((echoStringLen = strlen(achar)) > ECHOMAX) ErrorHandler("echo word too long");
		if ((echoStringLen = strlen(bchar)) > ECHOMAX) ErrorHandler("echo word too long");
		if ((echoStringLen = strlen(&function)) > ECHOMAX) ErrorHandler("echo word too long");

	// SENDING THE STRING TO THE SERVER
		echoStringLen=sizeof(function);
		if (sendto(sock, &function, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != echoStringLen)
		{
		ErrorHandler("send() sent a different number of bytes than expected.\n");
		closesocket(sock);
		ClearWinSock();
		return -1;
		}
	//SENDING THE STRING TO THE SERVER
	    echoStringLen=sizeof(achar);
		if (sendto(sock, achar, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != echoStringLen)
		{
		ErrorHandler("send() sent a different number of bytes than expected.\n");
		closesocket(sock);
		ClearWinSock();
		return -1;
		}
	// SENDING THE STRING TO THE SERVER
		echoStringLen=sizeof(bchar);
		if (sendto(sock, bchar, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != echoStringLen)
		{
		ErrorHandler("send() sent a different number of bytes than expected.\n");
		closesocket(sock);
		ClearWinSock();
		return -1;
		}


	// RETURN OF STRING WITH RESULT
	fromSize = sizeof(fromAddr);
	resulcharleng=recvfrom(sock, resultchar, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize);
	if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
	fprintf(stderr, "Error: received a packet from unknown source.\n");
	exit(EXIT_FAILURE);
	}
		result=atof(resultchar);
		if(function!='=')
			{
			printf("Risultato ricevuto dal server %s, ip %s : ",canonical_name_server, ip_server);
			printf("%d %c %d = %.2f\n", a, function, b, result);
			}
		fflush(stdin);
		}while(function!='=');


closesocket(sock);
ClearWinSock();
return EXIT_SUCCESS;
}
