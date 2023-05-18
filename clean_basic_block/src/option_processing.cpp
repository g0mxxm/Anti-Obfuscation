#include "option_processing.h"

ProgramOptions ProcessingParams(int argc, char* argv[])
{
	using namespace boost::program_options;

	options_description my_options("Allowed options");
	my_options.add_options()
		("help,h", "produce help message")
		("file_path,f", value<std::string>(), "set the file path of the code file")
		("start_address,a", value<std::string>(), "set the start address of the code")
		("end_address,e", value<std::string>(), "set the end address of the code");

	variables_map options_map;
	store(command_line_parser(argc, argv).options(my_options).run(), options_map);
	notify(options_map);
	

	ProgramOptions program_options;
	if (options_map.empty())
	{
		std::cout << my_options << std::endl;
		throw std::exception("help message!");
	}
	else
	{
		if (options_map.count("file_path"))
		{
			program_options.file_path = options_map["file_path"].as<std::string>();
		}
		if (options_map.count("start_address"))
		{
			program_options.start_address = options_map["start_address"].as<std::string>();
		}
		if (options_map.count("end_address"))
		{
			program_options.end_address = options_map["end_address"].as<std::string>();
		}
		if (options_map.count("help"))
		{
			std::cout << my_options << std::endl;
			throw std::exception("help message!");
		}
	}

	return program_options;
}