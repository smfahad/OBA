#ifndef Throttle_H
#define Throttle_H

class Throttle {
	public:
		Throttle();
		static void sendThrottleSignal(unsigned int voltage);
		static unsigned int getThrottleResponse();
		static bool getOddParityBit(unsigned int x);
		static void setMaintainSpeed(int speed);
		static void maintainSpeed();
};

typedef Throttle THRO;

#endif