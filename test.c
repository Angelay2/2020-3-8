#include <stdio.h>
#include <Windows.h>
#pragma warning(disable:4996)

// 文件的随机读写(操控文件的属性)
// 1. ftell(返回文件指针相对于起始位置的偏移量, 每写一个往后偏移一个 )获取当前的位置
// long int ftell(FILE* stream); 返回值为一个整数 永远返回的都是下一个可以被写入的位置
// 可以把文件想象成一个大数组, 写入位置相当于下标\
// 2. rewind(不管写到文件的任意位置, 调一下, 让文件写入的位置立马清0)
// 3. fseek (可以任意改变文件的读写位置)
// int fseek(FILE* stream, long int offset, int origin) 第一个文件流 第二个偏移量 第三个参考点
// SEEK_SET  文件的开始位置
// SEEK_CUR  文件的当前位置
// SEEK_END  文件的结束位置
// offset 相当于基地址 + 偏移量 (1. offset > 0 => 往后偏, 2. offset < 0  => 往前偏)
// 每次重新写入时会把上一次操作清空
int main1(){
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
		fputc('x', fp);
		pos = ftell(fp);
		printf("after pos: %d\n", pos);// 1
	}

	fclose(fp);
	system("pause");
	return 0;
}

// 二进制文件拷贝
// 1. 以读二进制的形式打开原文件
// 2. 以写二进制的形式打开目标文件
// 3. 把源文件的内容读进到程序中,再写到文件里 再把文件一关
// C:\Users\Administrator\Desktop
int main2(){
	// 文件从哪来 要到哪里去 先读到
	printf("Please Enter Src Path: ");// 路径的输入
	char src_path[1024];// 路径是以'\'结束 需改
	scanf("%s", src_path);
	printf("Please Enter Dst Path: ");//再输入文件要拷入哪里 名字叫什么
	char dst_path[1024];
	scanf("%s", dst_path);

	FILE* src_p = fopen(src_path, "rb");// 读二进制的方式打开
	FILE* dst_p = fopen(dst_path, "wb");// 写二进制的方式打开
	if (src_p == NULL || dst_p == NULL){
		printf("fopen file error!\n");
		return 1;
	}
	// 把src里的内容读进来 然后往dst里面写(以二进制形式)
	// 首先我们已经把src 打开了
	fseek(src_p, 0, SEEK_END);// 1. 把src文件的读写位置放在最后
	long int size = ftell(src_p);// 2. 获取当前写入的位置, size就是当前文件的大小
	rewind(src_p);// 3. 再把文件的读写位置放在最开始
	// 现在文件大小获得了, 后面可以一口气把文件读到我们的内存中, 
	// 然后再从内存搬到我们的输出设备上或目标文件里
	// size_t fread(void* buffer, size_t size, size_t count, FILE* stream)
	// 第一个参数把读到的数据放到哪里, 第二个每次读到的字符的基本大小
	// 第三个 你想读取多少个基本大小,  第四个你想从哪里读
	// 返回值是你实际读到的多少字符
	//size_t fwrite(const void* ptr, size_t size, size_t mem, FILE* stream)
	// 第一个是你要写哪个缓冲区,第二个你要写的基本大小, 第三个几个基本大小, 第四流
	char* mem = (char*)malloc(size);
	if (mem){
		fread(mem, size, 1, src_p);// 把文件里的内容直接全搬到我们申请的堆空间中
		fwrite(mem, size, 1, dst_p);
	}
	
	fclose(src_p);
	fclose(dst_p);
	system("pause");
	return 0;
}

// 文件结束判定
// 被错误使用的 feof
// (在文件读取过程中, 不能用 feof 函数的返回值直接用来判断文件的是否结束, 
// 而是应用于当文件读取结束的时候, 判断 => 是读取失败结束, 还是遇到文件结尾结束
// 1. 文本文件读取是否结束, 判断返回值是否为EOF(fgetc), 或者NULL(fgets)
//  例如: (1) fgetc 判断是否为EOF,  (2) fgets判断返回值是否为NULL
// 只有当你先判定他读结束了, 然后再去检测他的文件有没有退出
// 2. 二进制文件的读取结束判断, 帕努单返回值是否小于实际要读的个数
// 例如: fread判断 返回值是否小于实际要读的个数  比如fread(mem, size, 1, src_p) 想读size*1个
// 只要期望值 >= 实际读到的1, 那就继续读 直到出问题 再判定有没有到达文件结尾 
// 只要读到文件结尾 文件立马返回, 返回值要么0 要么返回负数 绝对不会大于0(大于0 证明没到文件结尾)
// 

int main(){
	FILE* pFile;
	int n = 0;
	pFile = fopen("myfile.txt", "wb");
	if (NULL == pFile){
		perror("Error opening file");
	}
	else{
		while (fgetc(pFile) != EOF){
			++n;
		}
		if (feof(pFile)){
			puts("End of File reached.");
			printf("Total number of bytes read: %d\n", n);
		}
		else{
			puts("End-of-File was not reached.");
		}
		fclose(pFile);
	}
	system("pause");
	return 0;
}
