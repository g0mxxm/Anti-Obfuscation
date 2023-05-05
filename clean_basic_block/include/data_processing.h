#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "capstone/capstone.h"

void PrintHexCode(unsigned char*, size_t);
void PrintMapTwoElements(std::unordered_map<int, std::unordered_set<uint16_t>>& map);
void PrintIntSet(std::unordered_set<int>& set);
void PrintU16Set(std::unordered_set<uint16_t>& set);
void PrintVector(std::vector<cs_insn>& vec);


#endif