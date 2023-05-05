#ifndef INSN_PROCESSING_H
#define INSN_PROCESSING_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include "capstone/x86.h"
#include "capstone/capstone.h"
#include "capstone/platform.h"

extern std::unordered_map<uint16_t, uint16_t> reg_map;

struct AccessInfo{
	std::vector<cs_insn> insn_list;
	std::unordered_map<int, std::unordered_set<uint16_t>> cs_use;
	std::unordered_map<int, std::unordered_set<uint16_t>> cs_def;
};
struct LiveInfo {
    std::unordered_map<int, std::unordered_set<uint16_t>> out_set;
    std::unordered_map<int, std::unordered_set<uint16_t>> in_set;
};
struct CleanInfo {
	std::unordered_set<int> del_set;
    bool is_changed;
};
struct Platform {
    cs_arch arch;
    cs_mode mode;
    uint8_t *code;
    uint64_t address;
    size_t size;
    const char* comment;
    cs_opt_type opt_type;
    cs_opt_value opt_value;
};

uint16_t ExtendRegTo32bit(uint16_t& cs_reg);
AccessInfo GetInsnAccessInfo(Platform& platform);
LiveInfo GetLiveInfo(AccessInfo& access_info);
CleanInfo GetCleanInfo(LiveInfo& live_info, AccessInfo& access_info);
AccessInfo ModifyAccessInfo(AccessInfo& access_info, CleanInfo& clean_info);


#endif