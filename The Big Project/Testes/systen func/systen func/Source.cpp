#include<stdio.h>
#include<Windows.h>

int main(){
	char str[1024];
	while (true) {
		sprintf(str,"");
		scanf("%[^\n]", str);
		scanf("%*c");
		system(str);
	}
	return 0;
}