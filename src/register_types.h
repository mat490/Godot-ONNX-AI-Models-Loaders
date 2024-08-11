#ifndef GD_ONNX_GODOT_LOADER_REGISTER_TYPES_H
#define GD_ONNX_GODOT_LOADER_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

void initialize_onnx_godot_loader_module(godot::ModuleInitializationLevel p_level);
void uninitialize_onnx_godot_loader_module(godot::ModuleInitializationLevel p_level);

#endif // GD_ONNX_GODOT_LOADER_REGISTER_TYPES_H
