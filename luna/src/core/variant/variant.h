#pragma once
#ifndef _VARIANT_
#define _VARIANT_
#include <core/core.h>
namespace luna 
{
	typedef std::vector<uint8_t> packedByteArray;
	typedef std::vector<int32_t> packedInt32Array;
	typedef std::vector<int64_t> packedInt64Array;
	typedef std::vector<float> packedFloat32Array;
	typedef std::vector<double> packedFloat64Array;
	typedef std::vector<std::string> packedStringArray;
	typedef std::vector<glm::vec2> packedVector2Array;
	typedef std::vector<glm::vec3> packedVector3Array;
	typedef std::vector<glm::vec4> packedColorArray;
	typedef std::vector<glm::vec4> packedVector4Array;
	typedef float real_t;
	class scene;
	class object;
	class variant
	{
	public:
		enum type
		{
			Nil,
			Bool,
			Float,
			Int,
			Uint,
			Double,
			String,
			Object,
			Array,
			PackedByteArray,
			PackedInt32Array,
			PackedInt64Array,

		};
		variant(void);
		variant(bool p_bool);
		variant(float p_float);

		variant(int8_t p_int);
		variant(int16_t p_int);
		variant(int32_t p_int);
		variant(int64_t p_int);

		variant(uint8_t p_uint);
		variant(uint16_t p_uint);
		variant(uint32_t p_uint);
		variant(uint64_t p_uint);
		variant(double p_double);
		variant(const char* p_string);
		variant(const std::string& p_string);	
		variant(object* p_object);
		variant();
		~variant();

	private:
		struct objectData
		{
			scene* p_scene;
			uint64_t objId;
		};
		union 
		{
			bool m_bool;
			int64_t m_int;
			uint64_t m_uint;
			float m_float;
			double m_double;
			void* m_ptr; 
			uint8_t _mem[sizeof(objectData) > (sizeof(real_t) * 4) ? sizeof(objectData) : (sizeof(real_t) * 4)]{ 0 };
		} m_data alignas(8);
		type m_type = Nil;
	};
}
#endif // !_VARIANT_
