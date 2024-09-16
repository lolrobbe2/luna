#pragma once
#ifndef _HASHED_OBJECT_STORAGE_
#define _HASHED_OBJECT_STORAGE_
#include "objectStorage.h"
namespace luna
{
	namespace utils
	{
		using storageObject = uint64_t;
		enum storageResult
		{
			storageInvalidHandle = 0,
			storageOpSucces = 1,
			storageOpFailed = 2
		};

		template<typename keyObject, typename value>
		class hashedObjectStorage
		{
		public:
			// Default constructor with default cache size of 200
			hashedObjectStorage() : objectStorage(200) {}

			// Constructor with custom cache size
			hashedObjectStorage(size_t cacheSize) : objectStorage(cacheSize) {}
			value& operator[](const keyObject& key) { return getValue(key, value()); };

			_ALWAYS_INLINE_ std::pair<storageResult, value> putValue(const keyObject& key, const value&& _value)
			{
				storage.putValue(hashFunction(key),std::forward<value>(_value))
			}
			_ALWAYS_INLINE_ value& getValue(const keyObject& key)
			{
				value value;
				storage.getValue(hashFunction(key),value);
				return value;
			}
		private:
			uint64_t hashFunction(const keyObject& key) const
			{
				static_assert(std::is_default_constructible_v<std::hash<keyObject>>,
					"[hashedObjectStorage]keyObject must be hashable using std::hash.");
				return std::hash<keyObject>{}(key);
			}
			objectStorage<value> storage;
		};
	}
}
#endif //!_HASHED_OBJECT_STORAGE_

