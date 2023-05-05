#ifndef OPTION_PROCESSING_H
#define OPTION_PROCESSING_H

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

using namespace boost::filesystem;


enum DataSourceType
{
	ORACLE,			//oracle类型的数据库
	SQLSERVER		//sqlserver类型的数据库
};
struct ProgramOptions
{
	std::string file_path;
	std::string start_address;
};

ProgramOptions ProcessingParams(int argc, char* argv[]);

#endif // OPTION_PROCESSING_H
