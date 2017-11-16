
#include "KinectTracking.h"

// Call StartKinectST once at application start.
HRESULT KinectTracker::StartKinectST()
{
	// Initialise all Location Data.
	for (int i = 0; i < NUI_SKELETON_COUNT; ++i)
	{
		PerformerData[i].Coordinates.x = 0.00;
		PerformerData[i].Coordinates.y = 0.00;
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
			SetSkeletonPositionData(&skeletonFrame);
		}
	}
}

// Called in StartKinectST
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
void KinectTracker::SetSkeletonPositionData(NUI_SKELETON_FRAME* pSkeletonFrame)
{
	// Since the kinect can identify 6 skeletons, the application needs to test each skeleton slot.
	for (int CurrentSkel = 0; CurrentSkel < NUI_SKELETON_COUNT; ++CurrentSkel)
	{
		// If the current slot is tracking a skeleton.
		if (pSkeletonFrame->SkeletonData[CurrentSkel].eTrackingState == NUI_SKELETON_TRACKED)
		{
			// If the first performer was not being tracked, and the current slot isn't being used by the second perfromer.
			if (PerformerData[0].TrackingState == false && CurrentSkel != SecondSkeleton)
			{
				FirstSkeleton = CurrentSkel; // Store the slot number in FirstSkeleton
				PerformerData[0].TrackingState = true; // Set the first performers tracking state.
			}
			// If the second performer was not being tracked, and the current slot isn't being used by the first perfromer.
			if (PerformerData[1].TrackingState == false && CurrentSkel != FirstSkeleton)
			{
				SecondSkeleton = CurrentSkel; // Store the slot number in SecondSkeleton
				PerformerData[1].TrackingState = true; // Set the Second performers tracking state.
			}
			// Store coordinates for both performers, taken from the currently tracked slots. 
			PerformerData[0].Coordinates = pSkeletonFrame->SkeletonData[FirstSkeleton].Position;
			PerformerData[1].Coordinates = pSkeletonFrame->SkeletonData[SecondSkeleton].Position;
		}
		// Else if the current slot is not tracking a skeleton.
		else if (pSkeletonFrame->SkeletonData[CurrentSkel].eTrackingState == NUI_SKELETON_NOT_TRACKED)
		{	
			// If the slot is being used to track the first performer
			if (CurrentSkel == FirstSkeleton)
			{
				PerformerData[0].TrackingState = false;
				FirstSkeleton = -1;
			}
			// If the slot is being used to track the second performer
			else if (CurrentSkel == SecondSkeleton)
			{
				PerformerData[1].TrackingState = false; 
				SecondSkeleton = -1; 
			}
		}
	}
}

float KinectTracker::GetX(int SkeletonNum)
{
	return PerformerData[SkeletonNum].Coordinates.x + 1.0;
}
float KinectTracker::GetY(int SkeletonNum)
{
	return PerformerData[SkeletonNum].Coordinates.z - 1.0;
}

bool KinectTracker::GetKinectTrackingState(int SkeletonNum)
{
	return PerformerData[SkeletonNum].TrackingState;
}
