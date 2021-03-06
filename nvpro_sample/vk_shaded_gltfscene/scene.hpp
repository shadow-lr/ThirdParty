/*
 * Copyright (c) 2019-2021, NVIDIA CORPORATION.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2019-2021 NVIDIA CORPORATION
 * SPDX-License-Identifier: Apache-2.0
 */



#include <array>
#include <nvmath/nvmath.h>

#include "nvh/gltfscene.hpp"
#include "nvvk/appbase_vkpp.hpp"
#include "nvvk/commands_vk.hpp"
#include "nvvk/debug_util_vk.hpp"
#include "nvvk/descriptorsets_vk.hpp"
#include "nvvk/gizmos_vk.hpp"
#include "nvvk/memorymanagement_vk.hpp"
#include "nvvk/profiler_vk.hpp"
#include "skydome.hpp"
#include "vkalloc.hpp"


//--------------------------------------------------------------------------------------------------
// Simple example showing a cube, camera movement and post-process
//
class VkScene : public nvvk::AppBase
{
public:
  void setup(const vk::Instance& instance, const vk::Device& device, const vk::PhysicalDevice& physicalDevice, uint32_t graphicsQueueIndex) override;


  void initExample();
  void display();


  void destroy() override;
  void onResize(int w, int h) override;
  void createRenderPass() override;
  void onKeyboardChar(unsigned char key) override;
  void onKeyboard(int key, int scancode, int action, int mods) override;

  float         getDepth(int x, int y);
  nvmath::vec3f unProjectScreenPosition(const nvmath::vec3f& screenPos);


  struct SceneUBO
  {
    nvmath::mat4f projection;
    nvmath::mat4f model;
    nvmath::vec4f cameraPosition = {0.f, 0.f, 0.f};
    nvmath::vec4f lightDirection = {1.f, 1.f, 1.f};
    float         lightIntensity = 1.0f;
    float         exposure       = 0.5f;
    float         gamma          = 2.2f;
    int           materialMode   = 0;
    int           tonemap        = 1;
    float         envIntensity   = 0.2f;
  } m_ubo;

  struct PushC
  {
    nvmath::vec4f albedo;
    nvmath::vec3f emissiveFactor  = {0, 0, 0};
    float         metallicFactor  = 1.0f;
    float         roughnessFactor = 1.0f;
    float         alphaCutoff     = 0.0f;
    int           alphamode{0};
  };


  enum
  {
    eScene,
    eMatrix,
    eMaterial,
    eEnv,
    NB_DSET
  };

  std::vector<nvvk::DescriptorSetBindings> m_descSetLayoutBind{NB_DSET};
  std::vector<vk::DescriptorSetLayout>     m_descSetLayout{NB_DSET};
  std::vector<vk::DescriptorPool>          m_descPool{NB_DSET};
  std::vector<vk::DescriptorSet>           m_descSet{NB_DSET};

  vk::PipelineLayout m_pipelineLayout    = {};
  vk::Pipeline       m_drawPipeline      = {};
  vk::CommandBuffer  m_recordedCmdBuffer = {};

  nvmath::vec4f m_clearColor = nvmath::vec4f(0.07f, 0.07f, 0.07f, 1.f);

  // GLTF scene model
  nvh::GltfScene m_gltfScene;

  VkScene()          = default;
  virtual ~VkScene() = default;

  void setScene(const std::string& filename);
  void setEnvironmentHdr(const std::string& hdrFilename);


private:
  void setupDescriptorSetLayout();
  void setupDescriptorSets();
  void prepareUniformBuffers();
  void preparePipelines();
  void recordCommandBuffer();
  void render(const vk::CommandBuffer& cmdBuff);
  void updateUniformBuffer(const vk::CommandBuffer& cmdBuffer);
  void drawUI();
  void importImages(tinygltf::Model& gltfModel);

  std::string  m_filename;
  std::string  m_hdrFilename;
  int          m_upVector = 1;
  nvvk::AxisVK m_axis;

  vk::RenderPass m_renderPassSky;
  vk::RenderPass m_renderPassUI;

  nvvk::Buffer       m_sceneBuffer;
  nvvk::Buffer       m_vertexBuffer;
  nvvk::Buffer       m_normalBuffer;
  nvvk::Buffer       m_colorBuffer;
  nvvk::Buffer       m_uvBuffer;
  nvvk::Buffer       m_indexBuffer;
  nvvk::Buffer       m_matrixBuffer;
  nvvk::Buffer       m_pixelBuffer;  // Picking
  nvvk::AllocationID m_pixelAlloc;

  std::vector<nvvk::Texture> m_textures;

  SkydomePbr m_skydome;

  Allocator m_alloc;

  nvvk::DebugUtil m_debug;

  struct NodeMatrices
  {
    nvmath::mat4f world;    // local to world
    nvmath::mat4f worldIT;  // local to world, inverse-transpose (to transform normal vectors)
  };
};
