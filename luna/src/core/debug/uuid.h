#pragma once
#ifndef _UUID
#define _UUID
#include "core/core.h"
namespace luna
{
	class uuid
	{
	public:
		uuid();
		uuid(uint64_t uuid);
		uint64_t getId() { return _uuid; };
		operator uint64_t() { return _uuid; }
	private:
		uint64_t _uuid;
	};
}
#endif // !_UUID

