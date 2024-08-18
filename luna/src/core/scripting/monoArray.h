#pragma once
#ifndef _MONO_ARRAY_
#define _MONO_ARRAY_

#include "monoClass.h"

namespace luna
{
	namespace scripting
	{
        class Iterator {
        public:
            Iterator(MonoArray* p_array, int index) : p_array(p_array), index(index) {}

            bool operator!=(const Iterator& other) const {
                return index != other.index;
            }

            Iterator& operator++() {
                ++index;
                return *this;
            }

            // Assuming we are dealing with int elements in the array
            monoObject operator*() const;

            Iterator& operator=(monoObject value);
        private:
            MonoArray* p_array;
            int index;
        };

		class monoArray
		{
		public:
			monoArray(monoClass monoClass,size_t size);
            monoArray(MonoArray* p_array);
            MonoArray* getNative() { return p_array; }
            _ALWAYS_INLINE_ size_t size() const;
            Iterator begin() const {
                return Iterator(p_array, 0);
            }

            Iterator end() const {
                return Iterator(p_array,size());
            }
            Iterator operator[](int index);
          
		private:
			MonoArray* p_array;
		};
	}
}
#endif // !_MONO_ARRAY_