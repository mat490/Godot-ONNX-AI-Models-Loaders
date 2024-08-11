# Godot ONNX I Models Loader
Godot does not have a native tool to read pre-trained AI models in any format. This repository contains the project for a GDExtension to read and use pre-trained models stored in ONNX files.

This extension adds a new “Node” called “ONNXLoader”. This Node has two main methods: `void load_model(String path)` and `Array predict(Array input data)`, which are used to load the model and then perform the prediction.
To develop it I have used the onnxruntime library through Visual Studio 2022.

El código que elaboré para el proyecto de esta extensión de Godot se divide en:

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
After that you will need to use the scons tool to compile inside the godot-cpp folder, then you will need to compile the main project folder (where the SConstruct file provided in this repository is).


> [!IMPORTANT]
> Official Godot C++ repository
> 
> https://github.com/godotengine/godot-cpp
>

> [!IMPORTANT]
> I have taken the following tutorial as a basis to develop this project.
>
> https://www.youtube.com/watch?v=02KJouOjQ0c


