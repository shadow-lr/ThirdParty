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

#ifdef __cplusplus
// GLSL Type
using vec3 = nvmath::vec3f;
using vec4 = nvmath::vec4f;
#endif

struct GltfShadeMaterial
{
  int shadingModel;  // 0: metallic-roughness, 1: specular-glossiness

  // PbrMetallicRoughness
  vec4  pbrBaseColorFactor;
  int   pbrBaseColorTexture;
  float pbrMetallicFactor;
  float pbrRoughnessFactor;
  int   pbrMetallicRoughnessTexture;

  // KHR_materials_pbrSpecularGlossiness
  vec4  khrDiffuseFactor;
  int   khrDiffuseTexture;
  vec3  khrSpecularFactor;
  float khrGlossinessFactor;
  int   khrSpecularGlossinessTexture;

  int   emissiveTexture;
  vec3  emissiveFactor;
  int   alphaMode;
  float alphaCutoff;
  int   doubleSided;

  int   normalTexture;
  float normalTextureScale;
  int   occlusionTexture;
  float occlusionTextureStrength;
};
