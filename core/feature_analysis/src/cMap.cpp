

#include "cMap.h"

namespace MultiColSLAM
{
	cMap::cMap()
	{
		mbMapUpdated = false;
		mnMaxKFid = 0;
	}

	void cMap::AddKeyFrame(cMultiKeyFrame *pKF)
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		mspKeyFrames.insert(pKF);
		if (pKF->mnId > mnMaxKFid)
			mnMaxKFid = pKF->mnId;
		mbMapUpdated = true;
	}

	void cMap::AddMapPoint(cMapPoint *pMP)
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		mspMapPoints.insert(pMP);
		mbMapUpdated = true;
	}

	void cMap::EraseMapPoint(cMapPoint *pMP)
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		mspMapPoints.erase(pMP);
		mbMapUpdated = true;
	}

	void cMap::EraseKeyFrame(cMultiKeyFrame *pKF)
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		mspKeyFrames.erase(pKF);
		mbMapUpdated = true;
	}

	void cMap::SetReferenceMapPoints(const std::vector<cMapPoint*> &vpMPs)
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		mvpReferenceMapPoints = vpMPs;
		mbMapUpdated = true;
	}

	std::vector<cMultiKeyFrame*> cMap::GetAllKeyFrames()
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		return std::vector<cMultiKeyFrame*>(mspKeyFrames.begin(), mspKeyFrames.end());
	}

	std::vector<cMapPoint*> cMap::GetAllMapPoints()
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		return std::vector<cMapPoint*>(mspMapPoints.begin(), mspMapPoints.end());
	}

	int cMap::MapPointsInMap()
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		return mspMapPoints.size();
	}

	int cMap::KeyFramesInMap()
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		return mspKeyFrames.size();
	}

	std::vector<cMapPoint*> cMap::GetReferenceMapPoints()
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		return mvpReferenceMapPoints;
	}

	bool cMap::isMapUpdated()
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		return mbMapUpdated;
	}

	void cMap::SetFlagAfterBA()
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		mbMapUpdated = true;
	}

	void cMap::ResetUpdated()
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		mbMapUpdated = false;
	}

	unsigned int cMap::GetMaxKFid()
	{
		std::unique_lock<std::mutex> lock(mMutexMap);
		return mnMaxKFid;
	}

	void cMap::clear()
	{
		for (std::set<cMapPoint*>::iterator sit = mspMapPoints.begin(), send = mspMapPoints.end();
			sit != send; ++sit)
			delete *sit;

		for (std::set<cMultiKeyFrame*>::iterator sit = mspKeyFrames.begin(), send = mspKeyFrames.end();
			sit != send; ++sit)
			delete *sit;

		mspMapPoints.clear();
		mspKeyFrames.clear();
		mnMaxKFid = 0;
		mvpReferenceMapPoints.clear();
	}
}