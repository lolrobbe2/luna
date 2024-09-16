#include "variant.h"
#include <core/object/object.h>
namespace luna
{
	variant::variant(void)
	{
		m_type = Nil;

	}
	variant::variant(bool p_bool)
	{
		m_data.m_bool = p_bool;
		m_type = Bool;
	}
	variant::variant(float p_float)
	{
		m_data.m_float = p_float;
		m_type = Float;
	}
	variant::variant(int8_t p_int)
	{
		m_data.m_int = p_int;
		m_type = Int;
	}
	variant::variant(int16_t p_int)
	{
		m_data.m_int = p_int;
		m_type = Int;
	}
	variant::variant(int32_t p_int)
	{
		m_data.m_int = p_int;
		m_type = Int;
	}
	variant::variant(int64_t p_int)
	{
		m_data.m_int = p_int;
		m_type = Int;
	}


	variant::variant(uint8_t p_uint)
	{
		m_data.m_uint = p_uint;
		m_type = Uint;
	}
	variant::variant(uint16_t p_uint)
	{
		m_data.m_uint = p_uint;
		m_type = Uint;
	}
	variant::variant(uint32_t p_uint)
	{
		m_data.m_uint = p_uint;
		m_type = Uint;
	}
	variant::variant(uint64_t p_uint)
	{
		m_data.m_int = p_uint;
		m_type = Uint;
	}
	variant::variant(double p_double)
	{
		m_data.m_double = p_double;
		m_type = Double;
	}


	variant::variant(const char* p_string)
	{

		LN_ERR_FAIL_NULL(p_string);

		size_t len = std::strlen(p_string) + 1; // +1 for null terminator
		m_data.m_ptr = new char[len];           // Allocate memory
		std::strcpy(static_cast<char*>(m_data.m_ptr), p_string); // Copy string

		m_type = String;
	}
	variant::variant(const std::string& p_string) : variant(p_string.c_str()) {}


	variant::variant(object* p_object)
	{
		LN_ERR_FAIL_NULL(p_object);
		objectData obj_data = { p_object->getScene(), p_object->getUUID().getId() };
		std::memcpy(m_data._mem, &obj_data, sizeof(objectData));
	}
}