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

#ifndef __H_MESHUTILS_
#define __H_MESHUTILS_

#include "WMath.h"
#include <nvmath/nvmath.h>
#include <string>


namespace meshimport {

template <typename T>
struct MeshData
{
  T*        vertices;
  T*        normals;
  T*        uvs;
  uint32_t* indices;
  uint32_t  vertex_count;
  uint32_t  index_count;
  bool      has_normals;
  bool      has_uvs;

  int32_t materialID;
};

typedef MeshData<float> MeshDataf;

enum TextureType
{
  DIFFUSE = 0,
  AMBIENT,
  SPECULAR,
  OPACITY,
  NORMAL
};

struct TextureDataf
{
  TextureType type;
  uint32_t    count;
  char        filePaths[8][128];
};

struct MaterialDataf
{
  nvmath::vec4f diffuseColor;
  nvmath::vec4f ambientColor;
  nvmath::vec4f specularColor;
  float         opacity;
  float         shininess;
  float         reflectivity;
  uint32_t      textureCount;
  TextureDataf* textures;
};

struct OutputMaterialDataf
{
  MaterialDataf* materials;
  uint32_t       materialCount;
};

struct NodeAnimationKeyf
{
  nvmath::vec4f data;
  double        time;
};

struct NodeAnimationDataf
{
  NodeAnimationKeyf* position;
  NodeAnimationKeyf* rotation;
  NodeAnimationKeyf* scale;

  uint32_t position_count;
  uint32_t rotation_count;
  uint32_t scale_count;

  std::string* name;
};

struct AnimationDataf
{
  NodeAnimationDataf* nodes;
  uint32_t            node_count;
};

struct OutputAnimationDataf
{
  AnimationDataf* animations;
  uint32_t        animation_count;
};

struct OutputDataf
{
  MeshDataf* meshes;
  uint32_t   meshCount;
};

struct OutputMeshIndexDataf
{
  uint32_t* meshes;
  uint32_t  meshCount;
};

struct OutputNodeDataf
{
  OutputMeshIndexDataf meshData;
  nvmath::vec3f        position;
  nvmath::vec3f        scale;
  nvmath::quatf        rotation;

  std::string name;

  OutputNodeDataf* children;
  uint32_t         childCount;
};


struct OutputSceneDataf
{
  OutputNodeDataf*     rootNode;
  OutputDataf          meshData;
  OutputMaterialDataf  materialData;
  OutputAnimationDataf animationData;
};


}  // namespace meshimport

#endif
