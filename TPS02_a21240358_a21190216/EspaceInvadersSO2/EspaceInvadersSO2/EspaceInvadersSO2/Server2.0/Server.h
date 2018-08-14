#pragma once
#include "..\dll\dll.h"

typedef struct {
	int tamx;
	int tamy;
	int tipo;
	TCHAR bitmap[1024];
}tipo;
typedef struct definicoes {
	int maxx;
	int maxy;
	int nnaves;
	int Tdireita;
	int Tesquerda;
	int Tdisparo;
	int Tdireita2;
	int Tesquerda2;
	int Tdisparo2;
	TCHAR jogador1[1024];
	TCHAR jogador2[1024];
	int pid1;
	int pid2;
	int pontos1;
	int pontos2;
	tipo naveg;
	tipo navep;
	tipo tiro;
	tipo bomba;
	tipo tjogador1;
	tipo tjogador2;
	tipo power1;
	tipo power2;
	tipo power3;
	tipo power4;
	tipo power5;
	tipo power6;
	tipo power7;
	tipo power8;
	int folgax;
	int folgay;
	int folgahor;
	int folgaver;
}def;

HINSTANCE hInst;
def definicoes;
int objCounterID, nnaves, nnavesprontas, direcao, descida, level = 1;
HANDLE mapUpdate, cllock, mutex, clock2, jalitudo;

int tratamsg(msg data, obj * objectos);
int crianave(obj objecto, int indice, obj * objectos);
int criamapa(obj * objectos);
int criamapa1p();
int criamapa2p();
int criajogador1(obj *objectos);
int criajogador2(obj *objectos);
void SendDefinitions(int pid);
obj * mapeamento();
int CriaNovoJogo(msg data);
int tiro(msg data, obj * objectos);
int move(msg data, obj * objectos);
int buffercircular();
int buffercircular2(msg dados);
int apanhatecla();
DWORD WINAPI thread1(LPVOID param);
DWORD WINAPI thread2(LPVOID param);
DWORD WINAPI thread3(LPVOID param);
DWORD WINAPI thread4(LPVOID param);
DWORD WINAPI thread5(LPVOID param);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Dialog1Proc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);