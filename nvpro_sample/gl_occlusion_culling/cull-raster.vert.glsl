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


#version 430
#extension GL_ARB_shading_language_include : enable
#include "cull-common.h"

//////////////////////////////////////////////

layout(binding=CULLSYS_UBO_VIEW, std140) uniform viewBuffer {
  ViewData view;
};

layout(binding=CULLSYS_SSBO_MATRICES, std430) readonly buffer matricesBuffer {
  MatrixData matrices[];
};

#ifdef DUALINDEX
layout(binding=CULLSYS_SSBO_BBOXES, std430) readonly buffer bboxBuffer {
  BboxData bboxes[];
};
#endif

layout(std430,binding=CULLSYS_SSBO_OUT_VIS) writeonly buffer visibleBuffer {
  int visibles[];
};

//////////////////////////////////////////////

#ifdef DUALINDEX
layout(location=0) in int  bboxIndex;
layout(location=2) in int  matrixIndex;

vec4 bboxMin = bboxes[bboxIndex].bboxMin;
vec4 bboxMax = bboxes[bboxIndex].bboxMax;
#else
layout(location=0) in vec4 bboxMin;
layout(location=1) in vec4 bboxMax;
layout(location=2) in int  matrixIndex;
#endif

out VertexOut{
  vec3 bboxCtr;
  vec3 bboxDim;
  flat int matrixIndex;
  flat int objid;
} OUT;

//////////////////////////////////////////////

void main()
{
  int objid = gl_VertexID;
  vec3 ctr =((bboxMin + bboxMax)*0.5).xyz;
  vec3 dim =((bboxMax - bboxMin)*0.5).xyz;
  OUT.bboxCtr = ctr;
  OUT.bboxDim = dim;
  OUT.matrixIndex = matrixIndex;
  OUT.objid = objid;
  
  
  // if camera is inside the bbox then none of our
  // side faces will be visible, must treat object as 
  // visible
  
  mat4 worldInvTransTM = matrices[matrixIndex].worldInvTransTM;
    
  vec3 objPos = (vec4(view.viewPos,1) * worldInvTransTM).xyz;
  objPos -= ctr;
  if (all(lessThan(abs(objPos),dim))){
    // inside bbox
    visibles[objid] = 1;
    // skip rasterization of this box
    OUT.objid = CULL_SKIP_ID;
  }
  else {
  #if 1
    // avoid loading data
    mat4 worldTM = inverse(transpose(worldInvTransTM));
  #else
    mat4 worldTM = matrices[matrixIndex].worldTM;
  #endif
    mat4 worldViewProjTM = view.viewProjTM * worldTM;
  
    // frustum and pixel cull
    vec4 hPos0    = worldViewProjTM * getBoxCorner(bboxMin, bboxMax, 0);
    vec3 clipmin  = projected(hPos0);
    vec3 clipmax  = clipmin;
    uint clipbits = getCullBits(hPos0);

    for (int n = 1; n < 8; n++){
      vec4 hPos   = worldViewProjTM * getBoxCorner(bboxMin, bboxMax, n);
      vec3 ab     = projected(hPos);
      clipmin = min(clipmin,ab);
      clipmax = max(clipmax,ab);
      clipbits &= getCullBits(hPos);
    }    
    
    if (clipbits != 0 || pixelCull(view.viewSize, view.viewCullThreshold, clipmin, clipmax))
    {
      // invisible
      // skip rasterization of this box
      OUT.objid = CULL_SKIP_ID;
    }
  }
}
