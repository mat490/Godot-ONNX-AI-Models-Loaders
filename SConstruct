#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# Agregar rutas de inclusión para los archivos de encabezado
env.Append(CPPPATH=[
    "src/",
    "Microsoft.ML.OnnxRuntime.1.18.1/build/native/include"
])

# Agregar rutas para las bibliotecas
env.Append(LIBPATH=[
    #"C:/Users/SPARTAN PC/Documents/GodotC++/LeandroC++/src/Microsoft.ML.OnnxRuntime.1.18.1/runtimes/win-x64/native" 
	"src/Microsoft.ML.OnnxRuntime.1.18.1/runtimes/win-x64/native"
])

# Agregar bibliotecas necesarias
env.Append(LIBS=[
    "onnxruntime"
])

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
sources = Glob("src/*.cpp")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "./demo/bin/libgdonnx_godot_loader.{}.{}.framework/libgdonnx_godot_loader.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "./demo/bin/libgdonnx_godot_loader{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
