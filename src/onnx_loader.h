#ifndef  ONNX_LOADER_H /*AGENTE_LEANDRO_H*/
#define ONNX_LOADER_H /*AGENTE_LEANDRO_H*/

#include <godot_cpp/godot.hpp>
#include  <godot_cpp/classes/node.hpp>
#include "onnxruntime_cxx_api.h"

namespace godot {

    class ONNXLoader : public Node {
        GDCLASS(ONNXLoader, Node)

    private:
        Ort::Env env;
        Ort::Session* session;
        Ort::SessionOptions session_options;
        std::vector<const char*> input_node_names;
        std::vector<const char*> output_node_names;

    public:
        static void _bind_methods();
        void _init(); // our initializer called by Godot
        void load_model(String model_path);
        Array predict(Array input_data);
        void predictions_process(int model_type);/*proceso_predicciones*/

        ONNXLoader();
        ~ONNXLoader();
    };

}

#endif
