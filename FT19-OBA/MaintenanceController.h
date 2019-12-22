#ifndef MaintenanceController_H
#define MaintenanceController_H

class MaintenanceController {
	public:
		MaintenanceController();

		static void maintenanceRoutine(int tick);
		static void maintenanceChecker();
		static void maintenanceMessageService(int tick);
		static void showMaintenanceMessage();
		static unsigned int getCurrentMileage();
		static void debugger(unsigned int overrideMileage);
		static void serviceDoneHandler();
};

typedef MaintenanceController		MC;

#endif