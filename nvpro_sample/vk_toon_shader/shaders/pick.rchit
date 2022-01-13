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

#version 460
#extension GL_NV_ray_tracing : require
#extension GL_GOOGLE_include_directive : enable

#include "share.glsl"

// Payload information of the ray returning: 0 hit, 2 shadow
layout(location = 0) rayPayloadInNV PerRayData_pick prd;

// Raytracing hit attributes: barycentrics
hitAttributeNV vec2 attribs;

void main()
{
  prd.worldPos         = vec4(gl_WorldRayOriginNV + gl_WorldRayDirectionNV * gl_HitTNV, 0);
  prd.barycentrics     = vec4(1.0 - attribs.x - attribs.y, attribs.x, attribs.y, 0);
  prd.instanceID       = gl_InstanceID;
  prd.instanceCustomID = gl_InstanceCustomIndexNV;
  prd.primitiveID      = gl_PrimitiveID;
}
