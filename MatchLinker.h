#pragma once
#include <array>
#include <vector>
#include "Eigen/Eigen"

namespace DFF_VideoMat
{


	/**
	* class that defines one track point.
	*/
	class CTrackPoint
	{
	public:
		int m_frameNum;//the frame number which this track point belongs to
		int m_index;//the index of the track which this point belongs to
		double m_x;
		double m_y;
	};

	/**
	* class that defines one track.
	*/
	class CMatchLinker
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	public:
		CTrackPoint &Point(size_t index)
		{
			if (index <m_pTrackList.size())
				return m_pTrackList[index];

			std::cerr << "Error:CTrackPoint::index is out of range!\n";

			return CTrackPoint();
		}

		void Reserve(int num)
		{
			m_pTrackList.resize(num);
		}

		size_t Count()
		{
			return m_pTrackList.size();
		}

	public:
		int m_flag;
		int m_index;
		int m_errorLevel;
		Eigen::Vector3d m_points_3D;

	protected:
		std::vector<CTrackPoint> m_pTrackList;//store the track points in one track

	};

	/**
	* class that gets the track pointer with one point in this track.
	*/

	class CMatchPoint
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	public:
		double m_x;
		double m_y;
		std::shared_ptr<CMatchLinker> m_pMatchLinker;

	};

}
