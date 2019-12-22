#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>    
#include "time.h"
#include <iomanip>

using namespace std;

Logger::Logger() {
}

void LOG::LogStart() {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	cout << endl << ltm->tm_hour << ":" << ltm->tm_min  << ":" << ltm->tm_sec << " -- ";
}

void LOG::Log(char* log) {
	cout << log;
}

void LOG::Log(int log) {
	cout << log;
}
