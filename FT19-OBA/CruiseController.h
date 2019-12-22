#ifndef CruiseController_H
#define CruiseController_H

class CruiseController {
	public:
		CruiseController();

		static void activateCruising(int ifZeroisForResume);
		static void stopCruising();
		static void stopCruisingOnDeactivation();
		static void startAccelerationRequest();
		static void stopAccelerationRequest();
		static void maxSpeedReached();
		static void suspendCruising();
		static void resumeCruising();
		static bool validateCruisingRequest();

		static void setCruisingSpeed(int cs);
		static bool getIsActive();
		static bool getIsAccelerating();
		static bool FisSuspended(); 
		static int getCruisingSpeed();
		static int getThrottleRate();
		static void setThrottleRate(int rate);
		static void increaseThrottleRate();
		static void decreaseThrottleRate();
		

};

typedef CruiseController	CRUISE;

#endif