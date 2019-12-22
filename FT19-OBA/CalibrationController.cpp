#include "OBATargetMachine.h"
#include "PortsLayout.h"
#include "CalibrationController.h"
#include "CruiseController.h"
#include "TransmissionShaft.h"
#include "SafeDriveControlPanel.h"
#include <iostream>

using namespace std;

CalibrationController::CalibrationController() {
}

	static const int calibrationDefault = 5291; 
	static int calibrationValue = 5291;
	static bool isCalibrating = 0;
	static unsigned int initialPulse = 0;
	static unsigned int endPulse = 0;
	static int tempCalibrationValue = 0;

bool CC::getIsCalibrating() {
	return isCalibrating;
}

double CC::getCalibrationValue() {
	return calibrationValue;

}
