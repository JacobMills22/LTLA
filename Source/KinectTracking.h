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
	HRESULT startKinectST();

/** Based on "Skeleton Basics-D2D C++ Sample" (Microsoft)
	Creates the first connected Kinect found, returns success or failure.
	Called in StartKinectST() */
	HRESULT createFirstConnected();
	
	//==============================================================================
	
	/** Kinect Update Functions */

/** Based on "Tracking Users with Kinect Skeletal Tracking" (Microsoft).
	Refreshes the Kinect and processes tracking data.
	Called on each iteration of the application's update loop.*/		 
	void updateKinectST();

/** Handles the tracking data of two skeletons.
	Stores data in (Vector4) performerData array.
	Called in UpdateKinectST().*/
	void setSkeletonPositionData(NUI_SKELETON_FRAME* pSkeletonFrame);

	//==============================================================================
						
	/** Getters and Setters */
				
/** Returns and Converts raw tracking data into more usable data. 
	Converts X&Z to X&Y Coordinates to be displayed as a 2D top down view */
	float getX(int SkeletonNum);
	float getY(int SkeletonNum);

/** Returns the tracking state of a skeleton slot */
	bool getKinectTrackingState(int SkeletonNum);

	//==============================================================================

private:

	INuiSensor* m_pNuiSensor = NULL;
	HANDLE m_hNextSkeletonEvent;
	int firstSkeleton = -1;
	int secondSkeleton = -1;

	struct TrackingData
	{
		Vector4 coordinates;
		bool trackingState = false;
	};
	TrackingData performerData[NUI_SKELETON_COUNT];

};


