#ifndef OBA_ACCEL_H
#define OBA_ACCEL_H

class Acceleration {
	public:
		Acceleration();

		static void startAccelerating();
		static void stopAccelerating();

};

typedef Acceleration	ACCEL;

#endif