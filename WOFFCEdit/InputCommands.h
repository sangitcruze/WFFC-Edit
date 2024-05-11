#pragma once

struct InputCommands
{
	bool forward;
	bool objectMoveBackward;
	bool back;
	bool right;
	bool left;
	bool rotRight;
	bool rotLeft;
	bool objectMoveForward;
	bool deselectObjects;

	float mouseX;
	float mouseY;


	//bool LMB;
	bool RMB;


	//mouse select 
	bool mouse_LB_Down;

	//key
	bool shiftDown;
	float switchCameraMode;
	float spinCamera;
	float spinClockwise;
	float spinCounterClockwise;



};
