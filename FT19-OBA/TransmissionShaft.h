#ifndef TransmissionShaft_H
#define TransmissionShaft_H

class TransmissionShaft {
	public:
		TransmissionShaft();
		static unsigned int getCurrentPulse();
		static void calculateSpeed();
		static double getCurrentSpeed();
		static unsigned int getCurrentSpeedInt();
		static void setCalibrationReference(double calibrationOverride);
		static unsigned int getCalibrationReference();
};

typedef TransmissionShaft		TS;

#endif