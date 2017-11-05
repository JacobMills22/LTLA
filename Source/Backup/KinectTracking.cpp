
#include "KinectTracking.h"

// Call StartKinectST once at application start.
HRESULT KinectTracker::StartKinectST()
{
	for (int i = 0; i < NUI_SKELETON_COUNT; ++i)
	{
		LocationData[i].x = 0.00;
		LocationData[i].y = 0.00;
	}

	m_hNextSkeletonEvent = NULL;

	// Initialize m_pNuiSensor
	HRESULT hr = CreateFirstConnected();
	if (SUCCEEDED(hr))
	{
		// Initialize the Kinect and specify that we'll be using skeleton
		hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
		if (SUCCEEDED(hr))
		{
			// Create an event that will be signaled when skeleton data is available
			m_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

			// Open a skeleton stream to receive skeleton data
			hr = m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0);
		}
	}
	return hr;
}

// Call UpdateKinectST on each iteration of the application's update loop.
void KinectTracker::UpdateKinectST()
{
	// Wait for 0ms, just quickly test if it is time to process a skeleton
	if (WAIT_OBJECT_0 == WaitForSingleObject(m_hNextSkeletonEvent, 0))
	{
		NUI_SKELETON_FRAME skeletonFrame = { 0 };

		// Get the skeleton frame that is ready
		if (SUCCEEDED(m_pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame)))
		{
			// Process the skeleton frame
			SkeletonFrameReady(&skeletonFrame);
			SetSkeletonPositionData(&skeletonFrame);
		}
	}
}

// Called in StartKinect
HRESULT KinectTracker::CreateFirstConnected()
{
	INuiSensor * pNuiSensor;

	int iSensorCount = 0;
	HRESULT hr = NuiGetSensorCount(&iSensorCount);
	if (FAILED(hr))
	{
		return hr;
	}

	// Look at each Kinect sensor
	for (int i = 0; i < iSensorCount; ++i)
	{
		// Create the sensor so we can check status, if we can't create it, move on to the next
		hr = NuiCreateSensorByIndex(i, &pNuiSensor);
		if (FAILED(hr))
		{
			continue;
		}

		// Get the status of the sensor, and if connected, then we can initialize it
		hr = pNuiSensor->NuiStatus();
		if (S_OK == hr)
		{
			m_pNuiSensor = pNuiSensor;
			break;
		}

		// This sensor wasn't OK, so release it since we're not using it
		pNuiSensor->Release();
	}

	if (NULL != m_pNuiSensor)
	{
		// Initialize the Kinect and specify that we'll be using skeleton
		hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
		if (SUCCEEDED(hr))
		{
			// Create an event that will be signaled when skeleton data is available
			m_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

			// Open a skeleton stream to receive skeleton data
			hr = m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0);
		}
	}

	if (NULL == m_pNuiSensor || FAILED(hr))
	{
		//SetStatusMessage(L"No ready Kinect found!");
		return E_FAIL;
	}

	return hr;
}

// Called in UpdateKinectST
void KinectTracker::SkeletonFrameReady(NUI_SKELETON_FRAME* pSkeletonFrame)
{	
	for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	{
		const NUI_SKELETON_DATA & skeleton = pSkeletonFrame->SkeletonData[i];

		switch (skeleton.eTrackingState)
		{
		case NUI_SKELETON_TRACKED:
		//	DrawTrackedSkeletonJoints(skeleton);
			break;

		case NUI_SKELETON_POSITION_ONLY:
		//	DrawSkeletonPosition(skeleton.Position);
			break;
		}
	}	
}

// Called in UpdateKinectST
void KinectTracker::SetSkeletonPositionData(NUI_SKELETON_FRAME* pSkeletonFrame)
{
	SkeletonBeingTracked = false;

	for (int i = 0; i < NUI_SKELETON_COUNT; ++i)
	{
		if (pSkeletonFrame->SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
		{
			LocationData[i] = pSkeletonFrame->SkeletonData[i].Position;
			SkeletonBeingTracked = true;
		}
		else if (pSkeletonFrame->SkeletonData[i].eTrackingState == NUI_SKELETON_NOT_TRACKED)
		{
			LocationData[i].x = 0.00;
			LocationData[i].z = 0.00;
		}
	}	
}

float KinectTracker::GetConvertedXY(KinectTracker* Tracker, char XorY, int SkeletonNum)
{
	if (XorY == 'X')
	{
		return  Tracker->LocationData[SkeletonNum].x + 1.0;
	}
	else if (XorY == 'Y')
	{
		return  Tracker->LocationData[SkeletonNum].z - 1.0;
	}
}





