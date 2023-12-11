#pragma once
#include <core/platform/windows/windowsWindow.h>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
namespace luna 
{
	namespace artemis 
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
			separateSamplers,
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
			typeClass resourceClass = uniformBuffers;
			typeId type = Unknown;
			uint32_t location = 0;
			uint32_t set = 0;
			uint32_t binding = 0;
			uint32_t stride = 0;
			uint32_t offset = 0;
			uint32_t amount = 0;
			std::string name;
			std::vector<shaderResource> members{};
		};
		class shader
		{
		public:
			shader(const VkDevice* device,const std::string& filepath, const shaderStage stage);
			operator const VkShaderModule() const { return _module; };
			const VkShaderModule native() const { return _module; };

			operator const shaderStage() const { return _stage; };
			const shaderStage nativeStage() const { return _stage; };

			~shader() { vkDestroyShaderModule(*device, _module, nullptr); };

		private:
			void createLayout();
			/**
			 * @brief get the shderResource information and returns a corresponding engine shaderresource.
			 *
			 * \param _shaderResource
			 * \param shaderSource
			 * \param typeClass
			 * \return
			 */
			shaderResource getShaderResource(const spirv_cross::Resource& _shaderResource, const std::vector<uint32_t>& shaderSource, typeClass typeClass);
			/**
			 * @brief create shader byte offsets.
			 *
			 * \param std::vector<renderer::shaderResource>* layout;
			 */
			void createOffsets(std::vector<shaderResource>* layout);
			void createShaderModule();
			std::string shaderName;
			shaderStage _stage;
			std::vector<shaderResource> shaderLayout;
			shaderStage stage;
			std::vector<uint32_t> shaderSrc;
			VkShaderModule _module;
			const VkDevice* device;
		};
	}
}


