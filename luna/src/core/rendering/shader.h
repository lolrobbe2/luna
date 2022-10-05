#pragma once
#include <core/core.h>
namespace luna
{
	namespace renderer
	{
		enum typeClass
		{
			uniformBuffers,
			storageBuffers,
			stageInputs,
			stageOutputs,
			subpassInputs,
			storageImages,
			sampledImages,
			atomicCounters,
			accelerationStructures,
			pushConstantBuffers,

		// For Vulkan GLSL and HLSL source,
		// these correspond to separate texture2D and samplers respectively.
			separateImages,
			separateSsamplers,
			builtinInputs,
			builtinOutputs,
		};
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
			Uniform,
			PushConstant,
			StorageBuffer,
			Vec2,
			Vec3,
			Vec4,
			mat2,
			mat3,
			mat4
		};
		enum shaderStage
		{
			shaderStageVertex,
			shaderStageTessellationControl,
			shaderStageTessellationEvaluation,
			shaderStageGeometry,
			shaderStageFragment,
			shaderStageCompute 
		};
		struct shaderResource
		{
			typeId type = Unknown;
			uint32_t location = 0;
			uint32_t set = 0;
			uint32_t binding = 0;
			uint32_t stride = 0;
			std::string name;
			std::vector<shaderResource> members{};
		};
		 
		class shader
		{
		public:

			virtual ~shader() = default;
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			virtual void createLayout() = 0;
			/**
			 * @brief returns the shader compiled source.
			 */
			inline std::vector<uint32_t> self() { return shaderSrc; };
			/**
			 * @brief creates a shader from a filepath.
			 * 
			 * \param filepath
			 * \return a shader reference.
			 */
			static ref<shader> create(const std::string& filepath,const shaderStage& stage);
			/**
			 * @brief creates a shader from compiled code.
			 * 
			 * \param shaderSrc the shader compiled src code
			 * \return a shader reference.
			 */
			static ref<shader> create(const std::vector<uint8_t>& shaderSrc,const shaderStage& stage);
			std::string shaderName;
			std::vector<shaderResource> shaderLayout;
			shaderStage stage;
			std::vector<uint32_t> shaderSrc;
		private:
			friend class vulkanDevice;
			

		};

	}
}