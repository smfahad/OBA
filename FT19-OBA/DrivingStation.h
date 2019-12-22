#ifndef DrivingStation_H
#define DrivingStation_H

class DrivingStation {
	public:
		DrivingStation();
		static unsigned int getEngagedGearState();
		static bool getIgnitionState();
		static bool getClutchState();
		static bool getBrakeState();
		static bool getAcceleratorState();
		static void handleDSTAinterrupt(DrivingStationInterruptStatusWord*);
};

typedef DrivingStation	DS;

#endif