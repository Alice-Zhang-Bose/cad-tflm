/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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

#include "../../../kernels/internal/reference/reduce.h"

#include "../../../core/c/builtin_op_data.h"
#include "../../../core/c/common.h"
#include "../../../kernels/internal/quantization_util.h"
#include "../../../kernels/internal/reference/integer_ops/mean.h"
#include "../../../kernels/internal/tensor_ctypes.h"
#include "../../../kernels/internal/types.h"
#include "../../../kernels/kernel_util.h"
#include "../kernel_util.h"
#include "xtensa.h"
#include "xtensa_reduce.h"
#include "../../micro_utils.h"

namespace tflite {

void* XtensaInitReduce(TfLiteContext* context, const char* buffer,
                       size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  void* data =
      context->AllocatePersistentBuffer(context, sizeof(XtensaReduceOpData));

#if defined(VISION_P6)
  if (InitXtensaContext() != 0) {
    return nullptr;
  }
#endif
  return data;
}

TfLiteStatus XtensaPrepareMax(TfLiteContext* context, TfLiteNode* node) {
  OpDataReduce* op_data =
      &(static_cast<XtensaReduceOpData*>(node->user_data)->reference_op_data);
  TF_LITE_ENSURE_OK(context, PrepareMaxHelper(context, node, op_data));
#if defined(VISION_P6)
  TF_LITE_ENSURE_OK(context, ReducePrepareVision(context, node));
#endif  // VISION_P6
  return kTfLiteOk;
}

TfLiteStatus XtensaPrepareMeanOrSum(TfLiteContext* context, TfLiteNode* node) {
  OpDataReduce* op_data =
      &(static_cast<XtensaReduceOpData*>(node->user_data)->reference_op_data);
  return PrepareMeanOrSumHelper(context, node, op_data);
}

TfLiteStatus XtensaEvalMean(TfLiteContext* context, TfLiteNode* node) {
  OpDataReduce* op_data =
      &(static_cast<XtensaReduceOpData*>(node->user_data)->reference_op_data);
  return EvalMeanHelper(context, node, op_data);
}

TfLiteStatus XtensaEvalMax(TfLiteContext* context, TfLiteNode* node) {
  XtensaReduceOpData* op_data_xtensa =
      static_cast<XtensaReduceOpData*>(node->user_data);
  OpDataReduce* op_data = &(op_data_xtensa->reference_op_data);

#if defined(VISION_P6)
  const TfLiteEvalTensor* input = tflite::micro::GetEvalInput(context, node, 0);
  TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, 0);
  TF_LITE_ENSURE_TYPES_EQ(context, input->type, output->type);

  switch (input->type) {
    case kTfLiteInt8: {
      TF_LITE_ENSURE_EQ(context, static_cast<double>(op_data->input_scale),
                        static_cast<double>(op_data->output_scale));
      TF_LITE_ENSURE_EQ(context, op_data->input_zp, op_data->output_zp);
      ReduceEvalVision(*op_data_xtensa, input, output);
      break;
    }
    default: {
      // Use the reference EvalMax for all other cases.
      return EvalMaxHelper(context, node, op_data);
    }
  }
  return kTfLiteOk;
#else
  return EvalMaxHelper(context, node, op_data);
#endif
}

TfLiteStatus XtensaEvalSum(TfLiteContext* context, TfLiteNode* node) {
  OpDataReduce* op_data =
      &(static_cast<XtensaReduceOpData*>(node->user_data)->reference_op_data);
  return EvalSumHelper(context, node, op_data);
}

TFLMRegistration Register_MEAN() {
  return tflite::micro::RegisterOp(XtensaInitReduce, XtensaPrepareMeanOrSum,
                                   XtensaEvalMean);
}

TFLMRegistration Register_REDUCE_MAX() {
  return tflite::micro::RegisterOp(XtensaInitReduce, XtensaPrepareMax,
                                   XtensaEvalMax);
}

TFLMRegistration Register_SUM() {
  return tflite::micro::RegisterOp(XtensaInitReduce, XtensaPrepareMeanOrSum,
                                   XtensaEvalSum);
}

}  // namespace tflite
