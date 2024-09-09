#include "variant.h"
namespace luna
{
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
}