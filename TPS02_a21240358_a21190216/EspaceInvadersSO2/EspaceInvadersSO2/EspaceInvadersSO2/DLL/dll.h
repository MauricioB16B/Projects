#pragma once
#include <stdio.h>
#include <io.h>
#include <Windows.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <tchar.h>
#include <conio.h>
#include <stdlib.h>
#include <Commctrl.h>
#include <Windowsx.h>




// Estruturas
typedef struct obj {
	int lasthit;
	int vida;
	int id;
	int tipo;
	int x;
	int y;
	int tamx;
	int tamy;
	char bitmap[1024];
	char nome[256];
	struct obj * prox;
}obj;
typedef struct {
	int tipo;
	int aux1;
	int aux2;
	int aux3;
	int aux4;
	int aux5;
	TCHAR aux6[1024];
	TCHAR aux7[1024];
	TCHAR aux8[1024];
}msg;
typedef struct {

	msg dados[10];
	int iEscrita;
	int iLeitura;

}bufferinfo;
typedef struct {
	HANDLE hpipe;
	int pid;
}infothread3;

#define BufferSize 100
#define Buffers 10

#ifdef DLL_EXPORTS
#define DLL_IMP_API __declspec(dllexport)
#else
#define DLL_IMP_API __declspec(dllimport)
#endif

extern "C"
{
	DLL_IMP_API obj * mapeamento();
	DLL_IMP_API int buffercircular2(msg dados);
	DLL_IMP_API int buffercircularClienteServer(msg dados);
	DLL_IMP_API int buffercircularServerCliente(infothread3 info);

}