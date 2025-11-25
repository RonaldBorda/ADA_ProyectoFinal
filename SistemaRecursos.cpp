#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>

// INCLUIR FTXUI - La librería que descubriste
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"

using namespace ftxui;
using namespace std;

// CLASE TAREA - Representa cada proceso del sistema Tesla
class Tarea {
public:
    string nombre;
    int prioridad;  // 1-10 (10 = máxima prioridad)
    int cpu_necesario;
    string timestamp;
    bool ejecutada;
    
    Tarea(string n, int p, int cpu) {
        nombre = n;
        prioridad = p;
        cpu_necesario = cpu;
        ejecutada = false;
        
        // Generar timestamp automático
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back();
    }
};

// SKEW HEAP - Algoritmo principal de priorización
class SkewHeap {
private:
    struct Nodo {
        Tarea tarea;
        Nodo* izquierdo;
        Nodo* derecho;
        Nodo(Tarea t) : tarea(t), izquierdo(nullptr), derecho(nullptr) {}
    };
    
    Nodo* raiz;
    
    Nodo* mezclar(Nodo* h1, Nodo* h2) {
        if (!h1) return h2;
        if (!h2) return h1;
        
        if (h1->tarea.prioridad < h2->tarea.prioridad) {
            swap(h1, h2);
        }
        
        swap(h1->izquierdo, h1->derecho);
        h1->izquierdo = mezclar(h2, h1->izquierdo);
        
        return h1;
    }
    
public:
    vector<Tarea> tareasInsertadas;
    vector<string> logEjecucion;
    int cpuTotalUtilizado;
    
    SkewHeap() : raiz(nullptr), cpuTotalUtilizado(0) {}
    
    void insertar(Tarea tarea) {
        Nodo* nuevoNodo = new Nodo(tarea);
        raiz = mezclar(raiz, nuevoNodo);
        
        tareasInsertadas.push_back(tarea);
        string mensaje = "INSERTADA: " + tarea.nombre + " (Prioridad: " + to_string(tarea.prioridad) + ")";
        logEjecucion.push_back(mensaje);
    }
    
    Tarea extraerMaxima() {
        if (!raiz) {
            throw runtime_error("No hay tareas en el heap");
        }
        
        Tarea maxima = raiz->tarea;
        raiz = mezclar(raiz->izquierdo, raiz->derecho);
        
        // Marcar como ejecutada
        for (auto& tarea : tareasInsertadas) {
            if (tarea.nombre == maxima.nombre && tarea.timestamp == maxima.timestamp) {
                tarea.ejecutada = true;
                break;
            }
        }
        
        cpuTotalUtilizado += maxima.cpu_necesario;
        string mensaje = "EJECUTADA: " + maxima.nombre + " (Prioridad: " + to_string(maxima.prioridad) + ")";
        logEjecucion.push_back(mensaje);
        
        return maxima;
    }
    
    bool estaVacia() {
        return raiz == nullptr;
    }
    
    int getTareasPendientes() {
        int count = 0;
        for (const auto& tarea : tareasInsertadas) {
            if (!tarea.ejecutada) count++;
        }
        return count;
    }
    
    int getTareasEjecutadas() {
        int count = 0;
        for (const auto& tarea : tareasInsertadas) {
            if (tarea.ejecutada) count++;
        }
        return count;
    }
};

// FUNCIÓN PARA CREAR LA INTERFAZ FTXUI
void crearInterfazFTXUI() {
    auto screen = ScreenInteractive::Fullscreen();
    SkewHeap gestorTareas;
    
    // Estado de la aplicación
    vector<string> tareasPredefinidas = {
        "Control de Frenos de Emergencia (10)",
        "Detección de Peatones (10)", 
        "Navegación en Tiempo Real (8)",
        "Monitoreo de Sensores (7)",
        "Sistema de Entretenimiento (3)",
        "Actualización de Mapas (4)"
    };
    
    int tarea_seleccionada = 0;
    string nombre_tarea_personalizada = "";
    int prioridad_personalizada = 5;
    int cpu_personalizado = 15;
    bool ejecucion_automatica = false;
    string mensaje_estado = "Sistema listo";
    
    // COMPONENTES DE LA INTERFAZ
    auto selector_tareas = Radiobox(&tareasPredefinidas, &tarea_seleccionada);
    
    InputOption opcion_nombre;
    opcion_nombre.placeholder = "Nombre de la tarea";
    auto input_nombre = Input(&nombre_tarea_personalizada, opcion_nombre);
    
    auto slider_prioridad = Slider("Prioridad: ", &prioridad_personalizada, 1, 10, 1);
    auto slider_cpu = Slider("CPU: ", &cpu_personalizado, 1, 100, 1);
    
    auto boton_agregar = Button("➕ Agregar Tarea", [&] {
        if (tarea_seleccionada == 6) { // Tarea personalizada
            if (!nombre_tarea_personalizada.empty()) {
                gestorTareas.insertar(Tarea(nombre_tarea_personalizada, prioridad_personalizada, cpu_personalizado));
                mensaje_estado = "Tarea personalizada agregada: " + nombre_tarea_personalizada;
                nombre_tarea_personalizada = "";
            }
        } else {
            vector<int> prioridades = {10, 10, 8, 7, 3, 4};
            vector<int> cpus = {30, 25, 20, 15, 10, 12};
            string nombre = tareasPredefinidas[tarea_seleccionada].substr(0, tareasPredefinidas[tarea_seleccionada].find(" ("));
            gestorTareas.insertar(Tarea(nombre, prioridades[tarea_seleccionada], cpus[tarea_seleccionada]));
            mensaje_estado = "Tarea predefinida agregada: " + nombre;
        }
    });
    
    auto boton_ejecutar = Button("🎯 Ejecutar Siguiente", [&] {
        if (!gestorTareas.estaVacia()) {
            gestorTareas.extraerMaxima();
            mensaje_estado = "Tarea ejecutada";
        } else {
            mensaje_estado = "No hay tareas pendientes";
        }
    });
    
    auto boton_ejecutar_auto = Button("⚡ Ejecución Automática", [&] {
        ejecucion_automatica = true;
    });
    
    auto boton_reporte = Button("📊 Generar Reporte", [&] {
        ofstream archivo("reporte_tesla_ftxui.txt");
        archivo << "REPORTE TESLA - SISTEMA FTXUI\n";
        archivo << "==============================\n";
        for (const auto& log : gestorTareas.logEjecucion) {
            archivo << log << "\n";
        }
        archivo.close();
        mensaje_estado = "Reporte generado: reporte_tesla_ftxui.txt";
    });
    
    auto boton_salir = Button("🚪 Salir", [&] {
        screen.Exit();
    });
    
    // LAYOUT PRINCIPAL
    auto layout = Container::Vertical({
        selector_tareas,
        input_nombre,
        slider_prioridad,
        slider_cpu,
        Container::Horizontal({
            boton_agregar,
            boton_ejecutar,
            boton_ejecutar_auto,
            boton_reporte,
            boton_salir
        })
    });
    
    // RENDERIZADO
    auto renderer = Renderer(layout, [&] {
        auto panel_tareas = window(text("📋 GESTIÓN DE TAREAS"), 
            vbox({
                selector_tareas->Render(),
                separator(),
                text("Tarea Personalizada:"),
                input_nombre->Render(),
                slider_prioridad->Render(),
                slider_cpu->Render()
            })
        );
        
        // PANEL DE ESTADÍSTICAS CON GRÁFICOS FTXUI
        int tareas_pendientes = gestorTareas.getTareasPendientes();
        int tareas_ejecutadas = gestorTareas.getTareasEjecutadas();
        int total_tareas = tareas_pendientes + tareas_ejecutadas;
        int porcentaje_ejecutado = total_tareas > 0 ? (tareas_ejecutadas * 100) / total_tareas : 0;
        
        auto panel_estadisticas = window(text("📊 ESTADÍSTICAS EN TIEMPO REAL"),
            vbox({
                hbox({text("Tareas Pendientes: "), text(to_string(tareas_pendientes))}),
                hbox({text("Tareas Ejecutadas: "), text(to_string(tareas_ejecutadas))}),
                hbox({text("CPU Total Utilizado: "), text(to_string(gestorTareas.cpuTotalUtilizado))}),
                separator(),
                text("Progreso de Ejecución:"),
                gauge(porcentaje_ejecutado / 100.0) | color(Color::Green),
                hbox({text("["), gauge(porcentaje_ejecutado / 100.0) | color(Color::Cyan), text("] "), text(to_string(porcentaje_ejecutado) + "%")})
            })
        );
        
        // PANEL DE LOGS
        vector<Element> elementos_log;
        for (const auto& log : gestorTareas.logEjecucion) {
            elementos_log.push_back(text(log));
        }
        
        auto panel_logs = window(text("📝 LOGS DE EJECUCIÓN"),
            vbox(elementos_log) | frame | vscroll_indicator | flex
        );
        
        // PANEL DE BOTONES
        auto panel_botones = window(text("🎮 CONTROLES"),
            vbox({
                hbox({
                    boton_agregar->Render(),
                    boton_ejecutar->Render(),
                    boton_ejecutar_auto->Render()
                }),
                hbox({
                    boton_reporte->Render(),
                    boton_salir->Render()
                })
            })
        );
        
        // LAYOUT FINAL
        return vbox({
            text("🚗 TESLA - SISTEMA DE ASIGNACIÓN DE RECURSOS (FTXUI)") | bold | center,
            separator(),
            hbox({
                panel_tareas | flex,
                separator(),
                panel_estadisticas | flex,
            }),
            separator(),
            hbox({
                panel_logs | flex,
                separator(),
                panel_botones | flex,
            }),
            separator(),
            text(mensaje_estado) | color(Color::Yellow) | center
        });
    });
    
    // HILO PARA EJECUCIÓN AUTOMÁTICA
    thread hilo_ejecucion([&]() {
        while (true) {
            if (ejecucion_automatica && !gestorTareas.estaVacia()) {
                gestorTareas.extraerMaxima();
                this_thread::sleep_for(chrono::seconds(2));
                screen.PostEvent(Event::Custom);
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    });
    
    screen.Loop(renderer);
    hilo_ejecucion.detach();
}

// FUNCIÓN PRINCIPAL
int main() {
    crearInterfazFTXUI();
    return 0;
}
