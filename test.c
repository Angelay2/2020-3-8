#include <stdio.h>
#include <Windows.h>
#pragma warning(disable:4996)
// �ļ��������д
// 1. ftell(�����ļ�ָ���������ʼλ�õ�ƫ����, ÿдһ������ƫ��һ�� )
// long int ftell(FILE* stream); ����ֵΪһ������ ��Զ���صĶ�����һ�����Ա�д���λ��
// ���԰��ļ������һ��������, д��λ���൱���±�\
// 2. rewind(����д���ļ�������λ��, ��һ��, ���ļ�д���λ��������0

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
			// int fputc(int character, FILE* stream) ��һ��дʲô�ַ�, �ڶ�����д
			printf("before pos: %d\n", pos);
			fputc(c, fp);
			pos = ftell(fp);
			printf("after pos: %d\n", pos);// ������ǰ��д��λ�� ���֤���ַ���һ��һ��д��
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
