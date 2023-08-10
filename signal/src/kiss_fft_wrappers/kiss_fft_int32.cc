/* Copyright 2023 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "kiss_fft_common.h"

#define FIXED_POINT 32
namespace kiss_fft_fixed32 {
#include "../../../tensorflow/lite/micro/tools/make/downloads/kissfft/kiss_fft.c"
#include "../../../tensorflow/lite/micro/tools/make/downloads/kissfft/tools/kiss_fftr.c"
}  // namespace kiss_fft_fixed32
#undef FIXED_POINT
