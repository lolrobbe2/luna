#pragma once
#include <core/rendering/shader.h>
namespace luna
{
	namespace renderer
	{
		class bufferElement
		{
		public:
			bufferElement();
			bufferElement(const shaderResource& resource) : name(resource.name),type(resource.type),offset(resource.offset),size(resource.stride){};
			bufferElement(const std::string& Name, const typeId& Id, const uint32_t& Offset, const uint32_t& Size) : name(Name), type(Id), offset(Offset), size(Size){};
			bufferElement(const bufferElement& element) : name(element.name), type(element.type), offset(element.offset), size(element.size) {};

		private:
			std::string name;
			typeId type;
			uint32_t offset;
			uint32_t size;
		};
		class bufferLayout
		{
		public: 
			bufferLayout();
			bufferLayout(std::initializer_list<bufferElement> elements) : mElements(elements)
			{
			
			};
			bufferLayout(std::vector<shaderResource> shaderLayout) 
			{
				for (const auto& resource : shaderLayout) if (resource.resourceClass == stageInputs) mElements.push_back(bufferElement(resource));
			};
			virtual ~bufferLayout() = default;
		private:
			std::vector<bufferElement> mElements;
		};
		class vertexBuffer
		{
		public:
			virtual ~vertexBuffer() = default;

			virtual void bind() const = 0;
			virtual void unbind() const = 0;

			virtual void setData(const void* data, uint32_t size) = 0;

			virtual const bufferLayout& getLayout() const = 0;
			virtual void setLayout(const bufferLayout& layout) = 0;

			static ref<vertexBuffer> create(uint32_t size);
			static ref<vertexBuffer> create(float* vertices, uint32_t size);
		private:
			ref<shader> srcShader; // src shader to create buffer layout from
		};
		
		class indexBuffer
		{
			virtual ~indexBuffer() = default;

			virtual void bind() const = 0;
			virtual void unbind() const = 0;

			virtual uint32_t GetCount() const = 0;

			static ref<indexBuffer> Create(uint32_t* indices, uint32_t count);
		};
	}
}

