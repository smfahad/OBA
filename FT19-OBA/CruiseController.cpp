#include "OBATargetMachine.h"
#include "PortsLayout.h"
#include "CruiseController.h"
#include "TransmissionShaft.h"
#include "Throttle.h"
#include "DrivingStation.h"
#include "SafeDriveControlPanel.h"
#include "CalibrationController.h"

void displayActivateACFirstMessage() {
	SDCP::displayMessage((char*)"Activate AC First         ");
}

void displayAccelerateFirstMessage() {
	SDCP::displayMessage((char*)"Accelerate First          ");
}

void displayACAlreadyActiveMessage() {
	SDCP::displayMessage((char*)"AC Already Active          ");
}
void displayCannotActivateACMessage() {
	SDCP::displayMessage((char*)"Cannot Activate AC         ");
}

CruiseController::CruiseController() {
}

	static bool isActive		= 0;
	static bool isSuspended		= 0;
	static int cruisingSpeed	= 0;
	static bool isAccelerating	= 0;
	static int throttleRate		= 0;

void CRUISE::activateCruising(int ifResume) {
	
	if (validateCruisingRequest()) {

		if (ifResume) {
			cruisingSpeed = TS::getCurrentSpeed();
			throttleRate = THRO::getThrottleResponse();
		}

		THRO::setMaintainSpeed(cruisingSpeed);
		isActive = 1;
		isSuspended = 0;
		isAccelerating = 0;
		SDCP::sendLEDsignal(1, 1);
		SDCP::displayMessage(cruisingSpeed);
	}
	else {
		displayCannotActivateACMessage();
	}

}

void CRUISE::stopCruisingOnDeactivation() {
	if (isActive) {
		CRUISE::stopCruising();
	}
	else {
		displayActivateACFirstMessage();
	}
}

void CRUISE::stopCruising() {
	if (isActive) {
		cruisingSpeed = 0;
		isActive = 0;
		isSuspended = 0;
		SDCP::sendLEDsignal(1, 0);
		SDCP::displayMessage((char*)"AC Deactivated            ");
		THRO::setMaintainSpeed(0);
	}
}

void CRUISE::startAccelerationRequest() {
	if (isActive && !isSuspended) {
		isAccelerating = 1;
		suspendCruising();
		THRO::setMaintainSpeed(200);
		SDCP::displayMessage((char*)"Accelerating               ");
	}else {
		displayActivateACFirstMessage();
	}
}

void CRUISE::suspendCruising() {
	if (isActive) {
		isSuspended = 1;
		THRO::setMaintainSpeed(0);
		SDCP::displayMessage((char*)"AC Suspended              ");
	}
}

void CRUISE::stopAccelerationRequest() {
	if (isActive) {
		if (isAccelerating || DS::getAcceleratorState()) {
			isAccelerating = 0;
			activateCruising(101);
		} else {
			displayAccelerateFirstMessage();
		}
	}
	else {
		displayActivateACFirstMessage();
	}
}


void CRUISE::maxSpeedReached() {
	if (isActive) {
		THRO::setMaintainSpeed(cruisingSpeed);
		isActive = 1;
		isSuspended = 0;
		isAccelerating = 0;
		SDCP::sendLEDsignal(1, 1);
		SDCP::displayMessage(cruisingSpeed);
	}
}

void CRUISE::resumeCruising() {
	if (isActive && !isSuspended) {
		displayACAlreadyActiveMessage();
	} else if (isSuspended) {
		if (validateCruisingRequest()) {
			activateCruising(0);
		}
		else {
			displayCannotActivateACMessage();
		}
	} else {
		displayActivateACFirstMessage();
	}
}

bool CRUISE::validateCruisingRequest() {
	
	int currentSpeed = TS::getCurrentSpeed();

	if ( ! CC::getIsCalibrating() && DS::getIgnitionState() && 
		 ! DS::getBrakeState() && ! DS::getClutchState() &&
		   DS::getEngagedGearState()==5) {

			if (currentSpeed >= 80 && currentSpeed <= 170) {
				return true;
			}		
			else return false;
	}
	else return false;
}


bool CRUISE::getIsActive() {
	return isActive;
}

bool CRUISE::getIsAccelerating() {
	return isAccelerating;
}

bool CRUISE::FisSuspended() {
	return isSuspended;
}

int CRUISE::getCruisingSpeed() {
	return cruisingSpeed;
}

void CRUISE::setCruisingSpeed(int cs) {
	cruisingSpeed = cs;
}

int CRUISE::getThrottleRate() {
	return throttleRate;
}

void CRUISE::increaseThrottleRate() {
	if (throttleRate <= 72) {
		throttleRate = throttleRate + 8;
	} else {
		throttleRate = 80;
	}
}

void CRUISE::decreaseThrottleRate() {
	if (throttleRate > 8) {
		throttleRate = throttleRate - 8;
	}
	else {
		throttleRate = 0;
	}
}

void CRUISE::setThrottleRate(int rate) {
	throttleRate = rate;
}