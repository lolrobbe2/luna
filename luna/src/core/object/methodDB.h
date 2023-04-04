#pragma once
#include <core/core.h>
#include <core/debug/uuid.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
namespace luna
{
	class methodDB
	{
	public:
		static void setName(uuid nodeId, MonoString* name);
	};
}
