#include <stdio.h>
#include <Windows.h>
#pragma warning(disable:4996)

// �ļ��������д(�ٿ��ļ�������)
// 1. ftell(�����ļ�ָ���������ʼλ�õ�ƫ����, ÿдһ������ƫ��һ�� )��ȡ��ǰ��λ��
// long int ftell(FILE* stream); ����ֵΪһ������ ��Զ���صĶ�����һ�����Ա�д���λ��
// ���԰��ļ������һ��������, д��λ���൱���±�\
// 2. rewind(����д���ļ�������λ��, ��һ��, ���ļ�д���λ��������0)
// 3. fseek (��������ı��ļ��Ķ�дλ��)
// int fseek(FILE* stream, long int offset, int origin) ��һ���ļ��� �ڶ���ƫ���� �������ο���
// SEEK_SET  �ļ��Ŀ�ʼλ��
// SEEK_CUR  �ļ��ĵ�ǰλ��
// SEEK_END  �ļ��Ľ���λ��
// offset �൱�ڻ���ַ + ƫ���� (1. offset > 0 => ����ƫ, 2. offset < 0  => ��ǰƫ)
// ÿ������д��ʱ�����һ�β������
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
		fputc('x', fp);
		pos = ftell(fp);
		printf("after pos: %d\n", pos);// 1
	}

	fclose(fp);
	system("pause");
	return 0;
}

// �������ļ�����
// 1. �Զ������Ƶ���ʽ��ԭ�ļ�
// 2. ��д�����Ƶ���ʽ��Ŀ���ļ�
// 3. ��Դ�ļ������ݶ�����������,��д���ļ��� �ٰ��ļ�һ��
// C:\Users\Administrator\Desktop
int main2(){
	// �ļ������� Ҫ������ȥ �ȶ���
	printf("Please Enter Src Path: ");// ·��������
	char src_path[1024];// ·������'\'���� ���
	scanf("%s", src_path);
	printf("Please Enter Dst Path: ");//�������ļ�Ҫ�������� ���ֽ�ʲô
	char dst_path[1024];
	scanf("%s", dst_path);

	FILE* src_p = fopen(src_path, "rb");// �������Ƶķ�ʽ��
	FILE* dst_p = fopen(dst_path, "wb");// д�����Ƶķ�ʽ��
	if (src_p == NULL || dst_p == NULL){
		printf("fopen file error!\n");
		return 1;
	}
	// ��src������ݶ����� Ȼ����dst����д(�Զ�������ʽ)
	// ���������Ѿ���src ����
	fseek(src_p, 0, SEEK_END);// 1. ��src�ļ��Ķ�дλ�÷������
	long int size = ftell(src_p);// 2. ��ȡ��ǰд���λ��, size���ǵ�ǰ�ļ��Ĵ�С
	rewind(src_p);// 3. �ٰ��ļ��Ķ�дλ�÷����ʼ
	// �����ļ���С�����, �������һ�������ļ��������ǵ��ڴ���, 
	// Ȼ���ٴ��ڴ�ᵽ���ǵ�����豸�ϻ�Ŀ���ļ���
	// size_t fread(void* buffer, size_t size, size_t count, FILE* stream)
	// ��һ�������Ѷ��������ݷŵ�����, �ڶ���ÿ�ζ������ַ��Ļ�����С
	// ������ �����ȡ���ٸ�������С,  ���ĸ�����������
	// ����ֵ����ʵ�ʶ����Ķ����ַ�
	//size_t fwrite(const void* ptr, size_t size, size_t mem, FILE* stream)
	// ��һ������Ҫд�ĸ�������,�ڶ�����Ҫд�Ļ�����С, ����������������С, ������
	char* mem = (char*)malloc(size);
	if (mem){
		fread(mem, size, 1, src_p);// ���ļ��������ֱ��ȫ�ᵽ��������Ķѿռ���
		fwrite(mem, size, 1, dst_p);
	}
	
	fclose(src_p);
	fclose(dst_p);
	system("pause");
	return 0;
}

// �ļ������ж�
// ������ʹ�õ� feof
// (���ļ���ȡ������, ������ feof �����ķ���ֱֵ�������ж��ļ����Ƿ����, 
// ����Ӧ���ڵ��ļ���ȡ������ʱ��, �ж� => �Ƕ�ȡʧ�ܽ���, ���������ļ���β����
// 1. �ı��ļ���ȡ�Ƿ����, �жϷ���ֵ�Ƿ�ΪEOF(fgetc), ����NULL(fgets)
//  ����: (1) fgetc �ж��Ƿ�ΪEOF,  (2) fgets�жϷ���ֵ�Ƿ�ΪNULL
// ֻ�е������ж�����������, Ȼ����ȥ��������ļ���û���˳�
// 2. �������ļ��Ķ�ȡ�����ж�, ��Ŭ������ֵ�Ƿ�С��ʵ��Ҫ���ĸ���
// ����: fread�ж� ����ֵ�Ƿ�С��ʵ��Ҫ���ĸ���  ����fread(mem, size, 1, src_p) ���size*1��
// ֻҪ����ֵ >= ʵ�ʶ�����1, �Ǿͼ����� ֱ�������� ���ж���û�е����ļ���β 
// ֻҪ�����ļ���β �ļ�������, ����ֵҪô0 Ҫô���ظ��� ���Բ������0(����0 ֤��û���ļ���β)
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
