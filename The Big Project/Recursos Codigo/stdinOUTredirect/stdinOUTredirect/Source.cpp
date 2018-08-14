#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>

/*
int main() {
	int a;
	char buff[1024];
	FILE *input = stdin;
	input = fopen("cenas.txt", "r+");
	if (input == NULL) {
		printf("deu merda\n");
	}
	else {
		while (true)
		{
			fread(buff, 20, 1, input);
			printf("%s", buff);
		}
	}
	system("pause");
	return 0;
}
*/

int main() {



	freopen("1995.txt", "a", stdout);

	printf("cenas maradas\n mauricio2018\n windows10\n ;)");
	fclose(stdout);

	freopen("CONOUT$","a",stdout);

	printf("prontinho\n");
	system("pause");
	return 0;
}
	/*

	a = CreateFileA("1995.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL , OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (a == NULL)
		printf("Erro open\n");

	ret = SetStdHandle(STD_OUTPUT_HANDLE, a);

	if (GetStdHandle(STD_OUTPUT_HANDLE) == a)
		printf("redirected\n");

	if (ret == 0) {
		printf("erro! no REDIRECT\n");
		printf("%d\n\n",GetLastError());
	}
	else
	{
		printf("Malha!\n");
	}

	scanf("%s",str);
	printf("%s\n",str);

	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),str,strlen(str),&bit,0);

	system("pause");
	return 0;
}
*/