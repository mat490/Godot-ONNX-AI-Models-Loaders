# Godot ONNX AI Models Loader
Godot does not have a native tool to read pre-trained AI models in any format. This repository contains the project for a GDExtension to read and use pre-trained models stored in ONNX files.

This extension adds a new “Node” called “ONNXLoader”. This Node has two main methods: `void load_model(String path)` and `Array predict(Array input data)`, which are used to load the model and then perform the prediction.
To develop it I have used the onnxruntime library through Visual Studio 2022.

The code I developed for this Godot extension project is divided into:

- onnx_loader.h
- onnx_loader.cpp
- register_types.h
- register_types.cpp

## Implementing the ONNXLoader Node in your Godot project in
- First of all, you need to take into account that this extension works with models that receive integers, floats and strings as input values.

- To the ONNXLoader node you must first administer the path of your pre-trained model through the `load_model(String model_path)` method. 

- Afterwards you can use the `predict(Array input_data)` method which returns the output values ​​of your ML model


If you are only interested in using the "ONNXLoader" node in your game, all you need to do is copy the bin folder and paste it inside your game project (this bin folder contains the necessary gdextension, .dll, .lib and .exp files). And you will be able to see inside the engine the new ONNXLoader node.


### onnx_loader.h
This file is the definition of the onnx_loader class, declaring the methods and attributes needed for the ONNXLoader node.
The header of the program starts with the usual guards (#ifndef, #define, #endif) to avoid multiple inclusions and includes the necessary Godot and ONNX Runtime libraries.

The class contains several private members:

- `env`: The class contains several private members:
- `session`: A pointer to the ONNX Runtime session.
- `session_options`: Options to configure the ONNX session.
- `input_node_names` y `output_node_names`: Vectors that store the names of the input and output nodes of the model.

The class declares several public methods:

- `static void _bind_methods()`: Static method to bind class methods to the Godot system.
- `void _init()`: Initializer called by Godot.
- `void load_model(String model_path)`: Method to load a model from a specified path.
- `Array predict(Array input_data)`: Method to make predictions with the input data provided.
- `void process_predictions(int model_type)`: Additional method for specific prediction processes depending on the model type.

### onnx_loader.cpp
This is the implementation of the `onnx_loader` class. The program header includes the necessary libraries and below it is defined within the godot namespace.

The methods of the ONNXLoader class are bound so that they can be called from the Godot environment.

### register_types.h
This class is responsible for declaring the functions needed to initialize and deinitialize the extension module in the Godot engine.

### register_types.cpp
This implementation file, `register_types.cpp`, is part of the Godot extension and is responsible for registering and unregistering the `ONNXLoader` class in the Godot engine. It also contains the initialization function for the GDExtension library, which sets up the entry points needed to integrate the extension into Godot.

### To compile the project
First you need to clone the official godot repository into the folder that stores the "Godot-ONNX-AI-Models-Loaders" project
After that you will need to use the scons tool to compile inside the godot-cpp folder, then you will need to compile the main project folder (where the SConstruct file provided in this repository (ONNXLoader) is).

## How ONNX Loader adjusts the inputs and outputs of each model

This C++ program dynamically adjusts the inputs and outputs for each loaded ONNX model. Here's an explanation of how it works:

## 1. Model Loading

The `load_model` function loads an ONNX model:

```cpp
void ONNXLoader::load_model(String model_path) {
    // ...
    session = new Ort::Session(env, model_path_w.c_str(), session_options);
    // ...
}
```

## 2. Input Node Handling

After loading the model, it retrieves information about the input nodes:

```cpp
size_t num_input_nodes = session->GetInputCount();
input_node_names.resize(num_input_nodes);
for (size_t i = 0; i < num_input_nodes; ++i) {
    auto input_name = session->GetInputNameAllocated(i, allocator);
    input_node_names[i] = input_name.release();
}
```

## 3. Output Node Handling

Similarly, it obtains information about the output nodes:

```cpp
size_t num_output_nodes = session->GetOutputCount();
output_node_names.resize(num_output_nodes);
for (size_t i = 0; i < num_output_nodes; ++i) {
    auto output_name = session->GetOutputNameAllocated(i, allocator);
    output_node_names[i] = output_name.release();
}
```

## 4. Input Adjustment for Prediction

In the `predict` function, the input data is adjusted:

```cpp
std::vector<float> input_tensor_values;
for (int i = 0; i < input_data.size(); ++i) {
    input_tensor_values.push_back(static_cast<float>(input_data[i]));
}

std::vector<int64_t> input_node_dims = { 1, static_cast<int64_t>(input_tensor_values.size()) };
Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, input_tensor_values.data(), input_tensor_values.size(), input_node_dims.data(), input_node_dims.size());
```

## 5. Output Handling

After running the prediction, the output tensors are processed:

```cpp
auto output_tensors = session->Run(Ort::RunOptions{ nullptr }, input_node_names.data(), ort_inputs.data(), ort_inputs.size(), output_node_names.data(), output_node_names.size());
```

## 6. Flexible Output Processing

The ONNX Loader Node handles different types of outputs:

- For float tensors:
  ```cpp
  const float* floatarr = output_tensor.GetTensorData<float>();
  ```

- For integer tensors:
  ```cpp
  const int64_t* intarr = output_tensor.GetTensorData<int64_t>();
  ```

- For string tensors:
  ```cpp
  const char* result = output_tensor.GetTensorData<const char>();
  ```

This design allows the ONNX Loader to be flexible and handle various ONNX models with different input and output configurations, dynamically adjusting to the specific requirements of each loaded model.


> [!IMPORTANT]
> Official Godot C++ repository
> 
> https://github.com/godotengine/godot-cpp
>

> [!IMPORTANT]
> I have taken the following tutorial as a basis to develop this project.
>
> https://www.youtube.com/watch?v=02KJouOjQ0c


> [!NOTE]
> All contributions are welcome. 🤓


