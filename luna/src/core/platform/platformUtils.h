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
			static std::string getName();
			static std::string getLocale();
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
		static MonoString* GetLocale();

	};
}
