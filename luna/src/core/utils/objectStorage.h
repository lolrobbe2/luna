#pragma once
#include <core/utils/vectorCache.h>
namespace luna
{
	namespace utils
	{
		typedef uint64_t storageObject;
		enum storageResult
		{
			storageInvalidHandle = 0,
			storageOpSucces = 1,
			storageOpFailed = 2
		};
		/**
		 * @brief cached object memory class with a key handle.
		 */
		template<typename value> class objectStorage
		{
		public:
			std::pair<storageResult, value> putValue(storageObject* key, value _value)
			{
				if (*key != 0) return { storageInvalidHandle,_value };
				std::pair<cacheObject, value> result = objectCache.putValue(key, _value);
				if (result.first)
				{
					objectMemory.insert({ result.first,result.second });
					return { storageOpSucces, _value };
				}
				return { storageOpSucces,_value };
				
			}
			std::pair<storageResult, value> getValue(const storageObject& key,const value& _value)
			{
				std::pair<cacheResult, value> result = objectCache.getValue(key);
				switch (result.first)
				{
				case cacheResult::cacheHit:
					return { storageOpSucces,result.second };
				case cacheResult::cacheMiss:
					auto iterator = objectMemory.find(key);
					if (iterator != objectMemory.end()) return { storageOpSucces,iterator->second };
					return { storageOpFailed,value()};
				case cacheResult::cacheInvalidHandle:
					return { storageInvalidHandle,value() };
				default:
					return { storageOpFailed,value() };
				}
			}
			std::pair<storageResult, value> setValue(const storageObject& key,const value& _value)
			{
				std::pair<cacheResult, value> result = objectCache.setValue(key, _value);
				switch (result.first)
				{
				case cacheResult::cacheOpSucces:
					return { storageOpSucces,result.second };
				case caheResult::cacheOpFailed:
					auto iterator = objectMemory.find(key);
					if (iterator != objectMemory.end())
					{
						iterator->second = _value;
						return {storageOpSucces,_value}
					}

				default:
					break;
				}
			}
		private:
			vectorCache<value> objectCache; // fast constantly used memory
			std::unordered_map<storageObject, value> objectMemory; //slower longterm object storage.
		};
	}
}


