#include <stdio.h>
#include <Windows.h>
#pragma warning(disable:4996)
// �ļ��������д(�ٿ��ļ�������)
// 1. ftell(�����ļ�ָ���������ʼλ�õ�ƫ����, ÿдһ������ƫ��һ�� )
// long int ftell(FILE* stream); ����ֵΪһ������ ��Զ���صĶ�����һ�����Ա�д���λ��
// ���԰��ļ������һ��������, д��λ���൱���±�\
// 2. rewind(����д���ļ�������λ��, ��һ��, ���ļ�д���λ��������0
// 3. fseek (��������ı��ļ��Ķ�дλ��)
// int fseek(FILE* stream, long int offset, int origin) ��һ���ļ��� �ڶ���ƫ���� �������ο���
// SEEK_SET  �ļ��Ŀ�ʼλ��
// SEEK_CUR  �ļ��ĵ�ǰλ��
// SEEK_END  �ļ��Ľ���λ��
// offset �൱�ڻ���λ�� + ƫ���� (1. offset > 0 => ����ƫ, 2. offset < 0  => ��ǰƫ)
// ÿ������д��ʱ�����һ�β������
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
		//rewind(fp);// ���ļ�ָ��ص���ʼλ��0
		fseek(fp, 0, SEEK_SET);// �ļ���д��λ�òο���ǰ��ָ��λ��  ��1����Ϊx
		fseek(fp, 5, SEEK_SET); // ��5����Ϊx
		fseek(fp, -5, SEEK_END);//������5����Ϊx
		fputc('x', fp);// 
		pos = ftell(fp);
		printf("after pos: %d\n", pos);// 1
	}

	fclose(fp);
	system("pause");
	return 0;
}


