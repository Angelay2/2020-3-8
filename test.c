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
// ʵ�ֿ���
// 1. ����֮ǰд�� ���ļ��� ����ļ���С malloc�ռ���ļ����������֮��, 
// 2. Ҳ���Զ��������ַ� Ȼ���ٰ��ض��ַ�fwrite���ļ��� ����ѭ���Ķ� Ҳ���� 

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
	fseek(src_p, 0, SEEK_END);// 1. ��src�ļ��Ķ�дλ��ֱ�ӷ������
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
// ����ȥ�ж��ı����߶������ļ������Ļ�������
// (���ļ���ȡ������, ������ feof �����ķ���ֱֵ�������ж��ļ����Ƿ����, 
// ����Ӧ���ڵ��ļ���ȡ������ʱ��, �ж� => �Ƕ�ȡʧ�ܽ���, ���������ļ���β����
// 1. �ı��ļ���ȡ�Ƿ����, �жϷ���ֵ�Ƿ�ΪEOF(fgetc), ����NULL(fgets)
//  ����: (1) fgetc �ж��Ƿ�ΪEOF,  (2) fgets�жϷ���ֵ�Ƿ�ΪNULL
// ֻ�е������ж�����������, Ȼ����ȥ��������ļ���û���˳�
// 2. �������ļ��Ķ�ȡ�����ж�, �жϷ���ֵ�Ƿ�С��ʵ��Ҫ���ĸ���
// ����: fread�ж� ����ֵ�Ƿ�С��ʵ��Ҫ���ĸ���  ����fread(mem, size, 1, src_p) ���size*1��
// ֻҪ����ֵ >= ʵ�ʶ�����1, �Ǿͼ����� ֱ�������� ���ж���û�е����ļ���β 
// ֻҪ�����ļ���β �ļ�������, ����ֵҪô0 Ҫô���ظ��� ���Բ������0(����0 ֤��û���ļ���β)
// 

int main3(){
	FILE* pFile;
	int n = 0;
	pFile = fopen("myfile.txt", "wb");
	if (NULL == pFile){
		perror("Error opening file");
	}
	else{
		while (fgetc(pFile) != EOF){
			++n;// ֻҪ������EOFһֱ��
		}
		// ֻҪѭ���˳� ˵�����Ѿ�����EOF Ȼ������feof ���ж� ������Ҳ�����
		if (feof(pFile)){
			puts("End of File reached.");
			printf("Total number of bytes read: %d\n", n);// 
		}
		else{
			puts("End-of-File was not reached.");// 
		}
		fclose(pFile);
	}
	system("pause");
	return 0;
}
// �ж��ļ�����
int main4(){
	FILE* fp = fopen("log.txt", "r");
	if (NULL == fp){
		perror("fopen error!");
		return 1;
	}
	// ��������Ҫһֱ���ж� ֱ����������ʧ��
	// �������ǿ��Զ���һ�����Ƕ�Ӧ�ķ���ֵc, �����Ƕ�����ֵ�ŵ�c��
	// Ȼ���һֱ�� ֻҪ������ EOF,���Ǿ�putchar/fputc���������
	// ���������EOF, Ȼ���������ж�, 
	// ferror(fp)���ж���EOF�ǳ����� ���˳�
	// ����feof(fp) �ж��Ƿ��ļ���β, ����� ����Ϊ�ļ���β
	int c = 0;
	while ((c = fgetc(fp)) != EOF){// EOF �Ǹ��� ��ʵ���� -1
		fputc(c, stdout);// �����ǵ��ַ���ʾ����ʾ���� whileѭ������ ֤������
	}
	// �����ǳɹ�����������ж�
	if (ferror(fp)){
		puts("I/O error!\n");// ���Ĺ��̳�����
	}
	else if (feof(fp)){
		printf("End of File!\n");// û����󾭹�feof���ǵ��ļ���β��
	}
	else{
		puts("Unknown!\n");
	}

	fclose(fp);
	system("pause");
	return 0;
}

// �ж��������ļ���β
int main(){
	FILE* fp = fopen("����.jpg", "r");
	if (NULL == fp){
		printf("fopen error!\n");
		return 1;
	}
	// ���Ĺ����б���ע��һ������ -- ��������ֵ�����Ǵ���0(���ߴ��ڵ���1)
	// ֻҪ����0 ��Ϊ�����ǳɹ��� һ����������Ч��ֵ
	//size_t fread(void* buffer, size_t size, size_t count, FILE* stream)
	char buf[1024];
	size_t s = 0;
	while ((s = fread(buf, sizeof(buf), 1, fp)) > 0){
		// ��ֹ������� �ļ����ݴ� ������, ����һֱ��
	}
	if (ferror(fp)){
		puts("IO error!\n");
	}
	else if (feof(fp)){
		puts("End of File!\n");
	}
	else{
	}

	fclose(fp);
	system("pause");
	return 0;
}