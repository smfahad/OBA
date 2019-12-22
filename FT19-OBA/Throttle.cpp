#include "OBATargetMachine.h"
#include "PortsLayout.h"
#include "Throttle.h"
#include "TransmissionShaft.h"
#include "CruiseController.h"
#include "Logger.h"
#include "SafeDriveControlPanel.h"
#include <iostream>
using namespace std;

Throttle::Throttle() {

}
	//static unsigned int throttleCounter = 0;
	static bool isMaintaining = 0;
	static unsigned int speedToMaintain = 0;
	static unsigned int increaseCounter = 0;
	static unsigned int descreaseCounter = 0;
	static bool manageMaxSpeed = 0;

void THRO::sendThrottleSignal(unsigned int voltage) {
	ThrottleCommandWord *throttle;
	throttle = (ThrottleCommandWord *)(OTM::throttleCommandWordAddress);
	throttle -> outputvoltagevalue = voltage;
	throttle -> paritybit = getOddParityBit(voltage);
}


unsigned int THRO::getThrottleResponse() {
	ThrottleResponseWord *throttle;
	throttle = (ThrottleResponseWord *)(OTM::throttlePositionWordAddress);
	return throttle->inputvoltagevalue;
}


bool THRO::getOddParityBit(unsigned int x) {
	x = ((x >> 1) & 0x55555555) + (x & 0x55555555);
	x = ((x >> 2) & 0x33333333) + (x & 0x33333333);
	x = ((x >> 4) & 0x0F0F0F0F) + (x & 0x0F0F0F0F);
	x = ((x >>  8 ) + x);
	x = ((x >>  16 ) + x);
	return !(x&0x01); //return odd parity bit
}


void THRO::setMaintainSpeed(int speed) {
		isMaintaining = 1;
		speedToMaintain = speed;

		if (speed == 0) {
			isMaintaining = 0;
			speedToMaintain = 0;
			sendThrottleSignal(0);
		}
}

void THRO::maintainSpeed() {
	int currentSpeed = (int) TS::getCurrentSpeed();

	int currentThrottleRate = THRO::getThrottleResponse();
	LOG::Log(" -- ThrottleRate: ");
	LOG::Log(currentThrottleRate);

	if (currentSpeed < 80) {
		if (CRUISE::getIsActive() && currentSpeed >=80) {
			CRUISE::increaseThrottleRate();
			sendThrottleSignal(CRUISE::getThrottleRate());
			LOG::Log(" -- INC Speed");
		} else {
			CRUISE::stopCruisingOnDeactivation();
		}
	}
	else if (currentSpeed >= 80 && currentSpeed <=170) {
		LOG::Log(" Speed <= 170");
		if (currentSpeed < (speedToMaintain)) {
			//sendThrottleSignal(80);
			//if (currentThrottleRate <= 72) {
				CRUISE::increaseThrottleRate();
			//}
			sendThrottleSignal(CRUISE::getThrottleRate());
			LOG::Log(" -- INC Speed");

		} else if (currentSpeed > (speedToMaintain))  {
			if (SDCP::getIsHazardBoltingLEDLit() || (currentSpeed > (speedToMaintain+5))) {
				sendThrottleSignal(40);
				CRUISE::setThrottleRate(40);
			} else if (SDCP::getIsHazardBoltingLEDLit() || (currentSpeed > (speedToMaintain+8))) {
				sendThrottleSignal(0);
				CRUISE::setThrottleRate(0);
			}
			else {
				CRUISE::decreaseThrottleRate();
				sendThrottleSignal(CRUISE::getThrottleRate());
			}
			LOG::Log(" -- DEC Speed");
		}
		manageMaxSpeed = 0;
	} 
	else if (CRUISE::getIsAccelerating() || manageMaxSpeed)
	{
		sendThrottleSignal(0);
		if (manageMaxSpeed == 0) {
			manageMaxSpeed = 1;
			CRUISE::setCruisingSpeed(170);
			CRUISE::maxSpeedReached();			
			LOG::Log(" -- Speed > 170 while ACC");
		}
		else { 
			manageMaxSpeed = 1;
			CRUISE::maxSpeedReached();
			LOG::Log(" -- Speed > 170 waiting to settle ACC");
		}
	}
	else {
		CRUISE::stopCruisingOnDeactivation();
		LOG::Log(" -- Speed > 170 while not ACC");
		sendThrottleSignal(0);
	}
}