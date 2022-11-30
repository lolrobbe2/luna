#pragma once
#ifndef _UUID
#define _UUID
#include "core/core.h"
namespace luna
{
	/**
	 * @brief a general purpose uuid class with a maximum number of 2^64 uuid's
	 * the uuid class does not check for duplicates!
	 */
	class uuid
	{
	public:
		/**
		 * @brief creates/generates a uuid.
		 * 
		 */
		uuid();
		/**
		 * @brief generates a uuid from a uint64_t.
		 * 
		 * \param uuid 
		 */
		uuid(uint64_t uuid);
		/**
		 * @brief returns the uuid as uint64_t.
		 */
		uint64_t getId() { return _uuid; };
		operator uint64_t() { return _uuid; }
	private:
		uint64_t _uuid;
	};
}
#endif // !_UUID

