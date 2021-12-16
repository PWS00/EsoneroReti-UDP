#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h>
#define PRECISION 5
#include <string.h> /* for memset() */
#define ECHOMAX 255
#define PORT 48000
void ErrorHandler(char *errorMessage) {
printf(errorMessage);
}
void ClearWinSock() {
#if defined WIN32
WSACleanup();
#endif
}

int add(int, int);
int mult(int, int);
int sub(int, int);
float division(int, int);

int main() {
	struct hostent *host;

	char achar[PRECISION], bchar[PRECISION], function, resultchar[PRECISION];
	int a, b, arcv, brcv, frcv;
	float result;

#if defined WIN32
WSADATA wsaData;
int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
if (iResult != 0) {
printf ("error at WSASturtup\n");
return EXIT_FAILURE;
}
#endif
int sock;
struct sockaddr_in echoServAddr;
struct sockaddr_in echoClntAddr;
int cliAddrLen;
int recvMsgSize;


// CREAZIONE DELLA SOCKET
if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
ErrorHandler("socket() failed");


// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
memset(&echoServAddr, 0, sizeof(echoServAddr));
echoServAddr.sin_family = AF_INET;
echoServAddr.sin_port = htons(PORT);
echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.2"); //era 0.1


// BIND DELLA SOCKET
if ((bind(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))) < 0)
ErrorHandler("bind() failed");

const char* ip = "127.0.0.1";
struct in_addr addr;
addr.s_addr = inet_addr(ip);
host = gethostbyaddr((char *) &addr, 4, AF_INET);
char* canonical_name_c = host->h_name;







// RICEZIONE DELLA STRINGA ECHO DAL CLIENT
while(1) {
cliAddrLen = sizeof(echoClntAddr);


//printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

do{
	frcv=recvfrom(sock, &function, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen);
	arcv=recvfrom(sock, achar, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen);
	brcv=recvfrom(sock, bchar, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen);
	a=atoi(achar);
	b=atoi(bchar);
	printf("Richiesta operazione : ' %c %d %d ' dal client %s, ip %s\n", function,a,b,canonical_name_c,inet_ntoa(echoClntAddr.sin_addr));


	switch (function) //the application performs correctly operations with negative numbers
				{
					case '+': result=add(a,b);
					break;
					case '-': result=sub(a,b);
					break;
					case 'x': result=mult(a,b);
					break;
					case '/': result=division(a,b);
					break;
					case '=':
						printf("Client has finished running.\n\n");
					break;
				}
			gcvt(result, 4, resultchar);
			recvMsgSize=sizeof(resultchar);
			if (sendto(sock, resultchar, recvMsgSize, 0, (struct sockaddr *)&echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize)  ErrorHandler("sendto() sent different number of bytes than expected\n");
			fflush(stdin);
			}while(function!='=');





 /* //RINVIA LA STRINGA ECHO AL CLIENT
 recvMsgSize=sizeof(echoBuffer);
if (sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *)&echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize)
ErrorHandler("sendto() sent different number of bytes than expected");*/
}
}




int add(int a, int b){
	int result=0;
	if(a==0) result=b;
	else if(b==0) result=a;
	else result=a+b;
	return result;
}

int mult(int a, int b){
	int result=0;
	if(a==0 || b==0) result=0;
	else result=a*b;
	return result;
}

int sub(int a, int b){
	int result=0;
	if(a==0) result=-b;
	else if(b==0) result=a;
	else result=a-b;
	return result;
}


float division(int a, int b){
	float result=0;
	if(a==0) result=0;
	else result=(float)a/b;
	return result;
}

