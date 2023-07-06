#pragma once
#include <core/utils/vectorCache.h>
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
		/**
		 * @brief cached object memory class with a key handle.
		 */
		template<typename value> class objectStorage
		{
		public:
			value operator[](const uint64_t key) { return getValue(key, value()).second; };
			/**
			 * @brief deafault constructor whith default cache size of 200.
			 * @note this constructor is recommended for dafult use.
			 */
			objectStorage() {};
			/**
			 * @brief constructor with custom cache size.
			 * @note recommended to not go above cacheSize of 1000 else cache could become slow!
			 * @note recommended to use the deafult constructor unless you know what you are doing!
			 * 
			 * \param size_t cacheSize
			 */
			objectStorage(size_t cacheSize)
			{
				objectCache = vectorCache<value>(cacheSize);
			}
			/**
			 * @brief puts a value in the object storage.
			 * @note if the value of *key !=  it whill use this value instead of generating one.
			 * 
			 * \param storageObject* key pointer to key
			 * \param value _value value/object to be stored.
			 * \return std::pair<storageResult, value> value is the input value.
			 * @see storageResult
			 */
			std::pair<storageResult, value> putValue(storageObject* key,const value& _value)
			{
				LN_PROFILE_FUNCTION();
				std::pair<cacheObject, value> result = objectCache.putValue(key, _value);
				if (result.first)
				{
					objectMemory.insert({ result.first,result.second });
					return { storageOpSucces, _value };
				}
				return { storageOpSucces,_value };
				
			}
			/**
			 * @brief gets a value from the object storage.
			 * 
			 * \param storageObject key
			 * \param value _value
			 * \return std::pair<storageResult, value> value is the input value.
			 * @see storageResult
			 */
			std::pair<storageResult, value> getValue(const storageObject& key,const value& _value = value())
			{
				LN_PROFILE_FUNCTION();
				std::pair<cacheResult, value> result = objectCache.getValue(key);
				switch (result.first)
				{
				case cacheResult::cacheHit:
					return { storageOpSucces,result.second };
				case cacheResult::cacheMiss:
				{
					auto iterator = objectMemory.find(key);
					if (iterator != objectMemory.end()) return { storageOpSucces,iterator->second };
					return { storageOpFailed,value() };
				}
				case cacheResult::cacheInvalidHandle:
					return { storageInvalidHandle,value() };
				default:
					return { storageOpFailed,value() };
				}
			}
			/**
			 * @brief sets the value of a given key.
			 * 
			 * \param storageObject key
			 * \param value _value
			 * \return std::pair<storageResult, value> value is the input value.
			 * @see storageResult
			 */
			std::pair<storageResult, value> setValue(const storageObject& key,const value& _value)
			{
				LN_PROFILE_FUNCTION();
				std::pair<cacheResult, value> result = objectCache.setValue(key, _value);
				switch (result.first)
				{
				case cacheResult::cacheOpSucces:
					return { storageOpSucces,result.second };
				case cacheResult::cacheOpFailed:
					auto iterator = objectMemory.find(key);
					if (iterator != objectMemory.end())
					{
						iterator->second = _value;
						return {storageOpSucces,_value}
					}
				case cacheResult::cacheInvalidHandle:
					return { storageInvalidHandle,value() };
					return
				default:
					break;
				}
			}
			std::pair<storageResult, value> eraseValue(const storageObject& key)
			{
				LN_PROFILE_FUNCTION();
				std::pair<cacheResult, value> result = objectCache.eraseValue(key);
				switch (result.first)
				{
				case cacheResult::cacheOpSucces:
					return { storageOpSucces,result.second };
				case cacheResult::cacheOpFailed:
					objectMemory.erase(key);
					return { storageOpSucces,value() };
				case cacheResult::cacheInvalidHandle:
					return { storageInvalidHandle,value() };
				default:
					break;
				}

			}

			/**
			* @brief checks if a given key exists inside the objectStorage.
			*/
			bool hasValue(storageObject key)
			{
				if (objectCache.hasValue(key)) return true;
				return objectMemory.find(key) != objectMemory.end();
			}

			/**
			* @brief clears the objectstorage of all value's
			*/

			void clear() 
			{
				objectCache.clear();
				objectMemory.clear();
			}
		private:
			vectorCache<value> objectCache; // fast constantly used memory
			std::unordered_map<storageObject, value> objectMemory; //slower longterm object storage.
		};
	}
}


