#include "ActsFileParser.h"
#include "FileNameFunctions.h"
#include <iostream>
#include <fstream>
#include "MatchLinker.h"
#include <vector>
#include <cstring>

namespace DFF_VideoMat
{

	CActsFileParser::CActsFileParser() :
		CFileParser(), CActsParserBase(){
		m_indexBuffer = 0;
	}


	CActsFileParser::~CActsFileParser(){}

	bool CActsFileParser::LoadProject(const std::string& fileName)
	{
		if (!ReadFile(fileName.c_str()))
		{
			std::cerr << "Error:Can't open the ACT project file: " << fileName << std::endl;
			return false;
		}

		ReadLine();
		if (strncmp(m_pBufferLine, "#camera track project file", strlen("#camera track project file")))
		{
			std::cerr << "This is not camera track project file!" << std::endl;
			return false;
		}

		if (!ParseSequence())
		{
			return false;
		}

		ParseMotionType();
		ParseIntrinsicPara();
		ParseFeatureTracks();
		ParseCameraTrack();
		return true;

	}

	void CActsFileParser::LocateContext(const char* str, int & start, int & end)
	{
		std::string keyStringStart, keyStringEnd;
		keyStringStart = std::string("<") + std::string(str) + std::string(">");
		keyStringEnd = std::string("</") + std::string(str) + std::string(">");

		if (OffsetToString(keyStringStart.c_str()))
		{
			start = m_indexBuffer + keyStringStart.length();
		}

		if (OffsetToString(keyStringEnd.c_str()))
		{
			end = m_indexBuffer - 1;
		}

	}

	void CActsFileParser::LocateContext(const char* str, int rangeBegin, int rangeEnd, int & start, int & end)
	{
		std::string keyStringStart, keyStringEnd;
		keyStringStart = std::string("<") + std::string(str) + std::string(">");
		keyStringEnd = std::string("</") + std::string(str) + std::string(">");

		if (OffsetToString(keyStringStart.c_str(), rangeBegin, rangeEnd))
		{
			start = m_indexBuffer + keyStringStart.length();
		}
		if (OffsetToString(keyStringEnd.c_str(), rangeBegin, rangeEnd))
		{
			end = m_indexBuffer - 1;
		}

	}

	bool CActsFileParser::ParseSequence()
	{
		int start, end;

		LocateContext("Image Sequence", start, end);
		if (OffsetToString("Sequence:", start, end))
		{
			int index_path;
			index_path = m_indexBuffer + strlen("Sequence:");
			strcpy_s(m_pBufferWord, m_pBufferLine + strlen("Sequence:"));
			std::string tmp_path = m_pBufferWord;
			int tmp_index_start = tmp_path.find_first_not_of(' ');
			int tmp_index_end = tmp_path.find_last_not_of(' ');

			tmp_path = tmp_path.substr(tmp_index_start, tmp_index_end - tmp_index_start + 1);

			if (tmp_path[0] == '.')
				tmp_path = CFileNameFunctions::find_FileName_Dir(m_fileName) + tmp_path.substr(2);

			m_imgPath = tmp_path;
		}

		if (OffsetToString("start:", start, end))
		{
			int index_start_frame = m_indexBuffer + strlen("start:");
			strcpy_s(m_pBufferWord, m_pBufferLine + strlen("start:"));
			m_startFrame = atoi(m_pBufferWord);
		}

		if (OffsetToString("step:", start, end))
		{
			int index_step = m_indexBuffer + strlen("step:");
			strcpy_s(m_pBufferWord, m_pBufferLine + strlen("step:"));
			m_step = std::max(atoi(m_pBufferWord), 1);
		}

		if (OffsetToString("end:", start, end))
		{
			int index_end_frame = m_indexBuffer + strlen("end:");
			strcpy_s(m_pBufferWord, m_pBufferLine + strlen("end:"));
			m_endFrame = atoi(m_pBufferWord);
		}

		m_step = std::max(1, m_step);
		m_frameCount = (m_endFrame - m_startFrame) / m_step + 1;
		m_pFramesMatchPoints.assign(m_frameCount, std::shared_ptr<std::vector<CMatchPoint> >());


		return true;
	}


	void CActsFileParser::ParseMotionType()
	{
		int start = -1, end = -2;

		std::string type;
		LocateContext("Motion Type", start, end);
		m_indexBuffer = start;

		while (start != -1 && m_indexBuffer < end)
		{
			ReadWord();
			type = m_pBufferWord;
			if (type == "ROTATION_ONLY")
			{
				m_motionType = MotionType::ROTATION_ONLY;
			}
			else if (type == "FREE_MOVE"){
				m_motionType = MotionType::FREE_MOVE;
			}
			else if (type == "FOCAL_KNOWN"){
				m_focalType = FocalType::FOCAL_KNOWN;
			}
			else if (type == "FOCAL_CONSTANT"){
				m_focalType = FocalType::FOCAL_CONSTANT;
			}
			else if (type == "FOCAL_VARIABLE"){
				m_focalType = FocalType::FOCAL_VARIABLE;
			}
			else if (type == "ASPECT_KNOWN"){
				m_aspectType = AspectType::ASPECT_KNOWN;
			}
			else if (type == "ASPECT_CONSTANT"){
				m_aspectType = AspectType::ASPECT_CONSTANT;
			}
			else if (type == "ASPECT_VARIABLE"){
				m_aspectType = AspectType::ASPECT_VARIABLE;
			}
			else if (type == "PRINCIPAL_KNOWN"){
				m_principalType = PrincipalType::PRINCIPAL_KNOWN;
			}
			else if (type == "PRINCIPAL_CONSTANT"){
				m_principalType = PrincipalType::PRINCIPAL_CONSTANT;
			}
			else if (type == "PRINCIPAL_VARIABLE"){
				m_principalType = PrincipalType::PRINCIPAL_VARIABLE;
			}
			else if (type == "SKEW_KNOWN"){
				m_skewType = SkewType::SKEW_KNOWN;
			}
			else if (type == "SKEW_CONSTANT"){
				m_skewType = SkewType::SKEW_CONSTANT;
			}
			else if (type == "SKEW_VARIABLE"){
				m_skewType = SkewType::SKEW_VARIABLE;
			}
		}
	}

	void CActsFileParser::OutputMotionType(std::ofstream &fs)
	{
		if (m_motionType == FREE_MOVE)
			fs << "FREE_MOVE" << std::endl;
		else
			fs << "ROTATION_ONLY" << std::endl;

		if (m_focalType == FOCAL_KNOWN)
			fs << "FOCAL_KNOWN" << std::endl;
		else if (m_focalType == FOCAL_CONSTANT)
			fs << "FOCAL_CONSTANT" << std::endl;
		else if (m_focalType == FOCAL_VARIABLE)
			fs << "FOCAL_VARIABLE" << std::endl;

		if (m_principalType == PRINCIPAL_KNOWN)
			fs << "PRINCIPAL_KNOWN" << std::endl;
		else if (m_principalType == PRINCIPAL_CONSTANT)
			fs << "PRINCIPAL_CONSTANT" << std::endl;
		else if (m_principalType == PRINCIPAL_VARIABLE)
			fs << "PRINCIPAL_VARIABLE" << std::endl;

		if (m_skewType == SKEW_KNOWN)
			fs << "SKEW_KNOWN" << std::endl;
		else if (m_skewType == SKEW_CONSTANT)
			fs << "SKEW_CONSTANT" << std::endl;
		else if (m_skewType == SKEW_VARIABLE)
			fs << "SKEW_VARIABLE" << std::endl;
	}


	void CActsFileParser::ParseIntrinsicPara()
	{
		int start, end;
		m_indexBuffer = 0;
		LocateContext("intrinsic parameter", start, end);
		m_indexBuffer = start;
		ReadWord();
		m_fx = atof(m_pBufferWord);
		ReadWord();
		m_fy = atof(m_pBufferWord);
		ReadWord();
		m_cx = atof(m_pBufferWord);
		ReadWord();
		m_cy = atof(m_pBufferWord);
		ReadWord();
		m_skew = atof(m_pBufferWord);
		ReadWord();
		if (m_indexBuffer <= end){
			m_aspectRatio = atof(m_pBufferWord);
			m_fy = m_aspectRatio * m_fx;
		}
	}
	void CActsFileParser::ParseMatchPoint()
	{
	}

	void CActsFileParser::ParseFeatureTracks()
	{
		int start = -1, end = -1;
		m_indexBuffer = 0;
		LocateContext("Feature Tracks", start, end);
		if (start < 0 || end <0)
			return;
		m_indexBuffer = start;

		int frameStart, frameEnd;


		int trackCount;
		bool ifLoadKeypts = false;
		int dimension = 0;

		ReadWord();
		if (strcmp(m_pBufferWord, "kpts:") == 0)
		{
			ReadWord();
			dimension = atoi(m_pBufferWord);
			ifLoadKeypts = true;

			ReadWord();
			trackCount = atoi(m_pBufferWord);
		}
		else
		{
			trackCount = atoi(m_pBufferWord);
		}


		m_pMatchLinkers.clear();

		for (int index_track = 0; index_track<trackCount; index_track++)
		{
			std::shared_ptr<CMatchLinker> pSet = std::make_shared<CMatchLinker>();

			int trackLength, flag, errorLevel;
			ReadWord();
			trackLength = atoi(m_pBufferWord);
			ReadWord();
			flag = atoi(m_pBufferWord);
			ReadWord();
			errorLevel = atoi(m_pBufferWord);

			ReadWord();
			pSet->m_points_3D[0] = atof(m_pBufferWord);
			ReadWord();
			pSet->m_points_3D[1] = atof(m_pBufferWord);
			ReadWord();
			pSet->m_points_3D[2] = atof(m_pBufferWord);

			pSet->m_flag = flag;
			pSet->m_errorLevel = errorLevel;
			pSet->Reserve(trackLength);

			for (int index_point = 0; index_point<trackLength; ++index_point)
			{
				CTrackPoint &trackpt = pSet->Point(index_point);
				ReadWord();
				trackpt.m_frameNum = atoi(m_pBufferWord);
				if (m_pFramesMatchPoints[trackpt.m_frameNum] == false)
					m_pFramesMatchPoints[trackpt.m_frameNum] = std::make_shared<std::vector<CMatchPoint> >();
				trackpt.m_index = m_pFramesMatchPoints[trackpt.m_frameNum]->size();

				ReadWord();
				trackpt.m_x = atof(m_pBufferWord);
				ReadWord();
				trackpt.m_y = atof(m_pBufferWord);

				CMatchPoint matchpt;
				matchpt.m_x = trackpt.m_x;
				matchpt.m_y = trackpt.m_y;
				matchpt.m_pMatchLinker = pSet;
				m_pFramesMatchPoints[trackpt.m_frameNum]->push_back(matchpt);

			}
			pSet->m_index = m_pMatchLinkers.size();
			m_pMatchLinkers.push_back(pSet);

			if (ifLoadKeypts)
			{
				for (int index_key = 0; index_key < dimension; ++index_key)
				{
					ReadWord();
				}
			}
		}
	}

	void CActsFileParser::ParseMatchLink()
	{
	}

	void CActsFileParser::ParseCameraTrack()
	{
		int start = 0, end = 0;
		m_indexBuffer = 0;
		LocateContext("Camera Track", start, end);
		m_indexBuffer = start;
		int frameStart = 0, frameEnd = 0;

		m_extrinsicParas.resize(m_frameCount);

		char buffer[100];
		std::string frame;
		for (int index_frame = 0; index_frame<m_frameCount; index_frame++)
		{
			sprintf_s(buffer, "%d", index_frame);
			frame = std::string("FRAME") + buffer;
			LocateContext(frame.c_str(), start, end, frameStart, frameEnd);

			m_indexBuffer = frameStart;
			ReadWord();
			double scale = atof(m_pBufferWord) / m_fx;
			for (int index_r = 0; index_r<4; index_r++)
			{
				for (int index_c = 0; index_c<4; index_c++)
				{
					ReadWord();
					m_extrinsicParas[index_frame](index_r, index_c) = atof(m_pBufferWord);
				}
			}

			start = frameEnd;
		}
	}


	bool CActsFileParser::SaveProject(const std::string& fileName, const std::string &imgName, const std::vector<int> &indexs)
	{
		if (indexs.empty())
		{
			std::cerr << "There is no data to resave a act file!" << std::endl;
			return false;
		}
		std::ofstream fs(fileName);
		if (!fs){
			std::cerr << "Can not create file:" << (fileName) << std::endl;
			return false;
		}
		fs << "#camera track project file" << std::endl;
		fs << "<Image Sequence>" << std::endl;
		fs << "Sequence:" << "./"+imgName << std::endl;
		fs << "start:" << m_startFrame + m_step*indexs[0] << std::endl;
		fs << "step:" << m_step * (indexs[1] - indexs[0]) << std::endl;
		fs << "end:" << (indexs.size() - 1) * m_step * (indexs[1] - indexs[0]) << std::endl;
		fs << "</Image Sequence>\n" << std::endl;
		fs << "<Motion Type>" << std::endl;
		OutputMotionType(fs);
		fs << "</Motion Type>\n" << std::endl;

		fs << "<intrinsic parameter>" << std::endl;
		fs << std::fixed << m_fx << "\t" << m_fy << "\t" << m_cx << "\t" << m_cy << "\t" << m_skew << "\t" << m_aspectRatio << std::endl;
		fs << "</intrinsic parameter>" << std::endl;

		fs << "<Feature Tracks>" << std::endl;
		fs << std::fixed << m_pMatchLinkers.size() << std::endl;
		for (int index_track = 0; index_track<m_pMatchLinkers.size(); index_track++)
		{
			fs << std::fixed << m_pMatchLinkers[index_track]->Count() << "\t" << m_pMatchLinkers[index_track]->m_flag << "\t" << m_pMatchLinkers[index_track]->m_errorLevel << "\t"
				<< m_pMatchLinkers[index_track]->m_points_3D[0] << "\t" << m_pMatchLinkers[index_track]->m_points_3D[1] << "\t" << m_pMatchLinkers[index_track]->m_points_3D[2] << std::endl;
			for (int index_p = 0; index_p<m_pMatchLinkers[index_track]->Count(); index_p++)
			{
				fs << std::fixed << m_pMatchLinkers[index_track]->Point(index_p).m_frameNum << "\t" << m_pMatchLinkers[index_track]->Point(index_p).m_x << "\t" << m_pMatchLinkers[index_track]->Point(index_p).m_y << "\t";
			}
			fs << std::endl;
		}
		fs << "</Feature Tracks>" << std::endl;



		fs << "<Camera Track>" << std::endl;
		int oldPrecision = fs.precision(10);
		for (int index_frame = 0; index_frame < indexs.size(); ++index_frame)
		{
			fs << "<FRAME" << index_frame << ">" << std::endl;
			fs << m_fx << std::endl;

			for (int r = 0; r<4; r++){
				for (int c = 0; c<4; c++){
					fs << std::fixed << m_extrinsicParas[indexs[index_frame]](r, c) << "\t";
				}
				fs << std::endl;
			}
			fs << "</FRAME" << index_frame << ">" << std::endl;
		}
		fs << "</Camera Track>\n" << std::endl;
		fs.precision(oldPrecision);
		fs.close();
	}

	double CActsFileParser::GetAzimuth(int index_frame)
	{

		double fxyxc3;//相机姿态角构成的旋转矩阵中的9个方向余弦值中的c3(c3是相机倾角的余弦值)
		double fxyxa3, fxyxb3;//相机姿态角构成的旋转矩阵中的9个方向余弦值中的a3(a是相机倾角的正弦值乘以相机方位角的正弦值后负值)、b3(b3是相机倾角的正弦值乘以相机方位角的余弦值后负值)

		Eigen::Matrix4d& RT = m_extrinsicParas[index_frame];

		fxyxc3 = RT(2, 2);
		fxyxb3 = RT(1, 2);
		fxyxa3 = RT(0, 2);


		double fhxjqjaf = acos(fxyxc3);//c3是相机倾角的余弦值
		double fhfwjA = atan(fabs(fxyxa3 / fxyxb3));//a3/b3是方位角的正切值，这里解算的方位角是和控制点坐标系Y轴的夹角，如果Y轴是北方向，则为真正的方位角；若Y轴为东方向，则此方位角需加90度（大于360，减去360），才是真正的地理方位角

		double sinfwja = -fxyxa3 / sin(fhxjqjaf);
		double cosfwja = -fxyxb3 / sin(fhxjqjaf);



		fhfwjA = fhfwjA * (180 / M_PI);

		////根据sin(A)与cos(A)的正负号，来判断方位角A的象限 
		if (sinfwja < 0 && cosfwja < 0) fhfwjA = 180 + fhfwjA;
		if (sinfwja > 0 && cosfwja < 0) fhfwjA = 180 - fhfwjA;
		if (sinfwja < 0 && cosfwja > 0) fhfwjA = 360 - fhfwjA;


		return fhfwjA;
	}
	double CActsFileParser::GetPitch(int index_frame)
	{
		double fxyxc3;//相机姿态角构成的旋转矩阵中的9个方向余弦值中的c3(c3是相机倾角的余弦值)
		double fxyxa3, fxyxb3;//相机姿态角构成的旋转矩阵中的9个方向余弦值中的a3(a是相机倾角的正弦值乘以相机方位角的正弦值后负值)、b3(b3是相机倾角的正弦值乘以相机方位角的余弦值后负值)

		Eigen::Matrix4d& RT = m_extrinsicParas[index_frame];

		fxyxc3 = RT(2, 2);
		fxyxb3 = RT(1, 2);
		fxyxa3 = RT(0, 2);

		double fhxjqjaf = acos(fxyxc3);//c3是相机倾角的余弦值

		//再把弧度表示的相机姿态角换算成角度
		fhxjqjaf = fhxjqjaf * (180 / M_PI);

		return fhxjqjaf;
	}


}