#pragma once
#ifndef _VARIANT_
#define _VARIANT_
namespace luna 
{
	class variant
	{
	public:
		variant();
		~variant();

	private:
		union 
		{
			bool m_bool;
			int64_t m_int;
			uint64_t m_uint;
			double m_float;
		} data;
	};
}
#endif // !_VARIANT_
