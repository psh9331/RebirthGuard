
#include "Windows.h"
#include "Stdio.h"
#include "AES.h"

#define PADDING(p, size)	p / size * size + (p % size ? size : 0)

int main(void)
{
	BYTE Key[16] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

	HANDLE hSourceFile = CreateFile(L"RebirthGuard.ini", GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD FileSize	= GetFileSize(hSourceFile, 0);
	DWORD FileSize2	= PADDING(FileSize, 16);

	BYTE* Source = (BYTE*)malloc(FileSize2);
	BYTE* Buffer = (BYTE*)malloc(FileSize2);
	memset(Buffer, 0, FileSize2);

	ReadFile(hSourceFile, Source, FileSize, NULL, NULL);

	if (strstr((CHAR*)Source, "[ RebirthGuard ]"))
	{
		for (DWORD i = 0; i < FileSize2; i += 16)
			AES_ECB_Encrypt((BYTE*)Source + i, Key, Buffer + i, 16);
	}
	else
	{
		for (DWORD i = 0; i < FileSize2; i += 16)
			AES_ECB_Decrypt((BYTE*)Source + i, Key, Buffer + i, 16);
	}

	SetFilePointer(hSourceFile, 0, 0, 0);
	WriteFile(hSourceFile, Buffer, FileSize2, NULL, NULL);
	CloseHandle(hSourceFile);

	free(Source);
	free(Buffer);

	return 0;
}