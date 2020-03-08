#include <stdio.h>
#include <Windows.h>
#pragma warning(disable:4996)
// 文件的随机读写(操控文件的属性)
// 1. ftell(返回文件指针相对于起始位置的偏移量, 每写一个往后偏移一个 )
// long int ftell(FILE* stream); 返回值为一个整数 永远返回的都是下一个可以被写入的位置
// 可以把文件想象成一个大数组, 写入位置相当于下标\
// 2. rewind(不管写到文件的任意位置, 调一下, 让文件写入的位置立马清0
// 3. fseek (可以任意改变文件的读写位置)
// int fseek(FILE* stream, long int offset, int origin) 第一个文件流 第二个偏移量 第三个参考点
// SEEK_SET  文件的开始位置
// SEEK_CUR  文件的当前位置
// SEEK_END  文件的结束位置
// offset 相当于基地位置 + 偏移量 (1. offset > 0 => 往后偏, 2. offset < 0  => 往前偏)
// 每次重新写入时会把上一次操作清空
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
		//rewind(fp);// 将文件指针回到起始位置0
		fseek(fp, 0, SEEK_SET);// 文件的写入位置参考当前的指定位置  第1个变为x
		fseek(fp, 5, SEEK_SET); // 第5个变为x
		fseek(fp, -5, SEEK_END);//倒数第5个变为x
		fputc('x', fp);// 
		pos = ftell(fp);
		printf("after pos: %d\n", pos);// 1
	}

	fclose(fp);
	system("pause");
	return 0;
}


