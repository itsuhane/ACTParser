#include"FileParser.h"

#include <iostream>
#include <fstream>

namespace DFF_VideoMat
{

	CFileParser::CFileParser()
	{
		m_pBuffer = NULL;
		m_pBufferLine[0] = '\0';
		m_pBufferWord[0] = '\0';
		m_indexBuffer = 0;
	}

	CFileParser::~CFileParser()
	{
		Free();
	}

	void CFileParser::Free()
	{
		if (m_pBuffer != NULL)
		{
			delete[]m_pBuffer;
			m_pBuffer = NULL;
		}
		m_indexBuffer = 0;
	}

	int CFileParser::ReadFile(const char* filename)
	{
		m_fileName = filename;

		std::fstream infile;
		infile.open(m_fileName, std::ios::in | std::ios::binary);
		if (infile.good() == false)
		{
			std::cerr << "unable to open this file!\n";
			return 0;
		}

		infile.seekg(0, std::ios::end);
		m_sizeFile = infile.tellg();
		infile.seekg(0, std::ios::beg);

		if (m_pBuffer != NULL)
			Free();

		m_pBuffer = new char[m_sizeFile];
		if (m_pBuffer == NULL)
		{
			std::cerr << "Memory allocation failed!\n";
			return 0;
		}

		if (infile.read(m_pBuffer, m_sizeFile).fail())
		{
			std::cerr << "Error during reading!\n";
			return 0;
		}
		infile.close();

		return 1;
	}

	int CFileParser::ReadLine()
	{
		m_pBufferLine[0] = '\0';
		int L1_index = 0;
		//L1
		do
		{
			m_pBufferLine[L1_index++] = m_pBuffer[m_indexBuffer++];
		} while (m_pBuffer[m_indexBuffer - 1] != '\n' && L1_index<MAX_LINE_MS && m_indexBuffer<m_sizeFile);

		m_pBufferLine[L1_index - 1] = '\0';
		//delete the extra char '\r'
		if (L1_index - 2 >= 0 && m_pBufferLine[L1_index - 2] == '\r')
			m_pBufferLine[L1_index - 2] = '\0';
		return 1;
	}

	int CFileParser::NextLine()
	{
		while (m_indexBuffer<m_sizeFile && m_pBuffer[m_indexBuffer++] != '\n');
		if (m_indexBuffer >= m_sizeFile)
		{
			m_indexBuffer = m_sizeFile - 1;
			return 0;
		}

		int pre_indexBuffer = m_indexBuffer;
		m_pBufferLine[0] = '\0';

		int L1_index = 0;
		do
		{
			m_pBufferLine[L1_index++] = m_pBuffer[m_indexBuffer++];
		} while (m_pBuffer[m_indexBuffer - 1] != '\n' && L1_index<MAX_LINE_MS && m_indexBuffer<m_sizeFile);

		m_pBufferLine[L1_index - 1] = '\0';
		if (L1_index - 2 >= 0 && m_pBufferLine[L1_index - 2] == '\r')
			m_pBufferLine[L1_index - 2] = '\0';

		//set the buffer index as current line
		m_indexBuffer = pre_indexBuffer;

		return 1;
	}

	int CFileParser::ReadWord()
	{
		m_pBufferWord[0] = '\0';

		while ((m_pBuffer[m_indexBuffer] == '\n' || m_pBuffer[m_indexBuffer] == '\t'
			|| m_pBuffer[m_indexBuffer] == '\r' || m_pBuffer[m_indexBuffer] == ' ') && m_indexBuffer<m_sizeFile)
			m_indexBuffer++;

		if (m_indexBuffer >= m_sizeFile)
			return 0;

		int L1_index = 0;
		//L1
		do
		{
			m_pBufferWord[L1_index++] = m_pBuffer[m_indexBuffer++];
		} while (m_pBuffer[m_indexBuffer] != '\n' && m_pBuffer[m_indexBuffer] != '\t'
			&& m_pBuffer[m_indexBuffer] != '\r' && m_pBuffer[m_indexBuffer] != ' '
			&& m_indexBuffer<m_sizeFile && L1_index<MAX_WORD_MS);

		m_pBufferWord[L1_index] = '\0';

		return 1;

	}

	int CFileParser::OffsetToString(const char* str)
	{
		while (m_indexBuffer<m_sizeFile)
		{
			ReadLine();
			char* addr_str = strstr(m_pBufferLine, str);

			if (addr_str != NULL)
			{
				m_indexBuffer = m_indexBuffer - strlen(m_pBufferLine) - 1 + (addr_str - m_pBufferLine);
				return 1;

			}
		}
		return 0;
	}

	int CFileParser::OffsetToString(const char* str, int start, int end)
	{
		if (start < 0 || start >= m_sizeFile || end <= 0 || end >= m_sizeFile)
			return 0;
		m_indexBuffer = start;
		while (m_indexBuffer<m_sizeFile && m_indexBuffer <= end)
		{
			ReadLine();
			char * addr_str = strstr(m_pBufferLine, str);
			if (addr_str != NULL)
			{
				m_indexBuffer = m_indexBuffer - strlen(m_pBufferLine) - 1 + (addr_str - m_pBufferLine);
				return 1;
			}
		}

		return 0;
	}


}