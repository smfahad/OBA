#include <iostream>
#include "OBATargetMachine.h"
#include "PortsLayout.h"
#pragma comment(lib, "OBAVirtualTarget.lib")

#include "TransmissionShaft.h"
#include "Throttle.h"
#include "SafeDriveControlPanel.h"
#include "DrivingStation.h"
#include "CruiseController.h"
#include "MaintenanceController.h"

using namespace std;

static const unsigned int	periodInMillisecond = 1000; 

static unsigned int tick = 4;

void myInterruptHandler (Interrupt sig) {
	
	ControlPanelInterruptStatusWord *keyId = (ControlPanelInterruptStatusWord *) (OTM::controlPanelInterruptWordAddress);
	SafeDriveControlPanel::KEYMAP pressedKey;
	DrivingStationInterruptStatusWord *eventDSTA = (DrivingStationInterruptStatusWord *) (OTM::drivingControlsInterruptWordAddress);

	switch (sig)
	{
	case IT_controlPanel: 
		SafeDriveControlPanel::keyPressHandler(keyId->keyCode);
		break;

	case IT_drivingControls:
		DS::handleDSTAinterrupt(eventDSTA);
		break;

	case IT_timer:
			TS::calculateSpeed();
			if (CRUISE::getIsActive() == 1) {
				THRO::maintainSpeed();
			}

			MC::maintenanceRoutine(tick--);

			if (tick==0) {
				tick = 4;
			}
		break;

	default :
	cout << "handler " << sig <<endl;
	}
}


int main() {

try {
		OTM::interruptVector[IT_controlPanel] = myInterruptHandler;
        OTM::interruptVector[IT_drivingControls] = myInterruptHandler;
		OTM::interruptVector[IT_timer] = myInterruptHandler;

		OTM::setPeriodicTimer(periodInMillisecond, 10);

        OTM::enabledInterruptsRegister = allObaRelatedInterrupts;
		SDCP::displayMessage((char*)"Avazona Solutions        ");
		SDCP::sendLEDsignal(1, 0);
		SDCP::sendLEDsignal(2, 0);
		SDCP::sendLEDsignal(3, 0);
        for (;;) {
        }
   }

   catch (const char *const message) {
      throw;
   }

}
