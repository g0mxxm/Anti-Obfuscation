#include "file_processing.h"

// Read Binary Data From File
std::vector<uint8_t> GetBinaryData(const std::string& file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return {};
    }
    std::vector<uint8_t> b_code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return b_code;
}

// Get PE Data
std::vector<BYTE> GetPEData(const char* file_path, DWORD rva, DWORD size)
{
    std::vector<BYTE> data;

    std::ifstream file(file_path, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return data;
    }

    IMAGE_DOS_HEADER dos_header;
    IMAGE_NT_HEADERS nt_header;
    file.read(reinterpret_cast<char*>(&dos_header), sizeof(dos_header));
    file.seekg(dos_header.e_lfanew, std::ios::beg);
    file.read(reinterpret_cast<char*>(&nt_header), sizeof(nt_header));

    IMAGE_SECTION_HEADER section_header;
    DWORD sectionOffset = dos_header.e_lfanew + sizeof(nt_header.Signature) + sizeof(nt_header.FileHeader) + nt_header.FileHeader.SizeOfOptionalHeader;
    for (int i = 0; i < nt_header.FileHeader.NumberOfSections; i++)
    {
        file.seekg(sectionOffset, std::ios::beg);
        file.read(reinterpret_cast<char*>(&section_header), sizeof(section_header));
        sectionOffset += sizeof(section_header);

        if (rva >= section_header.VirtualAddress && rva < section_header.VirtualAddress + section_header.Misc.VirtualSize)
        {
            DWORD fileOffset = section_header.PointerToRawData + rva - section_header.VirtualAddress;

            data.resize(size);
            file.seekg(fileOffset, std::ios::beg);
            file.read(reinterpret_cast<char*>(data.data()), size);
            break;
        }
    }

    return data;
}