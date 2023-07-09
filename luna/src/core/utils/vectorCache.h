#pragma once
#include <core/core.h>
#include <core/debug/uuid.h>
namespace luna
{
	namespace utils
	{
		using cacheObject = uint64_t;
		enum cacheResult
		{
			cacheMiss = 0,
			cacheHit = 1,
			cacheInvalidHandle = 2,
			cacheOpSuccess = 3,
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
			vectorCache() : maxCacheSize(200)
			{
				LN_PROFILE_FUNCTION();
				handleCache.reserve(maxCacheSize);
				valueCache.reserve(maxCacheSize);
			};
			/**
			 * @brief creates vectorcache with defined size (recommended to have size less then 1000 objects.
			 * 
			 * \param maxCacheSize
			 */
			vectorCache(size_t maxCacheSize) : maxCacheSize(maxCacheSize)
			{
				LN_PROFILE_FUNCTION();
				handleCache.reserve(maxCacheSize);
				valueCache.reserve(maxCacheSize);
			};
			/**
			 * @brief destroys/deletes the cache :).
			 */
			~vectorCache()
			{
				std::lock_guard<std::mutex> cacheGuard(lockGuard);
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
				LN_PROFILE_FUNCTION();
				if (*key == 0) *key = uuid();
				std::lock_guard<std::mutex> cacheGuard(lockGuard);

				if (handleCache.size() >= maxCacheSize)
				{
					value overFlowValue = std::move(valueCache.back());
					cacheObject overFlowKey = handleCache.back();
					handleCache.pop_back();
					valueCache.pop_back();
					handleCache.insert(handleCache.begin(), *key);
					valueCache.insert(valueCache.begin(), std::move(_value));
					return std::make_pair(overFlowKey, std::move(overFlowValue)); // returns true in case of cache overflow.
				}
				else
				{
					handleCache.insert(handleCache.begin(), *key);
					valueCache.insert(valueCache.begin(), std::move(_value));
					return std::make_pair(false, value()); // returns false incase there is no cache overflow.
				}
			};
			/**
			 * @brief gets a stored cahe value using the key handle.
			 * 
			 * \param key
			 * \return 
			 */
			std::pair<cacheResult, value> getValue(cacheObject key)
			{
				LN_PROFILE_FUNCTION();

				if (key == 0) return std::make_pair(cacheResult::cacheInvalidHandle, value());
				//lock mutex to block other threads from accesing it.
				std::lock_guard<std::mutex> cacheGuard(lockGuard);

				//find cahePair based on key. 
				auto it = findHandle(key);
				if (it != handleCache.end())
				{
					value requestedCacheObject = valueCache[std::distance(handleCache.begin(), it)];
					handleCache.erase(it);
					valueCache.erase(valueCache.begin() + std::distance(handleCache.begin(), it));
					handleCache.insert(handleCache.begin(), key);
					valueCache.insert(valueCache.begin(), requestedCacheObject);
					return std::make_pair(cacheResult::cacheHit, std::move(requestedCacheObject));
				}

				return std::make_pair(cacheResult::cacheMiss, value());
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
				LN_PROFILE_FUNCTION();


				if (key == 0) return std::make_pair(cacheResult::cacheInvalidHandle, value());
				std::lock_guard<std::mutex> cacheGuard(lockGuard);

				auto it = findHandle(key);
				if (it != handleCache.end())
				{
					valueCache[std::distance(handleCache.begin(), it)] = std::move(_value);
					return std::make_pair(cacheResult::cacheOpSuccess, valueCache[std::distance(handleCache.begin(), it)]);
				}

				return std::make_pair(cacheResult::cacheOpFailed, _value);
			}

			/**
			 * @brief erases value of the given key.
			 * 
			 * \param cacheObject key
			 * \param value _value
			 * \return cacheOpSuccess when value whas succesfully erased
			 * \return cacheOpFailed when value could not be found or some other operation failed
			 */
			std::pair<cacheResult, value> eraseValue(cacheObject key)
			{
				LN_PROFILE_FUNCTION();
				if (key == 0) return std::make_pair(cacheResult::cacheInvalidHandle, value());
				std::lock_guard<std::mutex> cacheGuard(lockGuard);

				auto it = findHandle(key);
				if (it != handleCache.end())
				{
					size_t index = std::distance(handleCache.begin(), it);
					value erasedValue = std::move(valueCache[index]);
					handleCache.erase(it);
					valueCache.erase(valueCache.begin() + index);
					return std::pair<cacheResult,value>(cacheResult::cacheOpSuccess, erasedValue);
				}

				return std::make_pair(cacheResult::cacheOpFailed, value());
			}

			/**
			* @brief checks if the cache contains a value with a certain key
			*/
			bool hasValue(cacheObject key) 
			{
				return findHandle(key) != handleCache.end();	
			}

			/**
			* @brief clears the objectStorage of their key/value pairs.
			*/
			void clear()
			{
				std::lock_guard<std::mutex>(this->lockGuard);
				handleCache.clear();
				valueCache.clear();
			}
		protected:

			uint64_t maxCacheSize;
			std::vector<cacheObject> handleCache; //use seperate vector to allow the entire vector to remain in cache.
			std::vector<value> valueCache; //same principle value byte size is unkown -> valueCache might not fit in cahce completely.
			mutable std::mutex lockGuard;

			typename std::vector<cacheObject>::iterator findHandle(cacheObject key)
			{
				return std::find(handleCache.begin(), handleCache.end(), key);
			}
		};
	}
}