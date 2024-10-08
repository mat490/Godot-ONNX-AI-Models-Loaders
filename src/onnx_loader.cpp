#include "onnx_loader.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <locale>
#include <codecvt>

using namespace godot;

void ONNXLoader::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_init"), &ONNXLoader::_init);
    ClassDB::bind_method(D_METHOD("load_model", "model_path"), &ONNXLoader::load_model);
    ClassDB::bind_method(D_METHOD("predict", "input_data"), &ONNXLoader::predict);
}

ONNXLoader::ONNXLoader() : env(ORT_LOGGING_LEVEL_WARNING, "ONNXLoader") {}

ONNXLoader::~ONNXLoader() {
    if (session) {
        delete session;
    }
}

void ONNXLoader::_init() {
    session = nullptr;
}

void ONNXLoader::load_model(String model_path) {
    UtilityFunctions::print("Starting model loading...");

    if (session) {
        delete session;
        session = nullptr;
        UtilityFunctions::print("Previous session deleted.");
    }

    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    // Convertir String de Godot a std::wstring
    UtilityFunctions::print("Converting model path...");
    std::string model_path_utf8 = model_path.utf8().get_data();
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring model_path_w = converter.from_bytes(model_path_utf8);

    UtilityFunctions::print("Converted model path:", model_path);

    try {
        session = new Ort::Session(env, model_path_w.c_str(), session_options);
        UtilityFunctions::print("Session created successfully.");
    }
    catch (const Ort::Exception& e) {
        UtilityFunctions::print("Error creating session:", String(e.what()));
        return;
    }

    // Obtener el allocator predeterminado
    UtilityFunctions::print("Getting default allocator...");
    Ort::AllocatorWithDefaultOptions allocator;

    size_t num_input_nodes = session->GetInputCount();
    input_node_names.resize(num_input_nodes);
    for (size_t i = 0; i < num_input_nodes; ++i) {
        auto input_name = session->GetInputNameAllocated(i, allocator);
        input_node_names[i] = input_name.release();
        UtilityFunctions::print(String("Entry node: ") + input_node_names[i]);
    }

    size_t num_output_nodes = session->GetOutputCount();
    output_node_names.resize(num_output_nodes);
    for (size_t i = 0; i < num_output_nodes; ++i) {
        auto output_name = session->GetOutputNameAllocated(i, allocator);
        output_node_names[i] = output_name.release();
        UtilityFunctions::print(String("Exit node: ") + output_node_names[i]);
    }

    UtilityFunctions::print("Model loaded and nodes obtained successfully.");
}

Array ONNXLoader::predict(Array input_data) {
    if (!session) {
        UtilityFunctions::print("Model not loaded.");
        return Array();
    }

    UtilityFunctions::print("Starting prediction...");

    try {
        // Convertir los datos de entrada a un vector de floats
        std::vector<float> input_tensor_values;
        for (int i = 0; i < input_data.size(); ++i) {
            input_tensor_values.push_back(static_cast<float>(input_data[i]));
        }

        UtilityFunctions::print("Input Tensioner Size: ", String::num(input_tensor_values.size()));

        // Crear el tensor de entrada
        Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        std::vector<int64_t> input_node_dims = { 1, static_cast<int64_t>(input_tensor_values.size()) };

        UtilityFunctions::print("Input tensioner dimensions: ", String::num(input_node_dims[0]) + ", " + String::num(input_node_dims[1]));

        Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, input_tensor_values.data(), input_tensor_values.size(), input_node_dims.data(), input_node_dims.size());

        std::vector<Ort::Value> ort_inputs;
        ort_inputs.emplace_back(std::move(input_tensor));

        UtilityFunctions::print("Number of entries: ", String::num(ort_inputs.size()));

        // Obtener los nombres de los nodos de entrada y salida
        UtilityFunctions::print("Input node names:");
        for (const auto& name : input_node_names) {
            UtilityFunctions::print(String(name));
        }

        UtilityFunctions::print("Exit node names:");
        for (const auto& name : output_node_names) {
            UtilityFunctions::print(String(name));
        }

        // Ejecutar la sesión de predicción
        UtilityFunctions::print("Running prediction session...");
        auto output_tensors = session->Run(Ort::RunOptions{ nullptr }, input_node_names.data(), ort_inputs.data(), ort_inputs.size(), output_node_names.data(), output_node_names.size());

        UtilityFunctions::print("Prediction session executed.");

        if (output_tensors.size() == 0) {
            UtilityFunctions::print("No output tensioners were received.");
            return Array();
        }

        UtilityFunctions::print("Number of output tensioners: ", String::num(output_tensors.size()));

        Array predictions;

        // Manejar cada tensor de salida
        for (const auto& output_tensor : output_tensors) {
            if (!output_tensor.IsTensor()) {
                UtilityFunctions::print("The output 'tensioner' is not a tensioner.");
                continue;
            }

            auto output_type = output_tensor.GetTensorTypeAndShapeInfo().GetElementType();

            UtilityFunctions::print("Output tensor data type: ", String::num(output_type));

            const auto& tensor_shape = output_tensor.GetTensorTypeAndShapeInfo().GetShape();
            size_t num_elements = 1;
            for (auto dim : tensor_shape) {
                num_elements *= dim;
            }

            UtilityFunctions::print("Number of elements in the tensor: ", String::num(num_elements));

            if (output_type == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT) {
                UtilityFunctions::print("Predicción de tipo flotante");
                const float* floatarr = output_tensor.GetTensorData<float>();
                if (floatarr == nullptr) {
                    UtilityFunctions::print("Error getting output tensor data.");
                    return Array();
                }

                for (size_t i = 0; i < num_elements; ++i) {
                    predictions.append(floatarr[i]);
                }
            }
            else if (output_type == ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64) {
                UtilityFunctions::print("Integer type prediction");
                const int64_t* intarr = output_tensor.GetTensorData<int64_t>();
                if (intarr == nullptr) {
                    UtilityFunctions::print("Error getting output tensor data.");
                    return Array();
                }

                for (size_t i = 0; i < num_elements; ++i) {
                    predictions.append(intarr[i]);
                }
            }
            else if (output_type == ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING) {
                UtilityFunctions::print("String type prediction");
                const char* result = output_tensor.GetTensorData<const char>();
                if (result == nullptr) {
                    UtilityFunctions::print("Error getting output tensor data.");
                    return Array();
                }
                predictions.append(String(result));
            }
            else {
                UtilityFunctions::print("Unrecognized output tensor data type.");
                return Array();
            }
        }

        UtilityFunctions::print("Prediction completed.");
        return predictions;
    }
    catch (const Ort::Exception& e) {
        UtilityFunctions::print("Error making prediction: ", String(e.what()));
        return Array();
    }
    catch (const std::exception& e) {
        UtilityFunctions::print("Standard Exception: ", String(e.what()));
        return Array();
    }
    catch (...) {
        UtilityFunctions::print("Unknown exception during prediction.");
        return Array();
    }
}
