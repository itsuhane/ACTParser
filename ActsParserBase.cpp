#include "ActsParserBase.h"

namespace DFF_VideoMat
{

	CActsParserBase::CActsParserBase() :m_frameCount(0){}
	CActsParserBase::~CActsParserBase(){}

	void CActsParserBase::clear()
	{
		m_frameCount = 0;

		m_extrinsicParas.clear();
		m_pFramesMatchPoints.clear();
		m_pMatchLinkers.clear();

		m_extrinsicParas.shrink_to_fit();
		m_pFramesMatchPoints.shrink_to_fit();
		m_pMatchLinkers.shrink_to_fit();
	}


	bool CActsParserBase::SaveProject(const std::string &filename, const std::string &imageName)
	{
		std::vector<int> indexs(m_frameCount);

		for (int i = 0; i<m_frameCount; ++i)
			indexs[i] = i;

		return SaveProject(filename, imageName, indexs);
	}
}

