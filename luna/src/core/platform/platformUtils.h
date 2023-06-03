#pragma once
#include "lnpch.h"
#include <core/core.h>
namespace luna 
{
	namespace platform
	{
		class LN_API os
		{
		public:
			static std::string openFileDialog(const char* filter);
			static std::string saveFileDialog(const char* filter);
			static std::vector<unsigned char> openFile(const std::string& filePath);
			static std::string getCurrentWorkingDirectory();
			static std::string getVersion();
			static std::string getName();
			static std::string getLocale();
			static std::string getLocaleLanguage();
			static int getProcessId();
			static std::string getExecutablePath();
			static void setConsoleVisibility(bool visible);
			static void createConsole();
		};
	}
	class Os
	{
	public:
		static void RegisterMethods();
	private:
		static MonoString* OpenFileDialog(MonoString* filter);
		static MonoString* SaveFileDialog(MonoString* filter);
		static MonoString* GetCurrentWorkingDirectory();
		static MonoString* GetName();
		static MonoString* GetVersion();
		static MonoString* GetLocale();
		static MonoString* GetLocaleLanguage();
		static int GetProcessId();

	};
}
