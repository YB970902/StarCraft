#include "stdafx.h"
#include "EffectReader.h"

unsigned char* EffectReader::ReadShader(std::string filePath, int* dataLen)
{
	std::ifstream is(filePath, std::ifstream::binary);
	unsigned char* buffer = nullptr;
	if (is) {
		// seekg를 이용한 파일 크기 추출
		is.seekg(0, is.end);
		int length = (int)is.tellg();
		is.seekg(0, is.beg);

		// malloc으로 메모리 할당
		buffer = (unsigned char*)malloc(length);

		// read data as a block:
		is.read((char*)buffer, length);
		is.close();
		*dataLen = length;
	}

	return buffer;
}

BOOL EffectReader::LoadEffectFromFile(CString fileName, BYTE** ppData, DWORD* pNumBytes)
{
	HANDLE hFile = nullptr;

	hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	
	DWORD dwByteSizeOfFile = GetFileSize(hFile, NULL);
	*ppData = new BYTE[dwByteSizeOfFile];
	 
	ReadFile(hFile, *ppData, dwByteSizeOfFile, pNumBytes, NULL);

	CloseHandle(hFile);

	return true;
}
