#include "FileNameFunctions.h"
#include <cmath>
#include <cstdio>

namespace DFF_VideoMat
{
	CFileNameFunctions::CFileNameFunctions(){}
	CFileNameFunctions::~CFileNameFunctions(){}

	std::string CFileNameFunctions::find_FileName_Dir(const std::string & filename)
	{
		std::string::size_type pos_slant = filename.rfind('/');
		std::string::size_type pos_dslant = filename.rfind('\\');

		if (pos_slant == std::string::npos && pos_dslant != std::string::npos)
		{
			return filename.substr(0, pos_dslant + 1);
		}

		if (pos_slant != std::string::npos && pos_dslant == std::string::npos)
		{
			return filename.substr(0, pos_slant + 1);
		}

		if (pos_slant != std::string::npos && pos_dslant != std::string::npos)
		{
			if (pos_slant > pos_dslant)
				return filename.substr(0, pos_slant + 1);
			else
				return filename.substr(0, pos_dslant + 1);
		}

		return std::string();
	}

	std::string CFileNameFunctions::find_FileName_Name(const std::string &filename)
	{
		std::string::size_type pos_slant = filename.rfind('/');
		std::string::size_type pos_dslant = filename.rfind('\\');

		if (pos_slant == std::string::npos && pos_dslant != std::string::npos)
		{
			return filename.substr(pos_dslant + 1, filename.size());
		}

		if (pos_slant != std::string::npos && pos_dslant == std::string::npos)
		{
			return filename.substr(pos_slant + 1, filename.size());
		}

		if (pos_slant != std::string::npos && pos_dslant != std::string::npos)
		{
			if (pos_slant > pos_dslant)
				return filename.substr(pos_slant + 1, filename.size());
			else
				return filename.substr(pos_dslant + 1, filename.size());
		}

		return filename;
	}

	std::string CFileNameFunctions::find_FileName_Name_WithoutExt(const std::string &filename)
	{
		std::string::size_type pos_slant = filename.rfind('/');
		std::string::size_type pos_dslant = filename.rfind('\\');
		std::string::size_type pos_point = filename.rfind('.');

		if (pos_point == std::string::npos)
			pos_point = filename.size();

		if (pos_slant == std::string::npos && pos_dslant != std::string::npos)
		{
			return filename.substr(pos_dslant + 1, pos_point - pos_dslant - 1);
		}

		if (pos_slant != std::string::npos && pos_dslant == std::string::npos)
		{
			return filename.substr(pos_slant + 1, pos_point - pos_slant - 1);
		}

		if (pos_slant != std::string::npos && pos_dslant != std::string::npos)
		{
			if (pos_slant > pos_dslant)
				return filename.substr(pos_slant + 1, pos_point - pos_slant - 1);
			else
				return filename.substr(pos_dslant + 1, pos_point - pos_dslant - 1);
		}

		return filename.substr(0, pos_point);
	}

	std::string CFileNameFunctions::find_FileName_Ext(const std::string &filename)
	{
		std::string::size_type pos_point = filename.rfind('.');

		if (pos_point != std::string::npos)
		{
			return filename.substr(pos_point + 1, filename.size());
		}

		return std::string();
	}

	std::string CFileNameFunctions::find_FileName_Path_Name_WithoutExt(const std::string &filename)
	{
		std::string::size_type pos_point = filename.rfind('.');

		if (pos_point != std::string::npos)
		{
			return filename.substr(0, pos_point);
		}

		return filename;
	}

	int CFileNameFunctions::find_FileName_Number(const std::string &filename)
	{
		std::string::size_type pos_point = filename.rfind('.');

		if (pos_point == std::string::npos)
			pos_point = filename.size();

		int index_num_begin = pos_point - 1;
		while (index_num_begin >= 0 && isdigit(filename[index_num_begin])) index_num_begin--;
		index_num_begin++;
		if (index_num_begin == pos_point)
			return -1;

		return std::stoi(filename.substr(index_num_begin, pos_point));

	}

	std::string CFileNameFunctions::increase_FileName_Number(const std::string &filename, int increase)
	{
		std::string::size_type pos_point = filename.rfind('.');

		if (pos_point == std::string::npos)
			pos_point = filename.size();

		int index_num_begin = pos_point - 1;
		while (index_num_begin >= 0 && isdigit(filename[index_num_begin])) index_num_begin--;
		index_num_begin++;
		if (index_num_begin == pos_point)
			return filename;

		int valid_num = pos_point - index_num_begin;

		int num = std::stoi(filename.substr(index_num_begin, pos_point));
		num += increase;
		char str_buffer[300];

		switch (valid_num)
		{
		case 2:
			sprintf_s(str_buffer, "%.2d", num);
			break;
		case 3:
			sprintf_s(str_buffer, "%.3d", num);
			break;
		case 4:
			sprintf_s(str_buffer, "%.4d", num);
			break;
		case 5:
			sprintf_s(str_buffer, "%.5d", num);
			break;
		case 6:
			sprintf_s(str_buffer, "%.6d", num);
			break;
		case 7:
			sprintf_s(str_buffer, "%.7d", num);
			break;
		case 8:
			sprintf_s(str_buffer, "%.8d", num);
			break;
		case 9:
			sprintf_s(str_buffer, "%.9d", num);
			break;
		case 10:
			sprintf_s(str_buffer, "%.10d", num);
			break;
		default:
			sprintf_s(str_buffer, "%d", num);
			break;
		}

		std::string result = filename.substr(0, index_num_begin);

		result = result + str_buffer;

		if (pos_point != filename.size())
		{
			result = result + '.' + filename.substr(pos_point + 1, filename.size());
		}
		return result;
	}



}