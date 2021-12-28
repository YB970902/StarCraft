#pragma once
class EffectReader
{
public:
	static unsigned char* ReadShader(std::string filePath, int* dataLen);
	static BOOL LoadEffectFromFile(CString fileName, BYTE** ppData, DWORD* pNumBytes);
};