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

#include "reshape.h"

#include <cstring>

#include "../../core/c/builtin_op_data.h"
#include "../../core/c/common.h"
#include "../../kernels/internal/tensor_ctypes.h"
#include "../../kernels/kernel_util.h"
#include "../../kernels/op_macros.h"
#include "kernel_util.h"
#include "../memory_helpers.h"
#include "../micro_utils.h"

namespace tflite {
namespace {
/*
TfLiteStatus Eval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteEvalTensor* input =
      tflite::micro::GetEvalInput(context, node, kReshapeInputTensor);
  TfLiteEvalTensor* output =
      tflite::micro::GetEvalOutput(context, node, kReshapeOutputTensor);

  // TODO(b/162522304): storing input bytes in OpData increases some models
  // significantly, possibly due to alignment issues.
  size_t input_bytes;
  TF_LITE_ENSURE_STATUS(TfLiteTypeSizeOf(input->type, &input_bytes));
  input_bytes *= ElementCount(*input->dims);

  // Do nothing for in-place reshape.
  if (input->data.raw != output->data.raw) {
    // Otherwise perform reshape with copy.
    memcpy(output->data.raw, input->data.raw, input_bytes);
  }
  return kTfLiteOk;
}
*/
}  // namespace
/*
TFLMRegistration Register_RESHAPE() {
  return tflite::micro::RegisterOp(nullptr, PrepareReshapeReference, Eval);
}
*/
}  // namespace tflite
