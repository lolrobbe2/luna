#pragma once
#ifndef _TIMESTEP_
#define _TIMESTEP_
#include <core/core.h>

/*from hazel2D*/
namespace luna 
{
	namespace utils
	{
		class LN_API timestep
		{
		public:
			timestep() : m_Time(0.0f) {}  // Default constructor
			explicit timestep(float time) : m_Time(time) {}  // Explicit constructor
			explicit timestep(double time) : m_Time(time) {}  // Explicit constructor

			// Copy constructor and copy assignment (trivial)
			timestep(const timestep& other) = default;
			timestep& operator=(const timestep& other) = default;
			timestep& operator=(float time) {
				m_Time = time;
				return *this;
			}
			timestep& operator=(double time) {
				m_Time = time;
				return *this;
			}
			operator float() const { return m_Time; }

			float getSeconds() const { return m_Time; }
			float getMilliseconds() const { return m_Time * 1000.0f; }
			// Atomic-compatible assignment and retrieval methods
			void set(float time) { m_Time = time; }
			float get() const { return m_Time; }
		private:
			float m_Time;
		};
	}
}
#endif // !_TIMESTEP_
