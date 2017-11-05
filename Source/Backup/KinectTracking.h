#pragma once

#include <Windows.h>
#include <NuiApi.h>

class KinectTracker
{

public:

	HRESULT CreateFirstConnected();
	HRESULT StartKinectST();
	void UpdateKinectST();
	void SkeletonFrameReady(NUI_SKELETON_FRAME* pSkeletonFrame);
	void SetSkeletonPositionData(NUI_SKELETON_FRAME* pSkeletonFrame);
	float GetConvertedXY(KinectTracker* Tracker, char XorZ, int SkeletonNum);

	Vector4 LocationData[NUI_SKELETON_COUNT];
	bool SkeletonBeingTracked = false;

private:

	// Current Kinect
	INuiSensor*             m_pNuiSensor;
	HANDLE                  m_hNextSkeletonEvent;

};


