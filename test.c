#include <stdio.h>
#include <Windows.h>
#pragma warning(disable:4996)
// 文件的随机读写
// 1. ftell(返回文件指针相对于起始位置的偏移量, 每写一个往后偏移一个 )
// long int ftell(FILE* stream); 返回值为一个整数 永远返回的都是下一个可以被写入的位置
// 可以把文件想象成一个大数组, 写入位置相当于下标\
// 2. rewind(不管写到文件的任意位置, 调一下, 让文件写入的位置立马清0

int main(){
	FILE* fp = fopen("log.txt", "w");
	if (NULL == fp){
		perror("fopen");
		return 1;
	}
	else{
		char c = 'A';
		long int pos = 0;
		for (; c <= 'Z'; c++){
			// int fputc(int character, FILE* stream) 第一个写什么字符, 第二往哪写
			printf("before pos: %d\n", pos);
			fputc(c, fp);
			pos = ftell(fp);
			printf("after pos: %d\n", pos);// 反馈当前的写入位置 结果证明字符是一个一个写的
		}
		rewind(fp);
		fputc('x', fp);// 
		pos = ftell(fp);
		printf("after pos: %d\n", pos);// 27
	}

	fclose(fp);
	system("pause");
	return 0;
}
