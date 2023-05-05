#include "insn_processing.h"
#include "data_processing.h"

// Merge REG
std::unordered_map<uint16_t, uint16_t> reg_map = {
    {X86_REG_AH, X86_REG_EAX}, {X86_REG_AL, X86_REG_EAX}, {X86_REG_AX, X86_REG_EAX},
    {X86_REG_BH, X86_REG_EBX}, {X86_REG_BL, X86_REG_EBX}, {X86_REG_BX, X86_REG_EBX},
    {X86_REG_CH, X86_REG_ECX}, {X86_REG_CL, X86_REG_ECX}, {X86_REG_CX, X86_REG_ECX},
    {X86_REG_DH, X86_REG_EDX}, {X86_REG_DL, X86_REG_EDX}, {X86_REG_DX, X86_REG_EDX},
    {X86_REG_BP, X86_REG_EBP}, {X86_REG_BPL, X86_REG_EBP},
    {X86_REG_SP, X86_REG_ESP}, {X86_REG_SPL, X86_REG_ESP},
    {X86_REG_SI, X86_REG_ESI}, {X86_REG_SIL, X86_REG_ESI},
    {X86_REG_DI, X86_REG_EDI}, {X86_REG_DIL, X86_REG_EDI}
};

uint16_t ExtendRegTo32bit(uint16_t& cs_reg)
{
    auto pos = reg_map.find(cs_reg);
    if (pos != reg_map.end())
    {
        return pos->second;
    }
    else
    {
        return cs_reg;
    }
}

AccessInfo GetInsnAccessInfo(Platform& platform)
{
    csh handle;
    cs_insn *insn;
    //cs_detail *detail;
    cs_err err;
    cs_x86 *x86;
    //size_t count;
    
    const uint8_t *code;
    size_t size;
    uint64_t address;
    cs_regs regs_read, regs_write;
    uint8_t regs_read_count, regs_write_count;
    AccessInfo access_info;
    int index = 0;

    printf("****************\n");
    printf("Platform: %s\n", platform.comment);
    err = cs_open(platform.arch, platform.mode, &handle);
    if (err)
    {
        printf("Failed on cs_open() with error returned: %u\n", err);
        abort();
    }
    if (platform.opt_type)
        cs_option(handle, platform.opt_type, platform.opt_value);
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

    insn = cs_malloc(handle);
    x86 = &(insn->detail->x86);
    PrintHexCode(platform.code, platform.size);

    code = platform.code;
    size = platform.size;
    address = platform.address;

    printf("Disasm:\n");

    while (cs_disasm_iter(handle, &code, &size, &address, insn))
    {
        printf("0x%" PRIx64 ":\t%s\t\t%s          ", insn->address, insn->mnemonic, insn->op_str);
        std::cout << std::endl;

        access_info.insn_list.push_back(*insn);
        if (!cs_regs_access(handle, insn, regs_read, &regs_read_count, regs_write, &regs_write_count))
        {
            if (regs_read_count)
            {
                for (int i = 0; i < regs_read_count; i++)
                {
                    regs_read[i] = ExtendRegTo32bit(regs_read[i]);
                }
                access_info.cs_use[index] = std::unordered_set<uint16_t>(regs_read, regs_read + regs_read_count);
            }
            if (regs_write_count)
            {
                for (int i = 0; i < regs_write_count; i++)
                {
                    regs_write[i] = ExtendRegTo32bit(regs_write[i]);
                }
                access_info.cs_def[index] = std::unordered_set<uint16_t>(regs_write, regs_write + regs_write_count);
            }
        }
        index += 1;
    }
    printf("*****************************\n");

    cs_free(insn, 1);
    cs_close(&handle);
    return access_info;
}

LiveInfo GetLiveInfo(AccessInfo& access_info)
{
    LiveInfo  live_info;

    int insn_list_len = access_info.insn_list.size();
    for (int i = 0; i < insn_list_len; i++)
    {
        live_info.in_set[i] = std::unordered_set<uint16_t>();
        live_info.out_set[i] = std::unordered_set<uint16_t>();
	}
    int Exit = insn_list_len;
    //live_info.in_set[Exit] = { X86_REG_EBX , X86_REG_EBP };
    live_info.in_set[Exit] = access_info.cs_use[Exit - 1];
    //printf("in_set[Exit]: \n");
    //PrintU16Set(live_info.in_set[Exit]);

    for (int i = insn_list_len - 1; i >= 0; i--)
    {
        std::unordered_set<uint16_t> temp = std::unordered_set<uint16_t>();
        int succ = i + 1;

        std::set_union(live_info.out_set[i].begin(), live_info.out_set[i].end(), 
                        live_info.in_set[succ].begin(), live_info.in_set[succ].end(), 
                        std::inserter(temp, temp.end()));
        live_info.out_set[i].swap(temp);
        temp.clear();

        std::unordered_set<uint16_t> temp_cs_use = std::unordered_set<uint16_t>();
        std::set_difference(live_info.out_set[i].begin(), live_info.out_set[i].end(), 
                            access_info.cs_def[i].begin(), access_info.cs_def[i].end(), 
                            std::inserter(temp, temp.end()));
        std::set_union(access_info.cs_use[i].begin(), access_info.cs_use[i].end(), 
                        temp.begin(), temp.end(), 
                        std::inserter(temp_cs_use, temp_cs_use.end()));
        live_info.in_set[i].swap(temp_cs_use);
        temp_cs_use.clear();
    }

    return live_info;
}

CleanInfo GetCleanInfo(LiveInfo& live_info, AccessInfo& access_info)
{
    CleanInfo clean_info;
    int insn_list_len = access_info.insn_list.size();

    clean_info.del_set = std::unordered_set<int>();
    clean_info.is_changed = false;

    for (int i = 0; i < insn_list_len; i++)
    {
        int def_count = access_info.cs_def[i].size();
        if (def_count == 0) 
        {
            continue;
        }
        else
        {
            for (auto reg = access_info.cs_def[i].begin(); reg != access_info.cs_def[i].end(); reg++)
            {
                if (live_info.out_set[i].find(*reg) == live_info.out_set[i].end())
                {
                    def_count -= 1;
                }
            }
            if (def_count == 0)
            {
                clean_info.del_set.insert(i);
				clean_info.is_changed = true;
            }
        }
	}
    return clean_info;
}

AccessInfo ModifyAccessInfo(AccessInfo& access_info, CleanInfo& clean_info)
{
    AccessInfo new_access_info;
    int insn_list_len = access_info.insn_list.size();
    int live_index = 0;
    for (int i = 0; i < insn_list_len; i++)
    {
        if (clean_info.del_set.find(i) == clean_info.del_set.end())
        {
            new_access_info.insn_list.push_back(access_info.insn_list[i]);
			new_access_info.cs_use[live_index] = access_info.cs_use[i];
            new_access_info.cs_def[live_index] = access_info.cs_def[i];
			live_index += 1;
		}
	}
    return new_access_info;
}