#ifndef FILE_PROCESSING_H
#define FILE_PROCESSING_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>

std::vector<uint8_t> GetBinaryData(const std::string& file_path);
std::vector<BYTE> GetPEData(const char* filePath, DWORD rva, DWORD size);

#endif