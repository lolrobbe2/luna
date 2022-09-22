#include "lnpch.h"
#include "uuid.h"

namespace luna
{
	
	static std::random_device randomDevice;
	static std::mt19937_64 engine(randomDevice());
	static std::uniform_int_distribution<uint64_t> uniformDistr;
	
	uuid::uuid()
	{
		_uuid = uniformDistr(engine);
	}
	uuid::uuid(uint64_t uuid)
	{

	}
}

