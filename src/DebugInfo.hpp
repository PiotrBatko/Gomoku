#ifndef DEBUGINFO_HPP
#define DEBUGINFO_HPP

#include <iostream>
#include <string>

// Log non-error data without line ending.
#define LOG(...)		DebugInfo::Log		 (__VA_ARGS__)
// Log non-error data with line ending.
#define LOG_LN(...)		DebugInfo::LogLn	 (__VA_ARGS__)
// Log error with line ending. There is also function name and file line logged.
#define LOG_ERROR(...)	DebugInfo::LogErrorLn("ERROR in ", __FILE__, ":", __LINE__, ". In function ", __FUNCTION__, "(): ", __VA_ARGS__)

// This class is used for logging debug data: on console, file, etc.
// It is assumed that there isn't created any object of this class -
// Log() method is called only statically.
// There is already only console logging method implemented.
class DebugInfo {
	private:
		// Prevent creating object of this class.
		DebugInfo() {}

	public:
		template <typename T, typename... Args>
		static void LogLn(T&& arg1, Args&&... args) {
			Log(std::forward<T>(arg1));
			LogLn(std::forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		static void Log(T&& arg1, Args&&... args) {
			Log(std::forward<T>(arg1));
			Log(std::forward<Args>(args)...);
		}
		
		template <typename T>
		static void LogLn(T&& valueToLog) {
			//if (debugInfoShowMode == DebugInfoShowMode_ToConsole) {
				std::cout << valueToLog << std::endl;
			//} else if (debugInfoShowMode == DebugInfoShowMode_ToFile) {
			//}
		}

		template <typename T>
		static void Log(T&& valueToLog) {
			//if (debugInfoShowMode == DebugInfoShowMode_ToConsole) {
				std::cout << valueToLog;
			//} else if (debugInfoShowMode == DebugInfoShowMode_ToFile) {
			//}
		}

		template <typename... Args>
		static void LogErrorLn(Args&&... args) {
			LogLn(std::forward<Args>(args)...);
		}
};

#endif
