#ifndef CalibrationController_H
#define CalibrationController_H

class CalibrationController {
	public:
		CalibrationController();

		//static void startCalibration();
		//static void stopCalibration();
		static bool getIsCalibrating();
		static double getCalibrationValue();
};

typedef CalibrationController	CC;

#endif