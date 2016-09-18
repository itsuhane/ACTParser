#pragma once
#include<string>

namespace DFF_VideoMat
{
	class CFileNameFunctions
	{
	public:
		CFileNameFunctions();
		~CFileNameFunctions();

		static std::string find_FileName_Dir(const std::string &filename);
		static std::string find_FileName_Name(const std::string & fileneme);
		static std::string find_FileName_Name_WithoutExt(const std::string &filename);
		static std::string find_FileName_Ext(const std::string &string);
		static std::string find_FileName_Path_Name_WithoutExt(const std::string &filename);
		static int find_FileName_Number(const std::string &filename);
		static std::string increase_FileName_Number(const std::string &filename, int increase);
	};
}