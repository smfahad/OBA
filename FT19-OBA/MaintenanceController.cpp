#include "OBATargetMachine.h"
#include "PortsLayout.h"
#include "MaintenanceController.h"
#include "TransmissionShaft.h"
#include "SafeDriveControlPanel.h"
#include "Logger.h"
#include <iostream>

using namespace std;

MaintenanceController::MaintenanceController() {
}

	enum MAINT_MSG {
		EMPTY,
		OIL_REMIND,		
		AIR_REMIND,
		AIR_OIL_REMIND,
		GEN_REMIND,
		GEN_OIL_REMIND,
		GEN_AIR_REMIND,
		GEN_AIR_OIL_REMIND,
		ALL_REMIND,
	};

	static unsigned int currentMileage 		= 0;
	static int currentPulse					= 0;

	/*static unsigned int KM_OIL_CHANGE_REMIND	= 	7600;
	static unsigned int KM_OIL_CHANGE_WARN 		= 	7920;
	static unsigned int KM_AIR_FILTER_REMIND	= 	14600;
	static unsigned int KM_AIR_FILTER_WARN 		= 	14920;
	static unsigned int KM_GENERAL_REMIND 		= 	24600;
	static unsigned int KM_GENERAL_WARN 		= 	24920; */

	static unsigned int KM_OIL_CHANGE_REMIND	= 	2;
	static unsigned int KM_OIL_CHANGE_WARN 		= 	3;
	static unsigned int KM_AIR_FILTER_REMIND	= 	4;
	static unsigned int KM_AIR_FILTER_WARN 		= 	5;
	static unsigned int KM_GENERAL_REMIND 		= 	6;
	static unsigned int KM_GENERAL_WARN 		= 	7; 

	static unsigned int lastOilChange		=	0;
	static unsigned int lastAirFilterChange =	0;
	static unsigned int lastGeneralService  =	0;

	static bool maintenanceMessageOn		=	0;
	static bool intermittentMessage 		= 	0;
	static enum MAINT_MSG MESSAGE 			=	EMPTY;
	
bool isOilChangeRequiredIntermittent() {
	if (currentMileage - lastOilChange >= KM_OIL_CHANGE_REMIND) {
		return true;
	} 
	else{
		return false;
	}
}

bool isOilChangeRequiredContinious() {
	if (currentMileage - lastOilChange >= KM_OIL_CHANGE_WARN) {
		return true;
	} 
	else {
		return false;
	}
}

bool isAirChangeRequiredIntermittent() {
	if (currentMileage - lastAirFilterChange >= KM_AIR_FILTER_REMIND) {
		return true;
	}
	else {
		return  false;
	}
}

bool isAirChangeRequiredContinious() {
	if (currentMileage - lastAirFilterChange >= KM_AIR_FILTER_WARN) {
		return true;
	} 
	else {
		return  false;
	}
}

bool isGeneralCheckRequiredIntermittent() {
	if (currentMileage - lastGeneralService >= KM_GENERAL_REMIND) {
		return true;
	} 
	else {
		return  false;
	}
}

bool isGeneralCheckRRequiredContinious() {
	if (currentMileage - lastGeneralService >= KM_GENERAL_WARN) {
		return true;
	} else {
		return  false;
	}
}

unsigned int MC::getCurrentMileage() {
	int pulse = TS::getCurrentPulse() ;
	if (currentMileage < pulse) {
		currentMileage = TS::getCurrentPulse() / TS::getCalibrationReference();
		currentPulse = pulse;
	}
	return currentMileage;
}

void MC::maintenanceRoutine(int tick) {
	LOG::Log(" ---- currentmileage : ");
	LOG::Log(getCurrentMileage());

	getCurrentMileage();
	maintenanceChecker();
	maintenanceMessageService(tick);
}

void MC::maintenanceMessageService(int tick) {

	if (maintenanceMessageOn) {
		if (intermittentMessage) {
			if (tick==2 || tick==1) {
				SDCP::displayMessage((char*)"                          ");
			}
			else {
				showMaintenanceMessage();
			}
		} else {
			showMaintenanceMessage();
		}
	}

}

void MC::maintenanceChecker() {
	
	maintenanceMessageOn = 0;
	intermittentMessage  = 0;

	if (isOilChangeRequiredIntermittent()) {
		maintenanceMessageOn = 1;
		if	(isOilChangeRequiredContinious())
		{
			MESSAGE = OIL_REMIND;
			intermittentMessage  = 0;
		}
		else {
			MESSAGE = OIL_REMIND;
			intermittentMessage  = 1;
		}
	}


	if (isAirChangeRequiredIntermittent()) {
		maintenanceMessageOn = 1;

		if	(isAirChangeRequiredContinious())
		{
			if (isOilChangeRequiredIntermittent() || isOilChangeRequiredContinious()) {
				MESSAGE = AIR_OIL_REMIND;
			} 
			else {
				MESSAGE = AIR_REMIND;
			}			
			intermittentMessage  = 0;
		}
		else {
			intermittentMessage  = 1;

			if (isOilChangeRequiredIntermittent() || isOilChangeRequiredContinious()) {
				MESSAGE = AIR_OIL_REMIND;
				if (isOilChangeRequiredContinious()) {
					intermittentMessage  = 0;
				}
			} 
			else {
				MESSAGE = AIR_REMIND;
			}
			
		}
	}


	if (isGeneralCheckRequiredIntermittent()) {
		maintenanceMessageOn = 1;
		if	(isGeneralCheckRRequiredContinious())
		{
			intermittentMessage  = 0;

			if ((isOilChangeRequiredIntermittent() || isOilChangeRequiredContinious())
				&& (isAirChangeRequiredIntermittent() || isAirChangeRequiredContinious())) {
					MESSAGE = GEN_AIR_OIL_REMIND;
			} 
			else  if (isAirChangeRequiredIntermittent() || isAirChangeRequiredContinious()) {
				MESSAGE = GEN_AIR_REMIND;
			} 
			else if (isOilChangeRequiredIntermittent() || isOilChangeRequiredContinious()) {
				MESSAGE = GEN_OIL_REMIND;
			} 
			else {
				MESSAGE = GEN_REMIND;
			}
		}
		else {
			intermittentMessage  = 1;

			if ((isOilChangeRequiredIntermittent() || isOilChangeRequiredContinious())
				&& (isAirChangeRequiredIntermittent() || isAirChangeRequiredContinious())) {
					MESSAGE = GEN_AIR_OIL_REMIND;
					if (isOilChangeRequiredContinious() || isAirChangeRequiredContinious()) {
						intermittentMessage  = 0;
					}
			} 
			else  if (isAirChangeRequiredIntermittent() || isAirChangeRequiredContinious()) {
				MESSAGE = GEN_AIR_REMIND;
				if (isAirChangeRequiredContinious()) {
					intermittentMessage = 0;
				}
			} 
			else if (isOilChangeRequiredIntermittent() || isOilChangeRequiredContinious()) {
				MESSAGE = GEN_OIL_REMIND;
				if (isOilChangeRequiredContinious()) {
					intermittentMessage = 0;
				}
			} 
			else {
				MESSAGE = GEN_REMIND;
			}			
		}
	}
}


void MC::debugger(unsigned int overrideMileage) {
	currentMileage = overrideMileage;
	cout << " ---- currentMileage : " << getCurrentMileage();
}

void MC::showMaintenanceMessage() {

	switch (MESSAGE) {

	case EMPTY:
		SDCP::displayMessage((char*)"                          ");
		break;

	case OIL_REMIND:
		SDCP::displayMessage((char*)"Oil & Filter Change       ");
		break;

	case AIR_REMIND:
		SDCP::displayMessage((char*)"Air Filter Change          ");
		break;

	case AIR_OIL_REMIND:
		SDCP::displayMessage((char*)"Air & Oil Filter Chg      ");
		break;

	case GEN_REMIND:
		SDCP::displayMessage((char*)"General Checkup           ");
		break;

	case GEN_OIL_REMIND:
		SDCP::displayMessage((char*)"General & Oil Change      ");
		break;

	case GEN_AIR_REMIND:
		SDCP::displayMessage((char*)"Gen & Air Filter Chg      ");
		break;
		
	case GEN_AIR_OIL_REMIND:
		SDCP::displayMessage((char*)"Gen-Air-Oil Change      ");
		break;
	}
}

void MC::serviceDoneHandler() {
	
	if (maintenanceMessageOn) {
		
		switch (MESSAGE) {
			case OIL_REMIND:
				lastOilChange = currentMileage;
				break;
			case AIR_REMIND:
				lastAirFilterChange = currentMileage;
				break;
			case AIR_OIL_REMIND:
				lastOilChange = currentMileage;
				lastAirFilterChange = currentMileage;
				break;
			case GEN_OIL_REMIND:			
				lastOilChange = currentMileage;
				lastGeneralService = currentMileage;
				break;
			case GEN_AIR_REMIND:			
				lastAirFilterChange = currentMileage;		
				lastGeneralService = currentMileage;
				break;
			case GEN_AIR_OIL_REMIND:			
				lastOilChange = currentMileage;
				lastAirFilterChange = currentMileage;		
				lastGeneralService = currentMileage;
				break;
		}

		maintenanceMessageOn	= 	0;
		intermittentMessage		= 	0;
		MESSAGE 				= 	EMPTY;
		SDCP::displayMessage((char*)"                          ");

		LOG::LogStart();
		LOG::Log("\n\nService Done ==> Last Oil Change at ");
		LOG::Log(lastOilChange);
		LOG::Log("km, Last Air Filter at ");
		LOG::Log(lastAirFilterChange);
		LOG::Log("km, Last General Checkup at ");
		LOG::Log(lastGeneralService);
		LOG::Log("km\n\n");

	}

	else {
		LOG::LogStart();
		LOG::Log("\n\nNo Service Required ==> Last Oil Change at ");
		LOG::Log(lastOilChange);
		LOG::Log("km, Last Air Filter at ");
		LOG::Log(lastAirFilterChange);
		LOG::Log("km, Last General Checkup at ");
		LOG::Log(lastGeneralService);
		LOG::Log("km\n\n");
	}
}
