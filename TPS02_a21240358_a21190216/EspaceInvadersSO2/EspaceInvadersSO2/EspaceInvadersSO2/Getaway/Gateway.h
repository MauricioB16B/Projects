#pragma once
#include "..\dll\dll.h"




#define PIPE_NAME TEXT("\\\\.\\pipe\\main")


HANDLE arrayhandles[30];

DWORD WINAPI thread1(LPVOID param); // thread Recebe cliente
DWORD WINAPI thread2(LPVOID param); // thread trata cliente 1 (Cliente --> Servidor)
DWORD WINAPI thread3(LPVOID param); // thread trata cliente 2 (Servidor --> Cliente)
HANDLE abreEvento(TCHAR string[1024]);
int buffercircularClienteServer(msg dados);
int buffercircularServerCliente(infothread3 info);
obj * mapeamento();