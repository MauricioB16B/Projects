
#include "Gateway.h"




int _tmain() {
	obj * mapa;
	obj FimTrasmissao;
	HANDLE mapUpdate,jalitudo;
	int i,ih;

	CreateThread(NULL, 0, thread1, (LPVOID)NULL, 0, NULL);
	
	mapUpdate = abreEvento(TEXT("MapUpdate"));
	jalitudo = CreateSemaphore(NULL,0,1,L"PodeEscreverMapa");
	if (mapUpdate == NULL)
		return 0;
	mapa = mapeamento();
	FimTrasmissao.id = 5000;
	while (true){
		WaitForSingleObject(mapUpdate,INFINITE);
		//wprintf_s(L"Vou enviar mapa para o cliente\n");
		for (ih = 0;ih < 30;ih++) {
			if (arrayhandles[ih] != NULL) {
				for (i = 0;i < 300;i++) {
					//if (mapa[i].id != NULL) {
						if (!WriteFile(arrayhandles[ih], &mapa[i], sizeof(obj), NULL, NULL)) {
							wprintf_s(L"ERRO na escrita do pipe OBJ\n");
						}else{
							//wprintf_s(L"envio de NAVE!\n");
						}
					//}
				}
				//WriteFile(arrayhandles[ih], &FimTrasmissao, sizeof(obj), NULL, NULL);
			}
		}
		ReleaseSemaphore(jalitudo, 1, NULL);
	}
	return 0;
}

DWORD WINAPI thread1(LPVOID param) {
	HANDLE handleMain;
	HANDLE semaforo1 = CreateSemaphore(NULL, 1, 1, TEXT("semaforo1"));
	while (true) {

		handleMain = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_INBOUND, PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, PIPE_UNLIMITED_INSTANCES, sizeof(msg), sizeof(msg), 1000, NULL);
		if (handleMain == NULL)
			wprintf_s(L"Erro na criaçao do PipeMain");

		if (!ConnectNamedPipe(handleMain, NULL)) {
			printf_s("EROO Ligaçao ao cliente! (ConnectNamedPipe)");
		}

		WaitForSingleObject(semaforo1, INFINITE);
		CreateThread(NULL, 0, thread2, (LPVOID)&handleMain, 0, NULL);
		WaitForSingleObject(semaforo1, INFINITE);
		ReleaseSemaphore(semaforo1, 1, NULL);

	}
	return 0;
}

DWORD WINAPI thread2(LPVOID param) {
	HANDLE semaforo1 = CreateSemaphore(NULL, 1, 1, TEXT("semaforo1"));
	HANDLE * phandle;
	HANDLE hRead;
	phandle = (HANDLE *)param;
	hRead = *phandle;
	ReleaseSemaphore(semaforo1, 1, NULL);
	msg data;
	HANDLE hWrite,hObj, mutexarrayhandles = CreateMutex(NULL, FALSE, TEXT("mutexarray"));
	TCHAR string1[1024];
	infothread3 infoth3;
	int i;


	ReadFile(hRead, &data, sizeof(msg), NULL, NULL);

	if (!WaitNamedPipe(data.aux8, NMPWAIT_WAIT_FOREVER)) {
		wprintf_s(TEXT("Pipe MSG do cliente nao encontrado.        A terminar thread!\n"));
		return 0;
	}

	hWrite = CreateFile(data.aux8, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hWrite == NULL) {
		wprintf_s(TEXT("ERROR! (hWrite) (CreateFile).              A terminar thread"));
		return 0;
	}
	//  hWrite pronto para usar
	
	Sleep(100);// eu sei que nao se deve usar isto mas é provisorio so para garantir que isto corre bem

	swprintf_s(string1, L"%sobj", data.aux8);
	if (!WaitNamedPipe(string1, NMPWAIT_WAIT_FOREVER)) {
		wprintf_s(TEXT("Pipe OBJ do cliente nao encontrado.        A terminar thread"));
		return 0;
	}

	hObj = CreateFile(string1, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hObj == NULL) {
		wprintf_s(TEXT("ERROR! (hObj) (CreateFile).               A terminar thread"));
		return 0;
	}

	WaitForSingleObject(mutexarrayhandles, INFINITE);

	for (i = 0;i<30;i++) {
		if (arrayhandles[i] == NULL) {
			arrayhandles[i] = hObj;
			i = 35;
		}
	}
	ReleaseMutex(mutexarrayhandles);
	if (i != 36) {
		wprintf_s(TEXT("ERROR! (ArrayHandles).               A terminar thread"));
		return 0;
	}

	infoth3.hpipe = hWrite;
	infoth3.pid = _wtoi(data.aux8 + 9);
	CreateThread(NULL, 0, thread3, (LPVOID)&infoth3, 0, NULL);// no need semaforo! so é lancada uma thread destas por cliente

	wprintf_s(TEXT("Cliente \"%s\" Sem erros! tudo OK!!!\n"), data.aux8);

	while (true){
		
		ReadFile(hRead, &data, sizeof(msg), NULL, NULL);
		//wprintf_s(L"cliente --> server == %s\n",data.aux6);
		
		if (data.aux1 == 33) {// aux1 == 33 é mensagem para testar comunicaçao com o getaway

		}
		else {
			buffercircularClienteServer(data);
		}

	}

	return 0;
}

DWORD WINAPI thread3(LPVOID param) {
	infothread3 *pinfo;
	infothread3 info;
	pinfo = (infothread3 *)param;
	info = *pinfo;
	

	buffercircularServerCliente(info);
	
	
	return 0;
}



HANDLE abreEvento(TCHAR string[1024]) {
	int i=0;
	HANDLE hand;
	do {
		hand = OpenEvent(EVENT_ALL_ACCESS, FALSE, string);
		if (hand == NULL) {
			Sleep(5);
		}
		if (i==3000) {//depois de mais ou menos 5 segundoas a tentar abrir o evento (see sleep(5))
			wprintf_s(L"Falha Catastrofica a abrir EVENTO\n");
			return NULL;
		}
		i++;
	} while (hand == NULL);

	return hand;
}
