#include<Windows.h>
#include<stdio.h>

int main()
{
	//HKEY hKey;
	LONG result = 0;
	char filename[] = "C:\test.jpg";
	char path[] = "SOFTWARE\\AAA";
	char data[1024];
	DWORD buffersize = sizeof(data);
	/*
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_WOW64_64KEY, NULL, &hKey, NULL) == ERROR_SUCCESS) // <- HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node (32bits application) (KEY_WOW64_64KEY)(KEY_WOW64_32KEY)
		printf("1. success \n");
	else printf("fail\n");
	
	if (RegCreateKeyEx(HKEY_CURRENT_USER, "Console\\mauricio852", 0, NULL, REG_OPTION_NON_VOLATILE , KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS)
	printf("2. success \n");
	else printf("fail\n");
	*/

	printf("Tipo de sistema: ");
	if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "BuildLabEx", RRF_RT_REG_SZ, NULL, data, &buffersize) == ERROR_SUCCESS) 
		printf_s("	%s \n", data);
	else
		printf("	Erro na leitura!\n");

	printf("Registo de conta: ");
	buffersize = sizeof(data);
	if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "RegisteredOwner", RRF_RT_REG_SZ | RRF_SUBKEY_WOW6464KEY, NULL, data, &buffersize) == ERROR_SUCCESS) 
		printf_s("	%s \n", data);
	else
		printf("	Erro na leitura!\n");

	printf("ID da edicao: ");
	buffersize = sizeof(data);
	if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CompositionEditionID", RRF_RT_REG_SZ | RRF_SUBKEY_WOW6464KEY, NULL, data, &buffersize) == ERROR_SUCCESS)
		printf_s("		%s \n", data);
	else
		printf("		Erro na leitura!\n");

	printf("Caminho de sistema: ");
	buffersize = sizeof(data);
	if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "PathName", RRF_RT_REG_SZ | RRF_SUBKEY_WOW6464KEY, NULL, data, &buffersize) == ERROR_SUCCESS)
		printf_s("	%s \n", data);
	else
		printf("	Erro na leitura!\n");

	printf("CPU: ");
	buffersize = sizeof(data);
	if (RegGetValueA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", RRF_RT_REG_SZ | RRF_SUBKEY_WOW6464KEY, NULL, data, &buffersize) == ERROR_SUCCESS)
		printf_s("			%s \n", data);
	else
		printf("			Erro na leitura!\n");


	printf("Nome do Computador: ");
	buffersize = sizeof(data);
	if (RegGetValueA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName" , "ComputerName", RRF_RT_REG_SZ | RRF_SUBKEY_WOW6464KEY, NULL, data, &buffersize) == ERROR_SUCCESS)
		printf_s("	%s \n", data);
	else
		printf("	Erro na leitura!\n");
	printf("\n\n\n\n\n\n");

	system("pause");
	return 0;
}