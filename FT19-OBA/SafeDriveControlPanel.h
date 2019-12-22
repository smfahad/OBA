#ifndef SafeDriveControlPanel_H
#define SafeDriveControlPanel_H

class SafeDriveControlPanel {
	public:
		SafeDriveControlPanel();
		static void sendLEDsignal(int ledNumber, bool ledBool);
		static void displayMessage(char message[]);
		static void displayMessage(int speed);
		static enum KEYMAP;
		static void keyPressHandler(int pressedKey);

		static bool getIsCareToSpeedLEDLit();
		static bool getIsHazardBoltingLEDLit();
};

typedef SafeDriveControlPanel	SDCP;

#endif