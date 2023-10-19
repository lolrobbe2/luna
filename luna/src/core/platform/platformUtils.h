#pragma once
#include "lnpch.h"
#include <core/core.h>
#include <core/debug/debugMacros.h>
namespace luna 
{
	namespace platform
	{
		enum folderTypes 
		{
			desktop,
			desktopDir,	
			documents,
			music,
			video,
			fonts,
			appData,
			root, //windows = C:/
			programFiles,//windows = C:/Program Files
			programFilesX86,//windows = C:/Program Files (x86)
			recycleBin, 
			MAX_ENUM
		};
		enum cursorShape
		{
			ARROW,
			IBEAM,
			CROSSHAIR,
			HAND,
			HRESIZE,
			VRESIZE,
			APP_STARTING,
			WAITING
		};
		class LN_API os
		{
		public:
			static void init();
			static std::string openFileDialog(const char* filter);
			static std::string openFolderDialog();
			static std::string saveFileDialog(const char* filter);
			static std::vector<unsigned char> openFile(const std::string& filePath);
			static std::string getCurrentWorkingDirectory();
			static std::string getVersion();
			static std::string getName();
			static std::string getLocale();
			static std::string getLocaleLanguage();
			static int getProcessId();
			static std::string getExecutablePath();
			static void setCursorShape(const cursorShape shape);
			static uint64_t getTicksUsec();
			static uint64_t getTicksMsec();
		private:
			inline static uint64_t ticksStart;
			inline static uint64_t ticksPerSecond = 10000000; //1 win tick = 100ns
		};

		class LN_API filesystem
		{
		public:
			static std::string getSystemFolderPath(const folderTypes folderType);
			static std::filesystem::path getEngineRootPath();
		private:

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
