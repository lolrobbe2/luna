#pragma once
#include <core/core.h>
#include <core/debug/uuid.h>
namespace luna
{
	namespace utils
	{
		typedef uint64_t cacheObject;
		enum cacheResult
		{
			cacheMiss = 0,
			cacheHit = 1,
			cacheInvalidHandle = 2,
			cacheOpSucces = 3,
			cacheOpFailed = 4,
			CACHE_MAX_ENUM = 5
		};
		

		/**
		 * @brief a pretty optimal multithreaded vector cahce headeronly class.
		 * @note the vectorcache does not check for duplicate value's
		 */
		template<typename value> class vectorCache
		{

		public:
			/**
			 * @brief creates vectorCache with default cache size is of 200 
			 */
			vectorCache()
			{
				maxCacheSize = 200;
				handleCache.reserve(maxCacheSize);
				valueCache.reserve(maxCacheSize);
			};
			/**
			 * @brief creates vectorcache with defined size (recommended to have size less then 1000 objects.
			 * 
			 * \param maxCacheSize
			 */
			vectorCache(size_t maxCacheSize)
			{
				this->maxCacheSize = maxCacheSize;
				handleCache.reserve(maxCacheSize);
				valueCache.reserve(maxCacheSize);
			};
			/**
			 * @brief destroys/deletes the cache :).
			 */
			~vectorCache()
			{
				std::lock_guard<std::mutex>cacheGuard(this->lockGuard);
				handleCache.~vector();
				valueCache.~vector();
			};
			/**
			 * @brief returns a object in a vector cache using a key handle.
			 * 
			 * \param key a handle to the cahce object
			 * \param _value the cache object to store
			 * \return std::pair<bool, value> when succesful returs overFlowKey and the overFLow cache value.
			 */
			std::pair<cacheObject, value> putValue(cacheObject* key, value _value)
			{
				if (*key == 0) *key = uuid();
				std::lock_guard<std::mutex>(this->lockGuard);
				
				handleCache.insert(handleCache.begin(), *key);
				valueCache.insert(valueCache.begin(), _value);
				if (handleCache.size() > maxCacheSize)
				{
					value overFlowValue = valueCache.back();
					cacheObject overFlowKey = handleCache.back();
					handleCache.resize(maxCacheSize);
					valueCache.resize(maxCacheSize);
					return std::pair<cacheObject, value>(overFlowKey, overFlowValue); // returns true in case of cache overflow.
				}
				return std::pair<cacheObject, value>(false, value()); //returns false incase there is no cache overflow.
			};
			/**
			 * @brief gets a stored cahe value using the key handle.
			 * 
			 * \param key
			 * \return 
			 */
			std::pair<cacheResult, value> getValue(cacheObject key)
			{
				//lock mutex to block other threads from accesing it.
				if(key == 0) return std::pair<cacheResult, value>(cacheResult::cacheInvalidHandle, value());
				std::lock_guard<std::mutex>(this->lockGuard);

				//find cahePair based on key. 

				for (size_t iterator = 0; iterator < handleCache.size(); iterator++) //iterate iver handle cache.
				{
					cacheObject currentKey = handleCache[iterator];
					if (currentKey == key)
					{
						value requesteCacheObject = valueCache[iterator];
						handleCache.erase(handleCache.begin() + iterator);
						valueCache.erase(valueCache.begin() + iterator);
						handleCache.insert(handleCache.begin(), key);
						valueCache.insert(valueCache.begin(), requesteCacheObject);

						//return cache hit and value.
						return std::pair<cacheResult, value>(cacheResult::cacheHit, requesteCacheObject);
					}
				}
				//if value is not found return empty value and cache miss.
				return std::pair<cacheResult, value>(cacheResult::cacheMiss, value());
			};
			/**
			 * @brief sets the current value of the given cacheObject key.
			 * 
			 * \param cacheObject key the key to overwite
			 * \param _value the value to overwrite the current value
			 * \return 
			 */
			std::pair<cacheResult, value> setValue(cacheObject key, value _value)
			{
				if (key == 0) return std::pair<cacheResult, value>(cacheResult::cacheInvalidHandle, value());
				std::lock_guard<std::mutex>(this->lockGuard);

				for (size_t iterator = 0; iterator < handleCache.size(); iterator++)
				{
					cacheObject currentKey = handleCache[iterator];
					if (currentKey == key)
					{
						handleCache.erase(handleCache.begin() + iterator);
						valueCache.erase(valueCache.begin() + iterator);
						handleCache.insert(cache.begin(), key);
						valueCache.insert(valueCache.begin(), _value);
						//return cache hit and value.
						return std::pair<cacheResult, value>(cacheResult::cacheOpSucces, _value);
					}
				}
				return std::pair<cacheResult, value>(cacheResult::cacheOpFailed, _value);
			}

			/**
			 * @brief erases value of the given key.
			 * 
			 * \param cacheObject key
			 * \param value _value
			 * \return cacheOpSucces when value whas succesfully erased
			 * \return cacheOpFailed when value could not be found or some other operation failed
			 */
			std::pair<cacheResult, value> eraseValue(cacheObject key)
			{
				if (key == 0) return std::pair<cacheResult, value>(cacheResult::cacheInvalidHandle, value());
				std::lock_guard<std::mutex>(this->lockGuard);
				for (uint64_t iterator = 0; iterator < handleCache.size(); iterator++)
				{
					cacheObject currentKey = handleCache[iterator];
					if (currentKey == key)
					{
						handleCache.erase(handleCache.begin() + iterator);
						valueCache.erase(valueCache.begin() + iterator);
						return std::pair<cacheResult, value>(cacheResult::cacheOpSucces, value());
					}
				}
				return std::pair<cacheResult, value>(cacheResult::cacheOpFailed, value());
			}
		protected:

			uint64_t maxCacheSize;
			std::vector<uint64_t> handleCache; //use seperate vector to allow the entire vector to remain in cache.
			std::vector<value> valueCache; //same principle value size is unkown -> valueCache might not fit in cahce completely.
			mutable std::mutex lockGuard;
		};
	}
}