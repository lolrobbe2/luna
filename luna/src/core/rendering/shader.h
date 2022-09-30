#pragma once
#include <core/core.h>
namespace luna
{
	namespace renderer
	{
		enum typeId
		{
			Unknown,
			Void,
			Boolean,
			SByte,
			UByte,
			Short,
			UShort,
			Int,
			UInt,
			Int64,
			UInt64,
			AtomicCounter,
			Half,
			Float,
			Double,
			Struct,
			Image,
			SampledImage,
			Sampler,
			AccelerationStructure,
			RayQuery,

			// Keep internal types at the end.
			ControlPointArray,
			Interpolant,
			Char,
			uniform 
		};
		struct shaderResource
		{
			typeId type = Unknown;
			uint32_t location = 0;
			uint32_t set = 0;
			uint32_t binding = 0;
			uint32_t strides = 0;
			std::string name;
		};
		 
		class shader
		{
		public:

			virtual ~shader() = default;
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			virtual void createLayout() const = 0;
			static ref<shader> create(const std::string& filepath);
			static ref<shader> create(const std::vector<uint8_t> shaderSrc);
			std::string shaderName;
			std::vector<shaderResource> shaderLayout{};
		private:
			
		};

	}
}