#include "solutionGenerator.h"
#include <combaseapi.h>
namespace luna 
{
	namespace project 
	{
		static std::string createGuid() 
		{
			GUID guid;
			HRESULT result = CoCreateGuid(&guid);
			if (result != S_OK) {
				LN_CORE_ERROR("could not create visual studio GUID!");
				return "";
			}
			std::stringstream guidStream;
			guidStream << std::uppercase;
			guidStream.width(8);
			guidStream << std::hex << guid.Data1 << '-';

			guidStream.width(4);
			guidStream << std::hex << guid.Data2 << '-';

			guidStream.width(4);
			guidStream << std::hex << guid.Data3 << '-';

			guidStream.width(2);
			guidStream << std::hex
				<< static_cast<short>(guid.Data4[0])
				<< static_cast<short>(guid.Data4[1])
				<< '-'
				<< static_cast<short>(guid.Data4[2])
				<< static_cast<short>(guid.Data4[3])
				<< static_cast<short>(guid.Data4[4])
				<< static_cast<short>(guid.Data4[5])
				<< static_cast<short>(guid.Data4[6])
				<< static_cast<short>(guid.Data4[7]);
			guidStream << std::nouppercase;
			return guidStream.str();
		}
		bool solutionGenerator::generateSolution(const ref<project> project)
		{
			LN_CORE_INFO("guid: {}",createGuid());
			return false;
		}
	}
}