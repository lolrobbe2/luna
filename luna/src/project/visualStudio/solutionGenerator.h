#pragma once
#include <project/project.h>
namespace luna 
{
	namespace project 
	{
		class LN_API solutionGenerator
		{
		public:
			static bool generateSolution(const ref<project> project);
		};
	}
}


/*
 GUID guid;
 CoCreateGuid(&guid);
 BYTE * str;
 UuidToString((UUID*)&guid, &str);

 CString unique((LPTSTR)str);

 RpcStringFree(&str);

 unique.Replace(_T("-"), _T("_"));

 return unique;
 */