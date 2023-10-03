#pragma once
#ifdef LN_DEBUG
#include <core/debug/debugMacros.h>
#else 
#include <core/core.h>
#endif

#include <condition_variable>
#include <mutex>
namespace luna
{
	class semaphore {
	private:
		mutable std::mutex mutex;
		mutable std::condition_variable condition;
		mutable uint32_t count = 0; // Initialized as locked.
#ifdef LN_DEBUG
		mutable uint32_t awaiters = 0;
#endif

	public:
		_ALWAYS_INLINE_ void post() const {
			std::lock_guard lock(mutex);
			count++;
			condition.notify_one();
		}

		_ALWAYS_INLINE_ void wait() const {
			std::unique_lock lock(mutex);
#ifdef LN_DEBUG
			++awaiters;
#endif
			while (!count) { // Handle spurious wake-ups.
				condition.wait(lock);
			}
			--count;
#ifdef LN_DEBUG
			--awaiters;
#endif
		}

		_ALWAYS_INLINE_ bool try_wait() const {
			std::lock_guard lock(mutex);
			if (count) {
				count--;
				return true;
			}
			else {
				return false;
			}
		}

#ifdef LN_DEBUG
		~semaphore() {
			// Destroying an std::condition_variable when not all threads waiting on it have been notified
			// invokes undefined behavior (e.g., it may be nicely destroyed or it may be awaited forever.)
			// That means other threads could still be running the body of std::condition_variable::wait()
			// but already past the safety checkpoint. That's the case for instance if that function is already
			// waiting to lock again.
			//
			// We will make the rule a bit more restrictive and simpler to understand at the same time: there
			// should not be any threads at any stage of the waiting by the time the semaphore is destroyed.
			//
			// We do so because of the following reasons:
			// - We have the guideline that threads must be awaited (i.e., completed), so the waiting thread
			//   must be completely done by the time the thread controlling it finally destroys the semaphore.
			//   Therefore, only a coding mistake could make the program run into such a attempt at premature
			//   destruction of the semaphore.
			// - In scripting, given that Semaphores are wrapped by RefCounted classes, in general it can't
			//   happen that a thread is trying to destroy a Semaphore while another is still doing whatever with
			//   it, so the simplification is mostly transparent to script writers.
			// - The redefined rule can be checked for failure to meet it, which is what this implementation does.
			//   This is useful to detect a few cases of potential misuse; namely:
			//   a) In scripting:
			//      * The coder is naughtily dealing with the reference count causing a semaphore to die prematurely.
			//      * The coder is letting the project reach its termination without having cleanly finished threads
			//        that await on semaphores (or at least, let the usual semaphore-controlled loop exit).
			//   b) In the native side, where Semaphore is not a ref-counted beast and certain coding mistakes can
			//      lead to its premature destruction as well.
			//
			// Let's let users know they are doing it wrong, but apply a, somewhat hacky, countermeasure against UB
			// in debug builds.
			std::lock_guard lock(mutex);
			if (awaiters) {

				// And now, the hacky countermeasure (i.e., leak the condition variable).
				new (&condition) std::condition_variable();
			}
		}
#endif
	};
}