/*
 * Copyright (c) 2014-2021, NVIDIA CORPORATION.  All rights reserved.
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
 * SPDX-FileCopyrightText: Copyright (c) 2014-2021 NVIDIA CORPORATION
 * SPDX-License-Identifier: Apache-2.0
 */

/* Contact chebert@nvidia.com (Chris Hebert) for feedback */

#ifndef __H_VKE_CAMERA_
#define __H_VKE_CAMERA_

#pragma once

#include "Transform.h"
#include "VkeBuffer.h"
#include <map>
#include <nvmath/nvmath.h>


#ifndef VKE_DEFAULT_CAMERA_VIEWPORT
#define VKE_DEFAULT_CAMERA_VIEWPORT 0, 0, 1024, 768
#endif

#ifndef VKE_DEFAULT_CAMERA_NEAR_PLANE
#define VKE_DEFAULT_CAMERA_NEAR_PLANE 0.001
#endif

#ifndef VKE_DEFAULT_CAMERA_FAR_PLANE
#define VKE_DEFAULT_CAMERA_FAR_PLANE 600.0
#endif

#ifndef VKE_DEFAULT_CAMERA_FOV
#define VKE_DEFAULT_CAMERA_FOV 45.0
#endif

typedef struct _VkeCameraUniform
{
  nvmath::mat4f view_proj_matrix;
  nvmath::mat4f view_matrix;
  nvmath::vec4f camera_position;
} VkeCameraUniform;

class VkeCamera : public VkeBuffer<VkeCameraUniform>
{
public:
  typedef uint32_t                            ID;
  typedef std::map<VkeCamera::ID, VkeCamera*> Map;
  typedef uint32_t                            Count;

  class List
  {
  public:
    List();
    ~List();

    VkeCamera* newCamera();
    VkeCamera* newCamera(const VkeCamera::ID& inID);
    void       addCamera(VkeCamera* const inData);
    VkeCamera* getCamera(const ID& inID);

    void update();

    ID    nextID();
    Count count();

    void getDescriptors(VkDescriptorBufferInfo* outDescriptor);


  private:
    VkeCamera::Map             m_data;
    std::vector<VkeCamera::ID> m_deleted_keys;
  };

  VkeCamera();
  VkeCamera(const ID& inID);
  VkeCamera(const ID& inID, const float inX, const float inY, const float inZ);
  ~VkeCamera();

  void initCameraData();
  void updateCameraCmd(VkCommandBuffer inCommand);
  void update();

  void bind(VkCommandBuffer* inBuffer);

  void setViewport(float inX, float inY, float inW, float inH);

  void setPosition(float inX, float inY, float inZ);
  void setRotation(float inX, float inY, float inZ);
  void setRotation(nvmath::quatf& inQuat);

  void setNear(float inNear);
  void setFar(float inFar);
  void setFOV(float inFOV);

  float getNear();
  float getFar();
  float getFOV();

  nvmath::vec4f worldPosition();
  nvmath::vec4f worldPosition(nvmath::vec4f& inPosition);

  void lookAt(nvmath::vec4f& inPosition);
  void setLookAtMatrix(nvmath::mat4f& inMat);

private:
  void updateProjection();
  void updateTransform();
  void updateViewProjection();

  ID m_id;

  nvmath::vec4f m_viewport;

  float m_near;
  float m_far;
  float m_fov;
  float m_aspect;

  nvmath::vec3f m_position;
  nvmath::vec3f m_rotation;

  Transform m_transform;

  nvmath::mat4f m_projection;
  bool          m_projection_needs_update;
  bool          m_transform_needs_update;

  bool m_view_projection_needs_update;

  nvmath::mat4f m_look_at_matrix;
  bool          m_use_look_at;

  float m_time;
};

#endif
