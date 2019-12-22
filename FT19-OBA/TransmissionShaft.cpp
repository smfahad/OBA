#include "OBATargetMachine.h"
#include "PortsLayout.h"
#include "TransmissionShaft.h"
#include "SafeDriveControlPanel.h"
#include "CalibrationController.h"
#include "CruiseController.h"
#include "DrivingStation.h"
#include "Logger.h"

#include <iostream>
using namespace std;

TransmissionShaft::TransmissionShaft() {

}

static double calibrationReference = 5291.0053;
static unsigned int lastpulse = 0;
static double speed = 0;
static unsigned int speedInt = 0;

void  TS::calculateSpeed() {

	int pulse = getCurrentPulse();

	speed = ((pulse - lastpulse)/calibrationReference)*3600;
	int acSpeed = CRUISE::getCruisingSpeed();
	LOG::LogStart();
	LOG::Log("Current Speed: ");
	LOG::Log((int)speed);
	LOG::Log(" -- AC Speed: ");
	LOG::Log(acSpeed);
	LOG::Log(" -- Pulse: ");
	LOG::Log(getCurrentPulse());
	LOG::Log(" -- Last Pulse: ");
	LOG::Log(lastpulse);

	speedInt = static_cast<unsigned int>(((pulse - lastpulse)/calibrationReference)*3600);
	lastpulse = pulse;
	
	int thresholdSpeedLimit = acSpeed+10;

	if (CRUISE::getIsActive()) {
		if (CRUISE::getIsAccelerating() || DS::getAcceleratorState()) {
			// Care To Speed
			if (speed >= thresholdSpeedLimit) {
				SDCP::sendLEDsignal(2, 1);	
			}
			else {
				SDCP::sendLEDsignal(2, 0);	
			}
			SDCP::sendLEDsignal(3, 0);	
		} else {
			SDCP::sendLEDsignal(2, 0);	
			// Hazard Bolting
			if (speed >= thresholdSpeedLimit) {
				SDCP::sendLEDsignal(3, 1);	
			}
			else {
				SDCP::sendLEDsignal(3, 0);	
			}
		}
	}else {
		// Out of Auto Cruise
		SDCP::sendLEDsignal(2, 0);	
		SDCP::sendLEDsignal(3, 0);	
	}
}


unsigned int TS::getCurrentPulse() {
	CounterWord *pulse;
	pulse = (CounterWord *) (OTM::pulseCounterWordAddress);
	return pulse->pulsecount;
}


double TS::getCurrentSpeed() {
	return speed;
}

unsigned int TS::getCurrentSpeedInt() {
	return speedInt;
}

void TS::setCalibrationReference(double calibrationOverride) {
	calibrationReference = calibrationOverride;
}

unsigned int TS::getCalibrationReference() {
	return static_cast<unsigned int>(calibrationReference);
}