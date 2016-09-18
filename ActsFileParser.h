#pragma once
#include <string>
#include"FileParser.h"
//#include "WmlMatrix4.h"
#include "ActsParserBase.h"

namespace DFF_VideoMat
{

	class CActsFileParser :public CActsParserBase, public CFileParser
	{
	public:
		CActsFileParser();
		virtual ~CActsFileParser();
		//����act�ļ�
		virtual bool LoadProject(const std::string &filename);
		//�洢��act�ļ�
		virtual bool SaveProject(const std::string &filename, const std::string &imageName, const std::vector<int> & indexs);
		static CActsFileParser* GetInstance()
		{
			static CActsFileParser Instance;
			return &Instance;
		}


		//���㷽λ�Ǻ���
		double GetAzimuth(int index_frame);
		//���㸩���Ǻ���
		double GetPitch(int index_frame);

	private:
		void LocateContext(const char* str, int & start, int& end);
		void LocateContext(const char* str, int rangeBegin, int rangeEnd, int& start, int& end);
		void LoadSequence(int start, int end);
		bool ParseSequence();
		void ParseMotionType();
		void OutputMotionType(std::ofstream &fs);
		void ParseIntrinsicPara();
		void ParseMatchPoint();
		void ParseFeatureTracks();
		void ParseMatchLink();
		void ParseCameraTrack();
		void LoadFrameObjTransform(int frameIndex, int startIndex, int endIndex);

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

}