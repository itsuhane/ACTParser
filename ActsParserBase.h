#pragma once
#include <string>
//#include <WmlMatrix4.h>
#include "Eigen/Eigen"
#include <vector>
#include <memory>


namespace DFF_VideoMat
{

	class CMatchPoint;
	class CMatchLinker;

	class CActsParserBase
	{
	public:
		CActsParserBase();
		virtual ~CActsParserBase();

		virtual bool LoadProject(const std::string &filename) = 0;
		virtual bool SaveProject(const std::string &filename, const std::string &imageName);
		virtual bool SaveProject(const std::string &filename, const std::string &imageName, const std::vector<int> & indexs) = 0;
		void clear();

	public:
		enum MotionType{ FREE_MOVE, ROTATION_ONLY };
		enum FocalType{ FOCAL_KNOWN, FOCAL_CONSTANT, FOCAL_VARIABLE };
		enum AspectType{ ASPECT_KNOWN, ASPECT_CONSTANT, ASPECT_VARIABLE };
		enum PrincipalType{ PRINCIPAL_KNOWN, PRINCIPAL_CONSTANT, PRINCIPAL_VARIABLE };
		enum SkewType{ SKEW_KNOWN, SKEW_CONSTANT, SKEW_VARIABLE };


		//图片序列信息
		std::string m_imgPath;
		int m_startFrame;
		int m_step;
		int m_endFrame;
		int m_frameCount;

		//参数模型信息
		MotionType m_motionType;
		FocalType m_focalType;
		AspectType m_aspectType;
		PrincipalType m_principalType;
		SkewType m_skewType;

		//相机内参
		double m_fx, m_fy;
		double m_cx, m_cy;
		double m_skew;
		double m_aspectRatio;

		//相机外参，以及特征轨迹信息
		std::vector<Eigen::Matrix4d, Eigen::aligned_allocator<Eigen::Matrix4d>> m_extrinsicParas;//the vector stores the extrinsic parameters of all the frames
		std::vector<std::shared_ptr<std::vector<CMatchPoint>>> m_pFramesMatchPoints;//the vector stores all the frames' vectors of the match points in one frame
		std::vector<std::shared_ptr<CMatchLinker>> m_pMatchLinkers;//the vector stores all the tracks

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

}