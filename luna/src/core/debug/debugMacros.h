#pragma once
#include <core/core.h>
#ifdef __GNUC__
//#define FUNCTION_STR __PRETTY_FUNCTION__ - too annoying
#define FUNCTION_STR __FUNCTION__
#else
#define FUNCTION_STR __FUNCTION__
#endif

#ifdef _MSC_VER
/**
 * Don't use GENERATE_TRAP() directly, should only be used be the macros below.
 */
#define GENERATE_TRAP() __debugbreak()
#else
/**
 * Don't use GENERATE_TRAP() directly, should only be used be the macros below.
 */
#define GENERATE_TRAP() __builtin_trap()
#endif

#ifndef _INC_STDIO
#include <stdio.h>
#define LN_FLUSH_STDOUT fflush(stdout)
#else 
#define LN_FLUSH_STDOUT fflush(stdout)
#endif // !


#if defined(__GNUC__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) x
#define unlikely(x) x
#endif

 /**
  * Error macros.
  * WARNING: These macros work in the opposite way to assert().
  *
  * Unlike exceptions and asserts, these macros try to maintain consistency and stability.
  * In most cases, bugs and/or invalid data are not fatal. They should never allow a perfectly
  * running application to fail or crash.
  * Always try to return processable data, so the engine can keep running well.
  * Use the _MSG versions to print a meaningful message to help with debugging.
  *
  * The `((void)0)` no-op statement is used as a trick to force us to put a semicolon after
  * those macros, making them look like proper statements.
  * The if wrappers are used to ensure that the macro replacement does not trigger unexpected
  * issues when expanded e.g. after an `if (cond) ERR_FAIL();` without braces.
  */

  // Index out of bounds error macros.
  // These macros should be used instead of `ERR_FAIL_COND` for bounds checking.

  // Integer index out of bounds error macros.

/**
 * Try using `ERR_FAIL_INDEX_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures an integer index `m_index` is less than `m_size` and greater than or equal to 0.
 * If not, the current function returns.
 */
#define LN_ERR_FAIL_INDEX(m_index, m_size)                                                                         \
	if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                     \
		LN_CORE_ERROR("an index error occured at line:{2} in function:{0} in file:{0}! index = {3}, file = {4}, index param = {5}, size param = {6}",FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, stringify(m_index), stringify(m_size)); \
		return;                                                                                                 \
	} else                                                                                                      \
		((void)0)

#define LN_ERR_FAIL_INDEX_MSG(m_index, m_size,m_msg)                                                                         \
	if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                     \
		LN_CORE_ERROR("an index error occured at line:{2} in function:{0} in file:{0}! index = {3}, file = {4}, index param = {5}, size param = {6}, msg{7}",FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, stringify(m_index), stringify(m_size),m_msg); \
		return;                                                                                                 \
	} else                                                                                                      \
		((void)0)

#define LN_ERR_FAIL_INDEX_V(m_index, m_size, m_retval)                                                                         \
	if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                     \
		LN_CORE_ERROR("an index error occured at line:{2} in function:{0} in file:{0}! index = {3}, file = {4}, index param = {5}, size param = {6}",FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, stringify(m_index), stringify(m_size)); \
		return m_retval;                                                                                                 \
	} else                                                                                                      \
		((void)0)

#define LN_ERR_FAIL_INDEX_V_MSG(m_index, m_size, m_retval, m_msg)                                                                         \
	if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                     \
		LN_CORE_ERROR("an index error occured at line:{2} in function:{0} in file:{0}! index = {3}, file = {4}, index param = {5}, size param = {6}, msg{7}",FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, stringify(m_index), stringify(m_size),m_msg); \
		return m_retval;                                                                                                 \
	} else                                                                                                      \
		((void)0)
#define CRASH_BAD_INDEX(m_index, m_size)                                                                                         \
	if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                                      \
		LN_CORE_ERROR("an index error occured at line:{2} in function:{0} in file:{0}! index = {3}, file = {4}, index param = {5}, size param = {6}",FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, stringify(m_index), stringify(m_size)); \
		LN_FLUSH_STDOUT;                                                                                                    \
		GENERATE_TRAP();                                                                                                         \
	} else                                                                                                                       \
		((void)0)

 // Null reference error macros.

 /**
  * Try using `ERR_FAIL_NULL_MSG`.
  * Only use this macro if there is no sensible error message.
  *
  * Ensures a pointer `m_param` is not null.
  * If it is null, the current function returns.
  */

#define LN_ERR_FAIL_NULL(m_param) \
	if ( !m_param ) {	\
		LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3}",FUNCTION_STR,__FILE__,__LINE__,"Parameter ' ",stringify(m_param)" ' is null.");	\
		return;	 \
	}	else ((void)0) 	

  /**
   * Ensures a pointer `m_param` is not null.
   * If it is null, prints `m_msg` and the current function returns.
   */
#define LN_ERR_FAIL_NULL_MSG(m_param,m_msg) \
	if ( !m_param ) {	\
		LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3}, msg: {4}",FUNCTION_STR,__FILE__,__LINE__,"Parameter ' " stringify(m_param)" ' is null.",m_msg);	\
		return;	 \
	}	else ((void)0) 

   /**
	* Try using `LN_ERR_FAIL_NULL_V_MSG`.
	* Only use this macro if there is no sensible error message.
	*
	* Ensures a pointer `m_param` is not null.
	* If it is null, the current function returns `m_retval`.
	*/
#define LN_ERR_FAIL_NULL_V(m_param,m_retval,m_msg) \
	if (!m_param) { \
		LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3}", FUNCTION_STR, __FILE__, __LINE__, "Parameter ' " stringify(m_param)" ' is null.");	\
		return m_retval;	 \
	} else ((void)0) 

	/**
	 * Ensures a pointer `m_param` is not null.
	 * If it is null, prints `m_msg` and the current function returns `m_retval`.
	 */
#define LN_ERR_FAIL_NULL_V_MSG(m_param,m_retval,m_msg) \
	if (!m_param) { \
		LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3}, msg: {4}", FUNCTION_STR, __FILE__, __LINE__, "Parameter ' " stringify(m_param)" ' is null.", m_msg);	\
		return m_retval;	 \
	} else ((void)0) 

	 /**
	  * Try using `LN_ERR_FAIL_COND_MSG`.
	  * Only use this macro if there is no sensible error message.
	  * If checking for null use ERR_FAIL_NULL_MSG instead.
	  * If checking index bounds use ERR_FAIL_INDEX_MSG instead.
	  *
	  * Ensures `m_cond` is false.
	  * If `m_cond` is true, the current function returns.
	  */
#define LN_ERR_FAIL_COND(m_cond)                                                                          \
	if (unlikely(m_cond)) {                                                                            \
		LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3}",FUNCTION_STR, __FILE__, __LINE__, "Condition \"" stringify(m_cond) "\" is true."); \
		return;                                                                                        \
	} else                                                                                             \
		((void)0)

#define LN_ERR_FAIL_COND_MSG(m_cond,m_msg)                                                                          \
	if (unlikely(m_cond)) {                                                                            \
		LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3}, msg: {4}",FUNCTION_STR, __FILE__, __LINE__, "Condition \"" stringify(m_cond) "\" is true.",m_msg); \
		return;                                                                                        \
	} else                                                                                             \
		((void)0)
#define LN_ERR_FAIL_COND_V(m_cond,m_retval)                                                                          \
	if (unlikely(m_cond)) {                                                                            \
		LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3}",FUNCTION_STR, __FILE__, __LINE__, "Condition \"" stringify(m_cond) "\" is true."); \
		return m_retval;                                                                                        \
	} else                                                                                             \
		((void)0)

#define LN_ERR_FAIL_COND_V_MSG(m_cond,m_retval,m_msg)                                                                          \
	if (unlikely(m_cond)) {                                                                            \
		LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3}, msg: {4}",FUNCTION_STR, __FILE__, __LINE__, "Condition \"" stringify(m_cond) "\" is true.",m_msg); \
		return m_retval;                                                                                        \
	} else                                                                                             \
		((void)0)

	  // Generic error macros.

	  /**
	   * Try using `ERR_FAIL_COND_MSG` or `ERR_FAIL_MSG`.
	   * Only use this macro if more complex error detection or recovery is required, and
	   * there is no sensible error message.
	   *
	   * The current function returns.
	   */
#define LN_ERR_FAIL()                                                                     \
	if (true) {                                                                        \
		LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3}",FUNCTION_STR, __FILE__, __LINE__, "Method/function failed."); \
		return;                                                                        \
	} else                                                                             \
		((void)0)

	   /**
		* Try using `ERR_FAIL_COND_MSG`.
		* Only use this macro if more complex error detection or recovery is required.
		*
		* Prints `m_msg`, and the current function returns.
		*/
#define LN_ERR_FAIL_MSG(m_msg)                                                                   \
	if (true) {                                                                               \
				LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3},msg: {4}",FUNCTION_STR, __FILE__, __LINE__, "Method/function failed.",m_msg); \
		return;                                                                               \
	} else                                                                                    \
		((void)0)

		/**
		 * Try using `ERR_FAIL_COND_V_MSG` or `ERR_FAIL_V_MSG`.
		 * Only use this macro if more complex error detection or recovery is required, and
		 * there is no sensible error message.
		 *
		 * The current function returns `m_retval`.
		 */
#define LN_ERR_FAIL_V(m_retval)                                                                     \
	if (true) {                                                                        \
		LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3}",FUNCTION_STR, __FILE__, __LINE__, "Method/function failed."); \
		return m_retval;                                                                        \
	} else                                                                             \
		((void)0)

		/**
		 * Try using `ERR_FAIL_COND_V_MSG`.
		 * Only use this macro if more complex error detection or recovery is required.
		 *
		 * Prints `m_msg`, and the current function returns.
		 */
#define LN_ERR_FAIL_V_MSG(m_retval,m_msg)                                                                   \
	if (true) {                                                                               \
				LN_CORE_ERROR("an error occured at line:{2} in function:{0} in file:{1}! , reason: {3},msg: {4}",FUNCTION_STR, __FILE__, __LINE__, "Method/function failed.",m_msg); \
		return m_retval;                                                                               \
	} else                                                                                    \
		((void)0)
