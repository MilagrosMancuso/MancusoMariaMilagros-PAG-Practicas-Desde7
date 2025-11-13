//
// Created by Mili on 22/09/2025.
//
#include "Gui.h"
#include "imgui/imgui.h"
#include "Camara.h"
#include <filesystem>

namespace PAG {
    GUI* GUI::instancia = nullptr;

    /**
   * Constructor
   */
    GUI::GUI(){
        // Color inicial alineado con Renderer (gris medio)
        _bgColor[0] = 0.6f;
        _bgColor[1] = 0.6f;
        _bgColor[2] = 0.6f;
    }

    /**
     * Destructor
     */
    GUI::~GUI() {}

    GUI& GUI::getInstancia() {
        if (!instancia)
            instancia = new GUI();

        return *instancia;
    }

    /** Metodo para dibujar las ventanas de color y mensaje
     *
     */
    void GUI::dibuja() {
        auto& renderer = Renderer::getInstancia();
        auto& cam = renderer.getCamara();

        ImGui::Begin("Shaders");

        // Campo de texto: usa std::string* + imgui_stdlib.h para evitar error
        ImGui::InputText("Base name##shader", _baseName, ImGuiInputTextFlags_AutoSelectAll);

        ImGui::SameLine();
        if (ImGui::Button("Load")) {
            renderer.loadShaderProgramFromBase(_baseName);
        }

        // Ayuda de rutas
        ImGui::TextWrapped("Coloca '%s-vs.glsl' y '%s-fs.glsl' en el directorio de ejecucion, ./shaders/, ../ o ../shaders/ (o define SHADER_DIR).",
                           _baseName, _baseName);

        ImGui::Separator();

        ImGui::TextUnformatted("Color de fondo");
        if (ImGui::ColorEdit3("##bg", _bgColor, ImGuiColorEditFlags_NoInputs)) {
            renderer.setColorFondo(_bgColor);
        }

        ImGui::End();

        //Ventana para los modelos

        if (ImGui::Begin("Modelos", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            static char ruta[512] = "";
            static bool suavizaNormales = true;           // flag normales (NO OLVIDARSE DE NORMALES)
            static int selec = renderer.getModeloSeleccionado();

            ImGui::TextUnformatted("Cargar OBJ:");
            ImGui::InputText("Ruta##obj", ruta, IM_ARRAYSIZE(ruta));
            ImGui::SameLine();
            if (ImGui::Button("Cargar OBJ")) {
                if (strlen(ruta) > 0) {
                    int idx = renderer.loadOBJModel(ruta, suavizaNormales);
                    if (idx >= 0) {
                        selec = idx;
                        renderer.setModeloSeleccionado(selec);
                    }
                }
            }
            ImGui::SameLine();
            ImGui::Checkbox("Suavizar normales", &suavizaNormales);

            ImGui::Separator();

            //los modelos
            auto n_modelo = renderer.listaNombreModelo();

            if (!n_modelo.empty()) {
                if (selec < 0 || selec >= (int) n_modelo.size()) {
                    selec = (int) n_modelo.size() - 1;
                    renderer.setModeloSeleccionado(selec);
                }

                //los nombres
                if (ImGui::BeginCombo("Seleccionar modelo", n_modelo[selec].c_str())) {
                    for (int i = 0; i < (int) n_modelo.size(); ++i) {
                        bool isSel = (i == selec);
                        if (ImGui::Selectable(n_modelo[i].c_str(), isSel)) {
                            selec = i;
                            renderer.setModeloSeleccionado(selec);
                        }
                        if (isSel) ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                //para borrar un modelo
                ImGui::SameLine();
                if (ImGui::Button("Eliminar")) {
                    renderer.removeModel(selec);
                    auto nn = renderer.listaNombreModelo();
                    if (nn.empty()) {
                        selec = -1;
                        renderer.setModeloSeleccionado(-1);
                    } else {
                        selec = (int) nn.size() - 1;
                        renderer.setModeloSeleccionado(selec);
                    }
                }

                ImGui::Separator();

                //transformaciones en modelos
                Modelo *m = renderer.getModelo(selec);

                if (m) {
                    static float t[3] = {0, 0, 0}; //trasla
                    static float r[3] = {0, 0, 0}; //rota
                    static float s[3] = {1, 1, 1}; //escala
                    static bool wire = false;

                    ImGui::Text("Transformaciones de modelado");
                    ImGui::DragFloat3("Traslacion ", t, 0.01f);
                    ImGui::DragFloat3("Rotacion ", r, 0.5f);
                    ImGui::DragFloat3("Escalado ", s, 0.01f, 0.001f, 100.0f);

                    if (ImGui::Button("Aplicar TRS")) {
                        // Reconstruimos M = T * Rz * Ry * Rx * S
                        glm::mat4 M(1.0f);
                        M = glm::translate(M, glm::vec3(t[0], t[1], t[2]));
                        M = glm::rotate(M, glm::radians(r[2]), glm::vec3(0, 0, 1));
                        M = glm::rotate(M, glm::radians(r[1]), glm::vec3(0, 1, 0));
                        M = glm::rotate(M, glm::radians(r[0]), glm::vec3(1, 0, 0));
                        M = glm::scale(M, glm::vec3(s[0], s[1], s[2]));
                        m->modelaMatrix() = M;
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Reset")) {
                        t[0] = t[1] = t[2] = 0;
                        r[0] = r[1] = r[2] = 0;
                        s[0] = s[1] = s[2] = 1;
                        m->modelaMatrix() = glm::mat4(1.0f);
                    }

                    if (ImGui::Checkbox("Wireframe", &wire)) {
                        m->setWireframe(wire);

                    }

                    ImGui::Text("Tris: %zu", m->cuentaTriang());


                    //MATERIALES
                    ImGui::Separator();
                    ImGui::Text("Material");

                    auto& mat = m->getMaterial();
                    ImGui::ColorEdit3("Ka (Ambiente)", (float*)&mat.Ka);
                    ImGui::ColorEdit3("Kd (Difuso)",   (float*)&mat.Kd);
                    ImGui::ColorEdit3("Ks (Especular)",(float*)&mat.Ks);
                    ImGui::SliderFloat("brillo", &mat.brillo, 1.0f, 128.0f);

                }


            } else {
                ImGui::TextUnformatted("No hay modelos cargados.");
            }
        }
        ImGui::End();

        //ventana camara
        if (ImGui::Begin("Camara", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::SetWindowFontScale(1.0f); // Cambia a 2.0f para letra doble si quieres

            static int active = 6;
            const char* items =
                    "None\0"
                    "Zoom\0"
                    "Crane\0"
                    "Dolly\0"
                    "Pan\0"
                    "Tilt\0"
                    "Orbit\0\0";
            bool cambiaT = ImGui::Combo("##MovType", &active, items);
            if (cambiaT) {
                switch (active) {
                    case 1: cam.tipo = tipoCamara::Tilt;  break;
                    case 2: cam.tipo = tipoCamara::Crane; break;
                    case 3: cam.tipo = tipoCamara::Dolly; break;
                    case 4: cam.tipo = tipoCamara::Pan;   break;
                    case 5: cam.tipo = tipoCamara::Tilt;  break;
                    case 6: cam.tipo = tipoCamara::Orbit; break;
                    default: cam.tipo = tipoCamara::None; break;
                }
            }

            ImGui::Separator();

            // Botoneras por modo
            const float dx = 5;   // paso horizontal simulado
            const float dy = 5;   // paso vertical simulado
            const float rueda = 1; // paso de "rueda"

            switch (active) {
                case 1: { // Zoom
                   ImGui::TextUnformatted("Zoom ");
                    if (ImGui::Button("+")) { cam.onScroll(+ rueda); } ImGui::SameLine();
                    if (ImGui::Button("-")) { cam.onScroll(- rueda); }

                    ImGui::SliderFloat("##Angle", &cam.campoVisY, 15.f, 90.f, "", ImGuiSliderFlags_AlwaysClamp);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(60);
                    ImGui::InputFloat("##AngleBox", &cam.campoVisY, 0, 0, "%.0f");
                    cam.campoVisY = std::clamp(cam.campoVisY, 15.0f, 90.0f);
                } break;

                case 2: { // Crane
                    cam.tipo = tipoCamara::Crane;
                    ImGui::TextUnformatted("Crane (Up/Down)");
                    if (ImGui::Button("Up"))   { cam.options(0, +dy); } ImGui::SameLine();
                    if (ImGui::Button("Down")) { cam.options(0, -dy); }
                } break;

                case 3: { // Dolly
                    cam.tipo = PAG::tipoCamara::Dolly;
                    ImGui::TextUnformatted("Direction");

                    // Fila superior: Back
                    if (ImGui::Button("^ Back ^")) { cam.options(0, +dy); }  // +dy => alejar

                    // Fila central: Left / Right
                    ImGui::Dummy(ImVec2(0, 4));
                    if (ImGui::Button("<- Left")) { cam.options(-dx, 0); }   // usa Pan lateral para sensación lateral
                    ImGui::SameLine();
                    if (ImGui::Button("Right ->")) { cam.options(+dx, 0); }

                    // Fila inferior: Front
                    ImGui::Dummy(ImVec2(0, 4));
                    if (ImGui::Button("v Front v")) { cam.options(0, -dy); } // -dy => acercar
                } break;

                case 4: { // Pan
                    cam.tipo = PAG::tipoCamara::Pan;
                    ImGui::TextUnformatted("Direction");
                    if (ImGui::Button("<- Left")) { cam.options(-dx, 0); }
                    ImGui::SameLine();
                    if (ImGui::Button("Right ->")) { cam.options(+dx, 0); }
                } break;

                case 5: { // Tilt
                    cam.tipo = PAG::tipoCamara::Tilt;
                    ImGui::TextUnformatted("Direction");
                    if (ImGui::Button("^ Up ^")) { cam.options(0, -dy); }
                    ImGui::SameLine();
                    if (ImGui::Button("v Down v")) { cam.options(0, +dy); }
                } break;

                case 6: { // Orbit
                    cam.tipo = PAG::tipoCamara::Orbit;

                    ImGui::TextUnformatted("Latitude");
                    if (ImGui::Button("^ North ^")) { cam.options(0, -dy); }  // pitch -
                    ImGui::SameLine();
                    if (ImGui::Button("v South v")) { cam.options(0, +dy); }  // pitch +

                    ImGui::Dummy(ImVec2(0, 4));
                    ImGui::TextUnformatted("Longitude");
                    if (ImGui::Button("<- West")) { cam.options(+dx, 0); }    // yaw -
                    ImGui::SameLine();
                    if (ImGui::Button("East ->")) { cam.options(-dx, 0); }    // yaw +
                } break;

                default: {
                    cam.tipo = tipoCamara::None;
                    ImGui::TextUnformatted("Seleccione un modo para habilitar controles.");
                } break;
            }
        }
        ImGui::End();


        //Ventana Log
            ImGui::Begin("Log");
        ImGui::Checkbox("Auto-scroll", &_autoLog);
        ImGui::Separator();

        const auto& mensajes = renderer.getMensaje();
        ImGui::BeginChild("LogRegion", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
        for (const auto& m : mensajes) {
            ImGui::TextUnformatted(m.c_str());
        }
        if (_autoLog && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0);
        }

        ImGui::EndChild();
        ImGui::End();
    }
}
