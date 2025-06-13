#include "ChatModel.hpp"
#include <iostream>
#include <stdexcept>

ChatModel::ChatModel(const std::wstring& model_path)
    : env(ORT_LOGGING_LEVEL_WARNING, "gptneox_inference"),
      session_options(),
      session(nullptr),
      allocator()
{
    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    session = Ort::Session(env, model_path.c_str(), session_options);

    size_t num_inputs = session.GetInputCount();
    input_names.reserve(num_inputs);
    input_shapes.reserve(num_inputs);

    for (size_t i = 0; i < num_inputs; i++) {
        Ort::AllocatedStringPtr input_name = session.GetInputNameAllocated(i, allocator);
        std::string name = input_name.get();
        input_names.push_back(name);
        input_names_index[name] = i;

        Ort::TypeInfo type_info = session.GetInputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        input_shapes.push_back(tensor_info.GetShape());

        std::cout << "Input[" << i << "] name: " << name << std::endl;
        std::cout << "Shape: [ ";
        for (auto dim : input_shapes.back()) {
            if (dim == -1) std::cout << "? ";
            else std::cout << dim << " ";
        }
        std::cout << "]" << std::endl;
    }
}

ChatModel::~ChatModel() {}

std::vector<std::string> ChatModel::GetInputNames() const {
    return input_names;
}

std::vector<int64_t> ChatModel::GetInputShape(size_t index) const {
    if (index >= input_shapes.size()) return {};
    return input_shapes[index];
}

// Infer 改成用 int64_t 輸入
std::vector<float> ChatModel::Infer(const std::unordered_map<std::string, std::vector<int64_t>>& inputs,
                                    const std::unordered_map<std::string, std::vector<int64_t>>& input_shapes_map)
{
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    std::vector<Ort::Value> input_tensors;
    std::vector<const char*> input_names_ptr;
    input_tensors.reserve(inputs.size());
    input_names_ptr.reserve(inputs.size());

    for (const auto& name : input_names) {
        auto it_data = inputs.find(name);
        auto it_shape = input_shapes_map.find(name);

        if (it_data == inputs.end()) {
            throw std::runtime_error("Input data for " + name + " not provided.");
        }
        if (it_shape == input_shapes_map.end()) {
            throw std::runtime_error("Input shape for " + name + " not provided.");
        }

        const std::vector<int64_t>& data = it_data->second;
        const std::vector<int64_t>& shape = it_shape->second;

        size_t expected_size = 1;
        for (auto d : shape) {
            if (d <= 0) throw std::runtime_error("Dynamic dimension not supported.");
            expected_size *= d;
        }
        if (data.size() != expected_size) {
            throw std::runtime_error("Input data size mismatch for " + name);
        }

        size_t idx = input_names_index[name];
        Ort::TypeInfo type_info = session.GetInputTypeInfo(idx);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        ONNXTensorElementDataType elem_type = tensor_info.GetElementType();

        input_names_ptr.push_back(name.c_str());

        if (elem_type == ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64) {
            // 正確：模型要 int64，輸入也用 int64
            input_tensors.emplace_back(
                Ort::Value::CreateTensor<int64_t>(memory_info,
                                                  const_cast<int64_t*>(data.data()),
                                                  data.size(),
                                                  shape.data(),
                                                  shape.size()));
        } else if (elem_type == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT) {
            // 如果模型是 float，這裡可加 float 輸入處理
            throw std::runtime_error("Model expects float input but int64 input provided. Please convert.");
        } else {
            throw std::runtime_error("Unsupported input tensor element type");
        }
    }

    // 取 output name 同上
    size_t output_count = session.GetOutputCount();
    std::vector<std::string> output_names_str;
    std::vector<const char*> output_names;
    output_names_str.reserve(output_count);
    output_names.reserve(output_count);

    for (size_t i = 0; i < output_count; i++) {
        Ort::AllocatedStringPtr output_name = session.GetOutputNameAllocated(i, allocator);
        output_names_str.emplace_back(output_name.get());
        output_names.push_back(output_names_str.back().c_str());
    }

    auto output_tensors = session.Run(Ort::RunOptions{nullptr},
                                      input_names_ptr.data(), input_tensors.data(), input_tensors.size(),
                                      output_names.data(), output_names.size());

    std::vector<float> output_data;
    if (!output_tensors.empty()) {
        auto& output_tensor = output_tensors[0];
        auto type_info = output_tensor.GetTensorTypeAndShapeInfo();
        size_t total_len = type_info.GetElementCount();

        float* out_ptr = output_tensor.GetTensorMutableData<float>();
        output_data.assign(out_ptr, out_ptr + total_len);
    }

    return output_data;
}
