#include "OBATargetMachine.h"
#include "PortsLayout.h"
#include "DrivingStation.h"
#include "CruiseController.h"

DrivingStation::DrivingStation() {

}

DrivinStationStateWord *state;

unsigned int DS::getEngagedGearState() {
	state =  (DrivinStationStateWord *) (OTM::drivingControlsStateWordAddress);
	return state->engagedGear;
}


bool DS::getIgnitionState() {
	state =  (DrivinStationStateWord *) (OTM::drivingControlsStateWordAddress);
	return state->ignitionState;
}


bool DS::getClutchState() {
	state =  (DrivinStationStateWord *) (OTM::drivingControlsStateWordAddress);
	return state->clutchState;
}


bool DS::getBrakeState() {
	state =  (DrivinStationStateWord *) (OTM::drivingControlsStateWordAddress);
	return state->brakeState;
}


bool DS::getAcceleratorState() {
	state =  (DrivinStationStateWord *) (OTM::drivingControlsStateWordAddress);
	return state->acceleratorState;
}


void DS::handleDSTAinterrupt(DrivingStationInterruptStatusWord *whatChanged) {
	whatChanged = (DrivingStationInterruptStatusWord *) (OTM::drivingControlsInterruptWordAddress);

	if (whatChanged->brakeFlag || whatChanged->clutchFlag) {
		CRUISE::suspendCruising();
	}

	if (whatChanged->ignitionFlag) {
		CRUISE::stopCruising();
	}
}