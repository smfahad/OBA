#define _CRT_SECURE_NO_WARNINGS
#define  _NO_CRT_STDIO_INLINE

#include "OBATargetMachine.h"
#include "PortsLayout.h"
#include "SafeDriveControlPanel.h"
#include "Throttle.h"
#include "CruiseController.h"
#include "CalibrationController.h"
#include "MaintenanceController.h"
#include <string>
#include <cstring>


static bool isCareToSpeedLED = false;
static bool isHazardBoltingLED = false;


SafeDriveControlPanel::SafeDriveControlPanel() {

}

bool SDCP::getIsCareToSpeedLEDLit() {
	return isCareToSpeedLED;
}


bool SDCP::getIsHazardBoltingLEDLit() {
	return isHazardBoltingLED;
}

enum KEYMAP {
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9, 
		KEY_ACTIVATION,
		KEY_DEACTIVATION,
		KEY_RESUME,
		KEY_START_ACCELERATION,
		KEY_STOP_ACCELERATION,
		KEY_START_CALIBRATION,
		KEY_STOP_CALIBRATION,
		KEY_TRIP_START,
		KEY_FUEL_QUANTITY,
		KEY_AVERAGE_SPEED,
		KEY_SERVICE_COMPLETED,
		KEY_VALIDATION,
		KEY_CANCEL,
		KEY_AVERAGE_CONSUMPTION,
		KEY_DECIMAL
	};


void SDCP::sendLEDsignal(int ledNumber, bool ledBool) {
	
	LedCommandWord *led;
	led =  (LedCommandWord *)(OTM::ledCommandByteAddress);

	//assign/override accordingly
	if (ledNumber == 1) {
		led -> led_1 = ledBool;	//left   // green	// on/off
	}

	if (ledNumber == 2) {
		led -> led_2 = ledBool;	//center // yellow	// care to speed
		isCareToSpeedLED = ledBool;
	}

	if (ledNumber == 3) {
		led -> led_3 = ledBool;	//right  // red		// hazard bolting
		isHazardBoltingLED = ledBool;
	}

}


void SDCP::displayMessage(char message[]) {
	static  char *msg;
	//static	char msg1[] = "Hello World!             ";
	DisplayBufferAddress	 *display;
	display =  (DisplayBufferAddress*)(OTM::displayBufferAddress);

	msg=message;

	for (int i=0;i<20;i++)
		display->displaymessage[i]=msg[i];
}

void SDCP::displayMessage(int speed) {
	
	DisplayBufferAddress	 *display;
	display =  (DisplayBufferAddress*)(OTM::displayBufferAddress);

	std::string str1 = "AC Speed " + std::to_string(static_cast<long long>(speed));
	std::string str2 = str1 + " km/h         ";

  	char * cstr = new char [str2.length()+1];
  	std::strcpy (cstr, str2.c_str());
	static  char *msg;
	msg=cstr;

	for (int i=0;i<20;i++)
		 display->displaymessage[i]=msg[i];
}


void SDCP::keyPressHandler(int pressedKey) {

	KEYMAP keyEnum = static_cast<SDCP::KEYMAP>(pressedKey);

	switch (keyEnum) {

	case KEY_ACTIVATION:
		CRUISE::activateCruising(101);
		break;

	case KEY_DEACTIVATION:
		CRUISE::stopCruisingOnDeactivation();
		break;

	case KEY_START_ACCELERATION:
		CRUISE::startAccelerationRequest();
		break;

	case KEY_STOP_ACCELERATION:
		CRUISE::stopAccelerationRequest();
		break;

	case KEY_RESUME:
		CRUISE::resumeCruising();
		break;

	case KEY_SERVICE_COMPLETED:
		MC::serviceDoneHandler();
		break;

	default :
		break;
	}

}
