#include "renderer.h"
#include "rendering/builders/attachementBuilder.h"
#include <core/artemis/device/descriptorSet.h>
#include <core/artemis/device/commandBuffer.h>
#include <core/utils/shaderLibrary.h>
#include <core/assets/assetImporter.h>
#include <core/assets/assetManager.h>
#include <core/debug/debugMacros.h>
#ifdef IMGUI_API
#include <backends/imgui_impl_vulkan.cpp>
#endif // IMGUI_API


namespace luna 
{
	namespace artemis 
	{
		renderer::renderer(const ref<vulkan::window>& window)
		{
			LN_PROFILE_FUNCTION();
			c_device = *new device(window);
			
			p_swapChain = c_device.getSwapchain();

			p_allocator = c_device.getAllocator();
		
			assets::assetImporter::setAllocator(p_allocator);
			maxFramesInFlight = p_swapChain->size() - 1;
			
			setUpComputePipeline();
			setUpGraphicsPipeline();
			ref<assets::image> blankImageAsset = assets::assetManager::getAsset<assets::image>(assets::assetManager::importAsset("src/assets/media/blank.png", assets::TEXTURE));
			p_allocator->flush();
			
			renderCmdBuffers[0].bind(blankImageAsset, 0);
			p_window = window;
		}
		void renderer::beginScene()
		{
			currentBuffer = &renderCmdBuffers[0];
			currentBuffer->reset();
		}
		void renderer::endScene()
		{
			//for (renderCommandBuffer& renderCommandBuffer : renderCmdBuffers)
				//renderCommandBuffer.update();
		}
		void renderer::update()
		{
			//c_device.waitIdle();
			computeInflightFences[currentFrame]->wait();
			inFlightFences[currentFrame]->wait();
			VkResult result = p_swapChain->acquireNextImage(UINT64_MAX, *imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &swapchainImageIndex);
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				if (p_swapChain->invalid()) return;
				imageAvailableSemaphores[currentFrame] = c_device.getSemaphore(0);
				c_device.waitIdle();
				for (auto commandBuffer : p_computeCommandBuffer)
					commandBuffer->reset();
				for (auto commandBuffer : p_graphicsCommandBuffer)
					commandBuffer->reset();

				p_swapChain->resize(p_window->getWidth(), p_window->getHeight());
			
				ref<shader> vertexShader = shaderLibrary::get("vertex.glsl"); //get vertex shader
				ref<shader> fragmentShader = shaderLibrary::get("fragment.glsl"); //get vertex shader


				attachementBuilder attachementBuilder{ p_swapChain };
				attachement att = attachementBuilder
					.setClearColorValue(0.0f, 0.0f, 0.0f, 1.0f)
					.setSamples().setOp(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE)
					.setLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
					.setStencilOp(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE)
					.build();

				subPassBuilder subPassBuilder;
				subpassDescription subpass = subPassBuilder
					.addColorAttachement(att)
					.setBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
					.build();

				subpassDependency dependency{ 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,0 };
				renderPassBuilder renderPassBuilder = c_device.getRenderPassBuilder();
#ifdef IMGUI_API
				p_imguiRenderPass = renderPassBuilder
					.addSubPassDependency(dependency)
					.addSubPass(subpass)
					.build();
				imguiFrameBuffers.resize(0);
				imguiFrameBuffers.resize(p_swapChain->size());
				for (size_t i = 0; i < imguiFrameBuffers.size(); ++i) imguiFrameBuffers[i] = p_swapChain->getFrameBuffer(p_imguiRenderPass, i, 0, 1);
#else
				p_renderPass = renderPassBuilder
					.addSubPassDependency(dependency)
					.addSubPass(subpass)
					.build();
				frameBuffers.resize(0);
				frameBuffers.resize(p_swapChain->size());
				for (size_t i = 0; i < frameBuffers.size(); ++i) frameBuffers[i] = p_swapChain->getFrameBuffer(p_renderPass, i, 0, 1);
#endif // IMGUI_API
				//recreate pipeline
				pipelineBuilder graphicsPipelineBuilder = c_device.getPipelineBuilder();
				graphicsPipeline = graphicsPipelineBuilder
					.setColorBlendingParams()
					.setAlphaBlendingParams(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD)
					.setColorMask()
					.setPipelineType(GRAPHICS)
					.addShaderStage(vertexShader)
					.addShaderStage(fragmentShader)
					.addDescriptorSetLayout(grapchicsDescriptorPool)
					//.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT)
					//.addDynamicState(VK_DYNAMIC_STATE_SCISSOR)
					.addViewport(p_swapChain->getViewport())
					.addScissor(*p_swapChain)
					.setRenderPass(p_renderPass)
					.build();
				currentFrame = 0;
				return;
			}
			computeInflightFences[currentFrame]->reset();
			inFlightFences[currentFrame]->reset();
			//graphicsFences[swapchainImageIndex] = inFlightFences[currentFrame];

			//inFlightFences[currentFrame]->reset();
			for (renderCommandBuffer& commandBuffer : renderCmdBuffers) 
				commandBuffer.update(currentFrame);
			
			recordCommands();
			for (renderCommandBuffer& commandBuffer : renderCmdBuffers) 
				commandBuffer.reset();
			
			
			VkResult presentResult = p_graphicsCommandPool->present({ p_swapChain }, { renderFinishedSemaphores[currentFrame] },&swapchainImageIndex);

			if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR)
				LN_CORE_ERROR("out of date!");
			
			currentFrame = (currentFrame + 1) % maxFramesInFlight;
		}

		glm::vec4 renderer::normalizeColor(const glm::vec4& color)
		{
			return glm::normalize(color / 255.0f);
		}

		void renderer::bindImage(const ref<assets::image> p_image)
		{
			LN_ERR_FAIL_NULL_MSG(p_image, "[ARTEMIS] font was nullptr");
			bool bound = *p_image; //check for empty binding
			for (size_t i = 0; i < renderCmdBuffers.size(); i++)
				if (!renderCmdBuffers[i].bind(p_image, i)) { bound = true; break; }

			if (!bound)
			{
				renderCmdBuffers.push_back(renderCommandBuffer(p_allocator, computeDescriptorPool, grapchicsDescriptorPool, sampler, maxFramesInFlight));
				renderCmdBuffers.back().bind(p_image, renderCmdBuffers.size());
			}
		}

		void renderer::drawLabel(const glm::vec3& position, const glm::vec2& size, const ref<assets::font> font, const std::string labelText, const glm::vec4& color)
		{
			//TODO FONT BINDING
			LN_ERR_FAIL_NULL_MSG(font, "[ARTEMIS] font was nullptr");
			bool bound = *font; //check for empty binding
			for (size_t i = 0; i < renderCmdBuffers.size(); i++)
				if (!renderCmdBuffers[i].bind(font, i)) { bound = true; break; }

			if (!bound)
			{
				renderCmdBuffers.push_back(renderCommandBuffer(p_allocator, computeDescriptorPool, grapchicsDescriptorPool, sampler, maxFramesInFlight));
				renderCmdBuffers.back().bind(font, renderCmdBuffers.size());
			}


		
			float xAdvance = 0.0f;
			const ref<assets::image> spaceGlyph = font->getGlyph('_');
			const glm::vec2 normalizedDimensions = glm::vec2(1.0f) / getSceneDimensions();
			for (size_t i = 0; i < labelText.size(); i++)
			{
				xAdvance += font->getAdvance(labelText[i]).x * normalizedDimensions.x;
				drawCharQuadBound({ xAdvance + position.x, position.y + font->getAdvance(labelText[i]).y * normalizedDimensions.y, position.z }, size, font->getGlyph(labelText[i]));
				if (labelText[i] == ' ')
				{
					xAdvance += spaceGlyph->getExtent().x * normalizedDimensions.x;
				}
				else
				{
					xAdvance += font->getGlyph(labelText[i])->getExtent().x * normalizedDimensions.x;
				}
			}

		}

		void renderer::drawCharQuadBound(const glm::vec3 position, const glm::vec2& size, const ref<assets::image> image, const glm::vec4& color)
		{
			glm::mat4 transform = glm::mat4(1.0f); // Identity matrix

			// Set the translation
			transform[3] = glm::vec4(position, 1.0f);

			// Set the scale
			transform[0][0] = size.x;
			transform[1][1] = size.y;

			drawQuad({ transform,color,image->getUvCoords(),{*image,true} });
		}

		void renderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::array<glm::vec2, 4>& textureCoords)
		{
		}

		void renderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) 
		{
			const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
			drawQuad(transform,color);
		}
		void renderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<assets::image> image, const std::array<glm::vec2, 4>& textureCoords)
		{
		}
		void renderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<assets::image> image)
		{
			glm::mat4 transform = glm::mat4(1.0f); // Identity matrix

			// Set the translation
			transform[3] = glm::vec4(position, 1.0f);

			// Set the scale
			transform[0][0] = size.x;
			transform[1][1] = size.y;

			drawQuad(transform, {1,1,1,1}, image);

		}
		
		void renderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const ref<assets::image> image, const std::array<glm::vec2, 4>& textureCoords)
		{
		}

		void renderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const ref<assets::image> image)
		{
			glm::mat4 transform = glm::mat4(1.0f); // Identity matrix

			// Set the translation
			transform[3] = glm::vec4(position, 1.0f);

			// Set the scale
			transform[0][0] = size.x;
			transform[1][1] = size.y;

			drawQuad(transform, color, image);

		}
		void renderer::drawQuad(const glm::mat4& transform, const ref<assets::image> image, const std::array<glm::vec2, 4>& textureCoords)
		{
			if (image)
			{
				for (size_t i = 0; i < renderCmdBuffers.size(); i++)
					if (!renderCmdBuffers[i].bind(image, i)) return drawQuad({ transform,glm::vec4(1,1,1,1),textureCoords,*image }); //if an empty texture slot was found then bind it otherwise create new buffer
				renderCmdBuffers.push_back(renderCommandBuffer(p_allocator, computeDescriptorPool, grapchicsDescriptorPool, sampler, maxFramesInFlight));
				renderCmdBuffers.back().bind(image, renderCmdBuffers.size());
				return drawQuad({ transform,glm::vec4(1,1,1,1),*image,*image });
			}
			return drawQuad({ transform,glm::vec4(1,1,1,1),textureCoords,{*image , false} });
		}
		void renderer::drawQuad(const glm::mat4& transform, const ref<assets::image> image)
		{
			drawQuad(transform, image, *image);
		}
		void renderer::drawQuad(const glm::mat4& transform, const glm::vec4& color, const ref<assets::image> image, const std::array<glm::vec2, 4>& textureCoords)
		{
			if (image)
			{
				for (size_t i = 0; i < renderCmdBuffers.size(); i++)
					if (!renderCmdBuffers[i].bind(image, i)) return drawQuad({ transform,color,textureCoords,*image });
				renderCmdBuffers.push_back(renderCommandBuffer(p_allocator, computeDescriptorPool, grapchicsDescriptorPool, sampler, maxFramesInFlight));
				renderCmdBuffers.back().bind(image, renderCmdBuffers.size());
				return drawQuad({ transform,color,textureCoords,*image });
			}
			return drawQuad({ transform,color,textureCoords,{*image, false} });
		}
		void renderer::drawQuad(const glm::mat4& transform, const glm::vec4& color, const ref<assets::image> image)
		{
			drawQuad(transform, color, image,*image);
		}

		void renderer::drawQuad(const glm::mat4& transform,const glm::vec4& color1) 
		{
			drawQuad({ transform,color1 });
		}

		void renderer::drawQuad(const drawCommand& command) 
		{
			if(currentBuffer->addCommand(command))
			{
				LN_CORE_INFO("rip currentBuffer full");
			}
		}

		const glm::vec2 renderer::getSceneMousePos() const
		{
			//TODO mousepose
			return glm::vec2();
		}

		const glm::vec2 renderer::getSceneDimensions() const
		{
#ifndef IMGUI_API
			return { p_window->windowSpec.width, p_window->windowSpec.height };
#else 
			return  imguiSceneSize;
#endif // !IMGUI_API
		}

		void renderer::setUpComputePipeline()
		{
			p_computeCommandPool = c_device.getCommandPool(vkb::QueueType::compute);

			p_computeCommandBuffer.resize(p_swapChain->size() - 1);
			for (auto& commandBuffer : p_computeCommandBuffer) commandBuffer = p_computeCommandPool->getCommandBuffer();
			
			ref<shader> quadVertexGenerator = shaderLibrary::get("quadVertexGenerator.glsl"); //get compute shader

			descriptorPoolBuilder poolBuilder = c_device.getDescriptorPoolBuilder(quadVertexGenerator);

			//storageBuffers with fixed size used for vertex generation.
			computeDescriptorPool = poolBuilder
				.setStorageBufferAmount(1000)
				.build();
			
			pipelineBuilder computePipelineBuidler = c_device.getPipelineBuilder();
			computePipeline = computePipelineBuidler
				.setPipelineType(COMPUTE)
				.addShaderStage(quadVertexGenerator)
				.addDescriptorSetLayout(computeDescriptorPool)
				.build();

			computeFinishedSemaphores.resize(maxFramesInFlight);
			for (auto& semaphore : computeFinishedSemaphores) semaphore = c_device.getSemaphore(0);

			computeInflightFences.resize(maxFramesInFlight);
			for (auto& fence : computeInflightFences) fence = c_device.getFence(VK_FENCE_CREATE_SIGNALED_BIT);
		}

		void renderer::setUpGraphicsPipeline()
		{

			p_graphicsCommandPool = c_device.getCommandPool(vkb::QueueType::graphics);

			p_graphicsCommandBuffer.resize(maxFramesInFlight);
			for (auto& commandBuffer : p_graphicsCommandBuffer) commandBuffer = p_graphicsCommandPool->getCommandBuffer();
			
			ref<shader> vertexShader = shaderLibrary::get("vertex.glsl"); //get vertex shader
			ref<shader> fragmentShader = shaderLibrary::get("fragment.glsl"); //get vertex shader

			descriptorPoolBuilder poolBuilder = c_device.getDescriptorPoolBuilder(fragmentShader);

			//storageBuffers with fixed size used for vertex generation.
			grapchicsDescriptorPool = poolBuilder
				.setSamplerAmount(10*maxFramesInFlight)
				.setStorageImageAmount(32*10* maxFramesInFlight)
				.build();

			attachementBuilder attachementBuilder{ p_swapChain };
			attachement att = attachementBuilder
				.setClearColorValue(0.0f, 0.0f, 0.0f, 1.0f)
				.setSamples().setOp(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE)
				.setLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
				.setStencilOp(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE)
				.build();

			subPassBuilder subPassBuilder;
			subpassDescription subpass = subPassBuilder
				.addColorAttachement(att)
				.setBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
				.build();

			subpassDependency dependency { 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,0};
			renderPassBuilder renderPassBuilder = c_device.getRenderPassBuilder();
			
			p_renderPass = renderPassBuilder
				.addSubPassDependency(dependency)
				.addSubPass(subpass)
				.build();
			frameBuffers.resize(p_swapChain->size());

#ifdef IMGUI_API
			frameBufferImages = p_allocator->allocateImages(imguiSceneSize, 4,VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,p_swapChain->size());
			for (size_t i = 0; i < frameBuffers.size(); ++i) frameBuffers[i] = frameBuffer(c_device, frameBufferImages[i],p_renderPass);
#else 
			for (size_t i = 0; i < frameBuffers.size(); ++i) frameBuffers[i] = p_swapChain->getFrameBuffer(p_renderPass, i, 0, 1);
#endif // !
			pipelineBuilder graphicsPipelineBuilder = c_device.getPipelineBuilder();
			graphicsPipeline = graphicsPipelineBuilder
				.setColorBlendingParams()
				.setAlphaBlendingParams(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD)
				.setColorMask()
				.setPipelineType(GRAPHICS)
				.addShaderStage(vertexShader)
				.addShaderStage(fragmentShader)
				.addDescriptorSetLayout(grapchicsDescriptorPool)
				//.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT)
				//.addDynamicState(VK_DYNAMIC_STATE_SCISSOR)
				.addViewport(p_swapChain->getViewport())
				.addScissor(*p_swapChain)
				.setRenderPass(p_renderPass)
				.build();

			sampler = c_device.getSampler(VK_FILTER_NEAREST);
			renderCmdBuffers.reserve(10);
			for (size_t i = 0; i < 1; ++i) {
				renderCmdBuffers.push_back(renderCommandBuffer(p_allocator, computeDescriptorPool, grapchicsDescriptorPool, sampler,maxFramesInFlight));
			}
			currentBuffer = &renderCmdBuffers[0];

			inFlightFences.resize(maxFramesInFlight);
			for (auto& fence : inFlightFences) fence = c_device.getFence(VK_FENCE_CREATE_SIGNALED_BIT);
			

			imageAvailableSemaphores.resize(p_swapChain->size());
			for (auto& semaphore : imageAvailableSemaphores) semaphore = c_device.getSemaphore(0);
			

			// Resize and initialize renderFinishedSemaphores
			renderFinishedSemaphores.resize(maxFramesInFlight);
			for (auto& semaphore : renderFinishedSemaphores) semaphore = c_device.getSemaphore(0);
		}
#ifdef IMGUI_API
		void renderer::setUpImguiPipeline()
		{
			attachementBuilder attachementBuilder{ p_swapChain };
			attachement att = attachementBuilder
				.setClearColorValue(0.0f, 0.0f, 0.0f, 1.0f)
				.setSamples().setOp(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE)
				.setLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
				.setStencilOp(VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE)
				.build();

			subPassBuilder subPassBuilder;
			subpassDescription subpass = subPassBuilder
				.addColorAttachement(att)
				.setBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
				.build();

			subpassDependency dependency{ 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,0 };
			renderPassBuilder renderPassBuilder = c_device.getRenderPassBuilder();

			p_imguiRenderPass = renderPassBuilder
				.addSubPassDependency(dependency)
				.addSubPass(subpass)
				.build();
			for (size_t i = 0; i < imguiFrameBuffers.size(); ++i) imguiFrameBuffers[i] = p_swapChain->getFrameBuffer(p_imguiRenderPass, i, 0, 1);
		}
#endif // IMGUI_API

		void renderer::recordCommands()
		{
			p_computeCommandBuffer[currentFrame]->begin(0);
			p_computeCommandBuffer[currentFrame]->bindPipeline(computePipeline);
			for (renderCommandBuffer& renderCmdBuffer : renderCmdBuffers)
			{
				if (renderCmdBuffer.commandsAmount) {
					renderCmdBuffer.generateIndices();
					p_computeCommandBuffer[currentFrame]->bindDescriptorSet(computePipeline, renderCmdBuffer.computeDescriptorSet);
					p_computeCommandBuffer[currentFrame]->dispatch(ceil(renderCmdBuffer.commandsAmount / 512.0f), 1, 1); //dispatch per batch
				}
			}
			p_computeCommandBuffer[currentFrame]->end();

			p_graphicsCommandBuffer[currentFrame]->begin(0);
			p_graphicsCommandBuffer[currentFrame]->beginRenderPass(p_renderPass, frameBuffers[swapchainImageIndex]);
			p_graphicsCommandBuffer[currentFrame]->bindPipeline(graphicsPipeline);
			for (renderCommandBuffer& renderCmdBuffer : renderCmdBuffers)
			{
				if (renderCmdBuffer.commandsAmount)
				{
					static VkDeviceSize offsets = 0;
					p_graphicsCommandBuffer[currentFrame]->bindDescriptorSet(graphicsPipeline, renderCmdBuffer.graphicsDescriptorSets[currentFrame]);
					p_graphicsCommandBuffer[currentFrame]->bindIndexBuffer(renderCmdBuffer.cpuIndicesBuffer,0, VK_INDEX_TYPE_UINT32);
					p_graphicsCommandBuffer[currentFrame]->bindVertexBuffers(0, { renderCmdBuffer.gpuBuffer }, &offsets);
					p_graphicsCommandBuffer[currentFrame]->drawIndexed(renderCmdBuffer.commandsAmount * 6, 1, 0, 0, 0);
				}
			}
			p_graphicsCommandBuffer[currentFrame]->endCurrentRenderPass();

#ifdef IMGUI_API
			p_graphicsCommandBuffer[currentFrame]->beginRenderPass(p_imguiRenderPass, imguiFrameBuffers[swapchainImageIndex]);
			p_graphicsCommandBuffer[currentFrame]->bindPipeline(graphicsPipeline);
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *p_graphicsCommandBuffer[currentFrame]);
			p_graphicsCommandBuffer[currentFrame]->endCurrentRenderPass();
#endif // IMGUI_API

			p_graphicsCommandBuffer[currentFrame]->end();

			p_computeCommandPool->flush({ p_computeCommandBuffer[currentFrame].get() }, { computeFinishedSemaphores[currentFrame] }, {  }, computeInflightFences[currentFrame], nullptr, false);

			
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			p_graphicsCommandPool->flush({ p_graphicsCommandBuffer[currentFrame].get() }, {renderFinishedSemaphores[currentFrame]}, {computeFinishedSemaphores[currentFrame],imageAvailableSemaphores[currentFrame] }, inFlightFences[currentFrame],waitStages, false);
		}
		void renderer::flush() 
		{

		}
	}
}