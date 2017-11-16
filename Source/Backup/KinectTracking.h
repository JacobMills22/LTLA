#pragma once

#include <Windows.h>
#include <NuiApi.h>

//==============================================================================	
/**
	Class to handle all Kinect V1 tracking related processes.
	Assumes the Kinect will be placed either facing the audience/stage.
	Tracking data will be converted from X and Z to a top down view (XY).
*/
//==============================================================================	

class KinectTracker
{

public:
	//==============================================================================	

	/** Kinect Initialisation Functions */

/** Based on "Tracking Users with Kinect Skeletal Tracking" (Microsoft).
    Initialises kinect and open a skeleton stream.
	Call once at application start. */
	HRESULT StartKinectST();

/** Based on "Skeleton Basics-D2D C++ Sample" (Microsoft)
	Creates the first connected Kinect found, returns success or failure.
	Called in StartKinectST() */
	HRESULT CreateFirstConnected();
	
	//==============================================================================
	
	/** Kinect Update Functions */

/** Based on "Tracking Users with Kinect Skeletal Tracking" (Microsoft).
	Refreshes the Kinect and processes tracking data.
	Called on each iteration of the application's update loop.*/		 
	void UpdateKinectST();

/** Handles the tracking data of two skeletons.
	Stores data in (Vector4) LocationData array.
	Called in UpdateKinectST().*/
	void SetSkeletonPositionData(NUI_SKELETON_FRAME* pSkeletonFrame);

	//==============================================================================
						
	/** Getters and Setters */
				
/** Returns and Converts raw tracking data into more usable data. 
	Converts X&Z to X&Y Coordinates to be displayed as a 2D top down view */
	float GetX(int SkeletonNum);
	float GetY(int SkeletonNum);

/** Returns the tracking state of a skeleton slot */
	bool GetKinectTrackingState(int SkeletonNum);

	//==============================================================================

private:

	INuiSensor* m_pNuiSensor;
	HANDLE m_hNextSkeletonEvent;
	int FirstSkeleton = -1;
	int SecondSkeleton = -1;

	struct TrackingData
	{
		Vector4 Coordinates;
		bool TrackingState = false;
	};
	TrackingData PerformerData[NUI_SKELETON_COUNT];

};


