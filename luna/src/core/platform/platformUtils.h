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
			static void bindStaticFunctions();
		};
	}
	class LN_API os
	{
	public:
		static MonoString* OpenFileDialogInternal(MonoString* filter);
		static MonoString* SaveFileDialogInternal(MonoString* filter);
	};
}
