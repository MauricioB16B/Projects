#pragma comment(lib,"dnsapi.lib")
#pragma comment(lib, "Ws2_32.lib")



#include <winsock2.h>  //winsock
#include <windns.h>   //DNS api's
#include <stdio.h>    //standard i/o


int main(int argc, char *argv[])
{
	DNS_STATUS status;               //Return value of  DnsQuery_A() function.
	PDNS_RECORD pDnsRecord;          //Pointer to DNS_RECORD structure.
	PIP4_ARRAY pSrvList = NULL;      //Pointer to IP4_ARRAY structure.
	char pOwnerName[1024];        //Owner name to be queried.
	IN_ADDR ipaddr;

	while (true){
		printf_s("Dominio:\n");
		scanf("%s", pOwnerName);

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
			printf(" O endereco IP do Host '%s' is '%s' \n", pOwnerName, inet_ntoa(ipaddr));

			// Free memory allocated for DNS records. 
			DnsRecordListFree(pDnsRecord, DnsFreeRecordListDeep);
			system("pause");
		}

		LocalFree(pSrvList);
		system("cls");
	}
	return 0;
}