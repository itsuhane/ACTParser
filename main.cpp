#include<iostream>
#include"ActsFileParser.h"

int main()
{


	DFF_VideoMat::CActsFileParser file_parser;
	if (file_parser.LoadProject("D:\\LabWork\\CG2014Data\\01105\\01105\\seq_0-2-827_cam_tracking.act"))
	{
		std::cout << file_parser.m_frameCount << std::endl;
		std::cout << file_parser.GetAzimuth(20) << "   " << file_parser.GetPitch(20) << std::endl;
		std::cout << file_parser.m_frameCount << std::endl;

	}
	else
	{
		std::cout << "Can't Parser the file!\n";
	}


	return 0;
}