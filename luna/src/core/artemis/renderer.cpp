#include <core/artemis/rendering/builders/attachementBuilder.h>
#include <core/artemis/device/descriptorSet.h>
#include <core/artemis/device/commandBuffer.h>
#include <core/utils/shaderLibrary.h>
#include <core/assets/assetImporter.h>
#include <core/assets/assetManager.h>
#include <core/debug/debugMacros.h>
#ifdef IMGUI_API
#include <core/artemis/rendering/imGui.h>
#include <backends/imgui_impl_vulkan.cpp>
#include <backends/imgui_impl_glfw.h>


#endif // IMGUI_API
#include "renderer.h"



namespace luna 
{
	namespace artemis 
	{
#ifdef IMGUI_API
		static ref<imGui> imgui;
#endif // IMGUI_API
		renderer::renderer(const ref<vulkan::window>& window)
		{
			LN_PROFILE_FUNCTION();
			c_device = *new device(window);
			
			p_swapChain = c_device.getSwapchain();
#ifdef IMGUI_API
			imgui = createRef<imGui>(c_device, p_swapChain);
#endif // IMGUI_API
			
			p_allocator = c_device.getAllocator();
		
			assets::assetImporter::setAllocator(p_allocator);
			maxFramesInFlight = p_swapChain->size() - 1;
			
			setUpComputePipeline();
			setUpGraphicsPipeline();
#ifdef IMGUI_API
			setUpImguiPipeline();
#endif // IMGUI_API

			blankImage = assets::assetManager::getAsset<assets::image>(assets::assetManager::importAsset("src/assets/media/blank.png", assets::TEXTURE));
			 
			p_allocator->flush();
			
			bindImage(blankImage);
			p_window = window;
		}
#ifdef IMGUI_API

		void renderer::beginImGuiScene()
		{
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			//imgui commands
			ImGui::NewFrame();
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::DockSpaceOverViewport(viewport, ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoResize);
		}
		void renderer::endImGuiScene()
		{
			ImGui::Render();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
#endif // IMGUI_API

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
			p_allocator->flush();
			//c_device.waitIdle();
			computeInflightFences[currentFrame]->wait();
			inFlightFences[currentFrame]->wait();
			VkResult result = p_swapChain->acquireNextImage(UINT64_MAX, *imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &swapchainImageIndex);
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR 
#ifdef IMGUI_API
				|| resized
#endif // IMGUI_API
				)
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

				for (size_t i = 0; i < imguiFrameBuffers.size(); ++i)
					imguiFrameBuffers[i] = p_swapChain->getFrameBuffer(p_imguiRenderPass, i, 0, 1);
				
				frameBufferImages = p_allocator->allocateImages(imguiSceneSize, 4,VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,p_swapChain->size(), VK_FORMAT_B8G8R8A8_UNORM);
				
				frambufferGuiImages.resize(0);
				for (image& image : frameBufferImages)
					frambufferGuiImages.push_back(ImGui_ImplVulkan_AddTexture(*sampler, image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
				
				frameBuffers.resize(0);
				frameBuffers.resize(p_swapChain->size());
				for (size_t i = 0; i < frameBuffers.size(); ++i) 
					frameBuffers[i] = frameBuffer(c_device, frameBufferImages[i],p_renderPass);

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
					.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT)
					
					.addDynamicState(VK_DYNAMIC_STATE_SCISSOR)
					.addViewport(p_swapChain->getViewport())
					.addScissor(*p_swapChain)
					.setRenderPass(p_renderPass)
					.build();
#ifdef IMGUI_API
				//imguiEnabledImages = imguiEnabledImagesResize;
				
				resized = false;
#endif // IMGUI_API

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
			LN_ERR_FAIL_NULL_MSG(p_image, "[ARTEMIS] p_image was nullptr");
			bool bound = *p_image; //check for empty binding
			for (size_t i = 0; i < renderCmdBuffers.size(); i++)
				if (!renderCmdBuffers[i].bind(p_image, i)) { bound = true; break; }

			if (!bound)
			{
				renderCmdBuffers.push_back(renderCommandBuffer(p_allocator, computeDescriptorPool, grapchicsDescriptorPool, sampler, maxFramesInFlight));
				renderCmdBuffers.back().bind(p_image, renderCmdBuffers.size());
			}
		}

		void renderer::bindFont(const ref<assets::font> p_font)
		{
			LN_ERR_FAIL_NULL_MSG(p_font, "[ARTEMIS] p_font was nullptr");
			bool bound = *p_font; //check for empty binding
			for (size_t i = 0; i < renderCmdBuffers.size(); i++)
				if (!renderCmdBuffers[i].bind(p_font, i)) { bound = true; break; }

			if (!bound)
			{
				renderCmdBuffers.push_back(renderCommandBuffer(p_allocator, computeDescriptorPool, grapchicsDescriptorPool, sampler, maxFramesInFlight));
				renderCmdBuffers.back().bind(p_font, renderCmdBuffers.size());
			}
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
			frameBufferImages = p_allocator->allocateImages(imguiSceneSize, 4,VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,p_swapChain->size(), VK_FORMAT_B8G8R8A8_UNORM);
			for (size_t i = 0; i < frameBuffers.size(); ++i) frameBuffers[i] = *new frameBuffer(c_device, frameBufferImages[i],p_renderPass);
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
#ifdef IMGUI_API
				.setCreateFlags(VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT)
				.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT)
				.addDynamicState(VK_DYNAMIC_STATE_SCISSOR)
#endif // IMGUI_API

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
			imgui->setRenderPass(p_imguiRenderPass);
			imguiFrameBuffers.resize(p_swapChain->size());
			for (size_t i = 0; i < imguiFrameBuffers.size(); ++i) imguiFrameBuffers[i] = p_swapChain->getFrameBuffer(p_imguiRenderPass, i, 0, 1);
		
			//SAMPLER IS NEEDED FOR VK_FORMAT!

			for (image& image : frameBufferImages)
				frambufferGuiImages.push_back(ImGui_ImplVulkan_AddTexture(*sampler, image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
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
#ifdef IMGUI_API
			///TODO FIX VIEWPORT BUG WHERE THINGS ARE NOT CENTERED AND PART OF THE SCREEN IS GONE!
			VkViewport viewport{};
			//viewport.x = -imguiSceneSize.x / 2.0f;
			//viewport.y = -imguiSceneSize.y / 2.0f;

			viewport.x = 0.0f;
			viewport.y = 0.0f;

			viewport.width = imguiSceneSize.x;
			viewport.height = imguiSceneSize.y;

			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor{};
			scissor.offset.x = 0;
			scissor.offset.y = 0;
			scissor.extent.width = imguiSceneSize.x;
			scissor.extent.height = imguiSceneSize.y;

			p_graphicsCommandBuffer[currentFrame]->setScissor(scissor);
			p_graphicsCommandBuffer[currentFrame]->setViewport(viewport);
			for(ref<assets::image> p_image : imguiEnabledImages)
				p_graphicsCommandBuffer[currentFrame]->transitionImageLayout(p_image->_image,VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

			//imguiEnabledImages.clear();
#endif // IMGUI_API
			
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
			if(transition) 
				p_graphicsCommandBuffer[currentFrame]->transitionImageLayout(frameBufferImages[currentFrame],VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			//p_graphicsCommandBuffer[currentFrame]->setViewport(p_swapChain->getViewport());
			p_graphicsCommandBuffer[currentFrame]->beginRenderPass(p_imguiRenderPass, imguiFrameBuffers[swapchainImageIndex]);
			p_graphicsCommandBuffer[currentFrame]->bindPipeline(graphicsPipeline);
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *p_graphicsCommandBuffer[currentFrame]);
			p_graphicsCommandBuffer[currentFrame]->endCurrentRenderPass();
			
			if (transition) 
				p_graphicsCommandBuffer[currentFrame]->transitionImageLayout(frameBufferImages[currentFrame],VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

#endif // IMGUI_API

			p_graphicsCommandBuffer[currentFrame]->end();

			p_computeCommandPool->flush({ p_computeCommandBuffer[currentFrame].get() }, { computeFinishedSemaphores[currentFrame] }, {  }, computeInflightFences[currentFrame], nullptr, false);

			
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			p_graphicsCommandPool->flush({ p_graphicsCommandBuffer[currentFrame].get() }, {renderFinishedSemaphores[currentFrame]}, {computeFinishedSemaphores[currentFrame],imageAvailableSemaphores[currentFrame] }, inFlightFences[currentFrame],waitStages, false);
		}
		void renderer::flush() 
		{
		}
#ifdef IMGUI_API
		ImTextureID renderer::registerImGuiImage(const ref<assets::image> image)
		{
			imguiEnabledImages.push_back(image);
			//imguiEnabledImagesResize.push_back(image);
			return ImGui_ImplVulkan_AddTexture(*sampler, *image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}
		void renderer::unregisterImGuiImage(ImTextureID imGuiImageHandle)
		{
			return ImGui_ImplVulkan_RemoveTexture((VkDescriptorSet)imGuiImageHandle);
		}

		ImTextureID renderer::getWindowImage()
		{
			return frambufferGuiImages[currentFrame];
		}
		LN_API ImGuiContext* renderer::getImGuiContext()
		{
			return ImGui::GetCurrentContext();
		}
#endif
	}
}