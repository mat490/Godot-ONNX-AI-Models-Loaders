# Godot-ONNX-AI-Models-Loaders
Godot does not have a native tool to read pre-trained AI models in any format. This repository contains the project for a GDExtension to read and use pre-trained models stored in ONNX files.

This extension adds a new “Node” called “ONNXLoader”. This Node has two main methods: void load_model(String path) and Array predict(Array input data), which are used to load the model and then perform the prediction.
To develop it I have used the onnxruntime library through Visual Studio 2022.

El código que elaboré para el proyecto de esta extensión de Godot se divide en:

- onnx_loader.h
- onnx_loader.cpp
- register_types.h
- register_types.cpp

## Implementing the ONNXLoader Node in your Godot project in
If you are only interested in using the "ONNXLoader" node in your game, all you need to do is copy the bin folder that is inside the demo folder and paste it inside your game project (this bin folder contains the necessary gdextension, .dll, .lib and .exp files). 

### onnx_loader
This file is the definition of the onnx_loader class, declaring the methods and attributes needed for the ONNXLoader node.
The header of the program starts with the usual guards (#ifndef, #define, #endif) to avoid multiple inclusions and includes the necessary Godot and ONNX Runtime libraries.
