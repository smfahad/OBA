
class Logger {
	public:
		Logger();
		static void LogStart();
		static void Log(char* log);
		static void Log(int log);
};

typedef Logger	LOG;
