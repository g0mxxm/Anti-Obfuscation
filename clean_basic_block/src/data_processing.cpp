#include "data_processing.h"

void PrintHexCode(unsigned char* str, size_t len)
{
    unsigned char* c;

    printf("Code: ");
    for (c = str; c < str + len; c++)
    {
        printf("0x%02x ", *c & 0xff);
    }
    printf("\n");
}

void PrintMapTwoElements(std::unordered_map<int, std::unordered_set<uint16_t>>& map)
{
    for (const auto& entry : map)
    {
        std::cout << entry.first << ": ";
        for (auto reg : entry.second)
        {
			std::cout << reg << " ";
		}
        std::cout << std::endl;
    }
}

void PrintIntSet(std::unordered_set<int>& set)
{
    for (auto reg : set)
    {
		std::cout << reg << " ";
	}
	std::cout << std::endl;
}

void PrintU16Set(std::unordered_set<uint16_t>& set)
{
    for (auto reg : set)
    {
		std::cout << reg << " ";
	}
	std::cout << std::endl;
}

void PrintVector(std::vector<cs_insn>& vec)
{
    for (auto insn : vec)
    {
		printf("0x%" PRIx64 ":\t%s\t\t%s          ", insn.address, insn.mnemonic, insn.op_str);
		std::cout << std::endl;
	}
}
