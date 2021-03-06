/**
 * Copyright (c) 2016-present, Facebook, Inc.
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
 */

#include "caffe2/operators/minmax_ops.h"

namespace caffe2 {

REGISTER_CPU_OPERATOR(Max, MaxOp<float, CPUContext>);
REGISTER_CPU_OPERATOR(Min, MinOp<float, CPUContext>);

OPERATOR_SCHEMA(Max)
    .NumInputs(1, INT_MAX)
    .NumOutputs(1)
    .IdenticalTypeAndShapeOfInput(0)
    .AllowInplace({{0, 0}})
    .SetDoc(R"DOC(
Element-wise max of each of the input tensors. The first input tensor can be
used in-place as the output tensor, in which case the max will be done in
place and results will be accumulated in input0. All inputs and outputs must
have the same shape and data type.
)DOC")
    .Input(0, "data_0", "First of the input tensors. Can be inplace.")
    .Output(0, "max", "Output tensor. Same dimension as inputs.");

OPERATOR_SCHEMA(Min)
    .NumInputs(1, INT_MAX)
    .NumOutputs(1)
    .IdenticalTypeAndShapeOfInput(0)
    .AllowInplace({{0, 0}})
    .SetDoc(R"DOC(
Element-wise min of each of the input tensors. The first input tensor can be
used in-place as the output tensor, in which case the min will be done in
place and results will be accumulated in input0. All inputs and outputs must
have the same shape and data type.
)DOC")
    .Input(0, "data_0", "First of the input tensors. Can be inplace.")
    .Output(0, "min", "Output tensor. Same dimension as inputs.");

template <typename T, class Context>
bool MaxOp<T, Context>::Compute() {
  auto& input0 = Input(0);
  const int N = input0.size();
  T* output_data = Output(0)->template mutable_data<T>();

  for (int i = 1; i < InputSize(); i++) {
    auto input_data = Input(i).template data<T>();
    EigenVectorMap<T> output_vec(output_data, N);
    output_vec = output_vec.cwiseMax(ConstEigenVectorMap<T>(input_data, N));
  }

  return true;
}

template <typename T, class Context>
bool MinOp<T, Context>::Compute() {
  auto& input0 = Input(0);
  const int N = input0.size();
  T* output_data = Output(0)->template mutable_data<T>();

  for (int i = 1; i < InputSize(); i++) {
    auto input_data = Input(i).template data<T>();
    EigenVectorMap<T> output_vec(output_data, N);
    output_vec = output_vec.cwiseMin(ConstEigenVectorMap<T>(input_data, N));
  }

  return true;
}

} // namespace caffe2
