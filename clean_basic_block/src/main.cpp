#include "data_processing.h"
#include "insn_processing.h"
#include "file_processing.h"
#include "option_processing.h"


int main(int argc, char* argv[])
{
	try
	{
		const ProgramOptions program_options = ProcessingParams(argc, argv);
		std::string file_path = program_options.file_path;
		std::string start_address = program_options.start_address;

		if (!boost::filesystem::exists(file_path))
		{
			std::cout << "File not found!" << std::endl;
			return 0;
		}
		if (start_address.empty())
		{
			std::cout << "Start address not found!" << std::endl;
			return 0;
		}
		else
		{
			uint64_t hex_start_address = std::stoull(start_address, nullptr, 16);
			std::vector<uint8_t> b_code = GetBinaryData(file_path);
			//std::vector<uint8_t> b_code = GetBinaryData("D:\\Code_Files\\Clean_Juck_Code\\data.txt");
			//std::vector<uint8_t> b_code = GetBinaryData("D:\\Code_Files\\Clean_Juck_Code\\b_code.txt");
			//#define b_code "\x8d\x4c\x32\x08\x01\xd8\x81\xc6\x34\x12\x00\x00"

			Platform platform_info;
			platform_info.arch = CS_ARCH_X86;
			platform_info.mode = CS_MODE_32;
			platform_info.code = b_code.data();
			platform_info.size = b_code.size();
			//platform_info.address = 0x0064D713;
			platform_info.address = hex_start_address;
			platform_info.comment = "X86 32 (Intel syntax)";

			AccessInfo access_info = GetInsnAccessInfo(platform_info);
			//printf("access_info.insn_list.size: %d\n", access_info.insn_list.size());
			//printf("access_info.use_list: \n");
			//PrintMapTwoElements(access_info.cs_use);
			//printf("access_info.def_list: \n");
			//PrintMapTwoElements(access_info.cs_def);

			LiveInfo live_info = GetLiveInfo(access_info);
			//printf("live_info.out_set: \n");
			//PrintMapTwoElements(live_info.out_set);
			//printf("live_info.in_set: \n");
			//PrintMapTwoElements(live_info.in_set);

			CleanInfo clean_info = GetCleanInfo(live_info, access_info);
			//printf("clean_info.del_set: \n");
			//PrintIntSet(clean_info.del_set);
			while (clean_info.is_changed)
			{
				access_info = ModifyAccessInfo(access_info, clean_info);
				live_info = GetLiveInfo(access_info);
				clean_info = GetCleanInfo(live_info, access_info);
				//printf("clean_info.del_set: \n");
				//PrintIntSet(clean_info.del_set);
			}
			printf("Insn List After Clean: \n");
			PrintVector(access_info.insn_list);
			printf("clean over!!! \n");
			return 0;
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 0;
	}

}