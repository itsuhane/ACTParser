#pragma once
#include<string>

namespace DFF_VideoMat
{
#define MAX_LINE_MS 10000
#define MAX_WORD_MS 1000

	class CFileParser
	{
	public:
		CFileParser();
		virtual ~CFileParser();
	protected:
		int m_sizeFile;//The size of file 
		char *m_pBuffer;//the buffer to store the whole file words
		int m_indexBuffer;//the index of the current word which to be read 
		std::string m_fileName;//the name of of the input file
		char m_pBufferLine[MAX_LINE_MS];//the buffer to store the input line
		char m_pBufferWord[MAX_WORD_MS];//the buffer to store the input word

	public:
		void Free();//free the memory which has been allocated
		virtual int ReadFile(const char* filename);//read the input file
		int ReadLine();//get the current line in the buffer
		int NextLine();//get the next line in the buffer,but the index of the buffer still point at the beginning of this line
		int ReadWord();//get the current word in the buffer
		int OffsetToString(const char* string);//find the nearest matched string from the current position
		int OffsetToString(const char* string, int start, int end);//find the nearest matched string from the start to the end

	};



}