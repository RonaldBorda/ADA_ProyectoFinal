#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

// =============================================
// CLASE TAREA - REPRESENTA CADA PROCESO DEL SISTEMA
// =============================================
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
        timestamp.pop_back(); // Remover salto de línea
    }
};

// =============================================
// SKEW HEAP - ALGORITMO PRINCIPAL DE PRIORIZACIÓN
// =============================================
class SkewHeap {
private:
    struct Nodo {
        Tarea tarea;
        Nodo* izquierdo;
        Nodo* derecho;
        Nodo(Tarea t) : tarea(t), izquierdo(nullptr), derecho(nullptr) {}
    };
    
    Nodo* raiz;
    
    // Función recursiva para mezclar dos heaps
    Nodo* mezclar(Nodo* h1, Nodo* h2) {
        if (!h1) return h2;
        if (!h2) return h1;
        
        // Mantener propiedad de heap: mayor prioridad en raíz
        if (h1->tarea.prioridad < h2->tarea.prioridad) {
            swap(h1, h2);
        }
        
        // Intercambiar hijos y mezclar recursivamente
        swap(h1->izquierdo, h1->derecho);
        h1->izquierdo = mezclar(h2, h1->izquierdo);
        
        return h1;
    }
    
public:
    vector<Tarea> tareasInsertadas;
    vector<string> logEjecucion;
    int cpuTotalUtilizado;
    
    SkewHeap() : raiz(nullptr), cpuTotalUtilizado(0) {}
    
    // INSERTAR nueva tarea en el heap
    void insertar(Tarea tarea) {
        Nodo* nuevoNodo = new Nodo(tarea);
        raiz = mezclar(raiz, nuevoNodo);
        
        tareasInsertadas.push_back(tarea);
        string mensaje = "INSERTADA: " + tarea.nombre + " (Prioridad: " + to_string(tarea.prioridad) + ") - " + tarea.timestamp;
        logEjecucion.push_back(mensaje);
        cout << "✅ " << mensaje << endl;
    }
    
    // EXTRAER la tarea de MÁXIMA prioridad
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
        cout << "🎯 " << mensaje << endl;
        
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

// =============================================
// FUNCIONES AUXILIARES
// =============================================

// FUNCIÓN PARA MOSTRAR MENÚ DE TAREAS PREDEFINIDAS
void mostrarMenuTareas() {
    cout << "\n📋 TAREAS PREDEFINIDAS TESLA:\n";
    cout << "1. Control de Frenos de Emergencia (Prioridad: 10)\n";
    cout << "2. Detección de Peatones (Prioridad: 10)\n";
    cout << "3. Navegación en Tiempo Real (Prioridad: 8)\n";
    cout << "4. Monitoreo de Sensores (Prioridad: 7)\n";
    cout << "5. Sistema de Entretenimiento (Prioridad: 3)\n";
    cout << "6. Actualización de Mapas (Prioridad: 4)\n";
    cout << "7. Tarea Personalizada (Ingresar manualmente)\n";
    cout << "8. Mostrar Estadísticas Actuales\n";
    cout << "0. Finalizar ingreso de tareas\n";
}

// FUNCIÓN PARA GENERAR REPORTE EN TXT
void generarReporteTXT(const vector<string>& logEjecucion, int cpuTotal, int tareasEjecutadas, int tareasPendientes, const string& nombreArchivo = "reporte_tesla.txt") {
    ofstream archivo(nombreArchivo);
    
    if (!archivo) {
        cout << "❌ Error al crear el reporte" << endl;
        return;
    }
    
    time_t ahora = time(0);
    
    // Encabezado del reporte
    archivo << "=============================================\n";
    archivo << "         TESLA - REPORTE DEL SISTEMA\n";
    archivo << "     Asignación de Recursos Computacionales\n";
    archivo << "=============================================\n\n";
    archivo << "Fecha de generación: " << ctime(&ahora);
    archivo << "Total de tareas ejecutadas: " << tareasEjecutadas << "\n";
    archivo << "Tareas pendientes: " << tareasPendientes << "\n";
    archivo << "CPU total utilizado: " << cpuTotal << " unidades\n";
    archivo << "Total de eventos: " << logEjecucion.size() << "\n\n";
    archivo << "DETALLE DE EJECUCIÓN:\n";
    archivo << "=====================\n";
    
    // Contenido del log
    for (size_t i = 0; i < logEjecucion.size(); i++) {
        archivo << i + 1 << ". " << logEjecucion[i] << "\n";
    }
    
    archivo << "\n=============================================\n";
    archivo << "           FIN DEL REPORTE\n";
    archivo << "=============================================\n";
    
    archivo.close();
    cout << "📄 Reporte generado: " << nombreArchivo << endl;
}

// FUNCIÓN PARA INGRESAR TAREAS MANUALMENTE
Tarea ingresarTareaManual() {
    string nombre;
    int prioridad, cpu;
    
    cout << "\n➕ INGRESAR TAREA PERSONALIZADA:\n";
    cout << "Nombre de la tarea: ";
    cin.ignore();
    getline(cin, nombre);
    
    cout << "Prioridad (1-10, 10=máxima): ";
    cin >> prioridad;
    
    // Validar prioridad
    while (prioridad < 1 || prioridad > 10) {
        cout << "❌ Prioridad debe ser entre 1 y 10: ";
        cin >> prioridad;
    }
    
    cout << "Recursos CPU necesarios: ";
    cin >> cpu;
    
    return Tarea(nombre, prioridad, cpu);
}

// FUNCIÓN PARA MOSTRAR ESTADÍSTICAS
void mostrarEstadisticas(const SkewHeap& gestorTareas) {
    cout << "\n📊 ESTADÍSTICAS ACTUALES:\n";
    cout << "Tareas pendientes: " << gestorTareas.getTareasPendientes() << "\n";
    cout << "Tareas ejecutadas: " << gestorTareas.getTareasEjecutadas() << "\n";
    cout << "CPU total utilizado: " << gestorTareas.cpuTotalUtilizado << "\n";
    cout << "Total de eventos: " << gestorTareas.logEjecucion.size() << "\n";
    
    if (gestorTareas.getTareasPendientes() + gestorTareas.getTareasEjecutadas() > 0) {
        int porcentaje = (gestorTareas.getTareasEjecutadas() * 100) / 
                        (gestorTareas.getTareasPendientes() + gestorTareas.getTareasEjecutadas());
        cout << "Progreso: " << porcentaje << "% completado\n";
    }
}

// FUNCIÓN RECURSIVA PARA TAREAS CON DEPENDENCIAS
void procesarTareaRecursivo(string nombreTarea, int nivel = 0) {
    string indentacion(nivel * 2, ' ');
    cout << indentacion << "🔁 PROCESANDO: " << nombreTarea << " (Nivel: " << nivel << ")" << endl;
    
    // SIMULAR SUBTAREAS (RECURSIVIDAD)
    if (nombreTarea == "Esquivar Obstaculo") {
        cout << indentacion << "  📋 Subtareas detectadas..." << endl;
        procesarTareaRecursivo("Calcular Trayectoria", nivel + 1);
        procesarTareaRecursivo("Preparar Sistema de Direccion", nivel + 1);
        procesarTareaRecursivo("Ajustar Control de Velocidad", nivel + 1);
    }
    else if (nombreTarea == "Recalcular Ruta") {
        procesarTareaRecursivo("Obtener Datos GPS", nivel + 1);
        procesarTareaRecursivo("Analizar Trafico", nivel + 1);
    }
    
    cout << indentacion << "✅ COMPLETADO: " << nombreTarea << endl;
}

// =============================================
// FUNCIÓN PRINCIPAL - INTERFAZ DINÁMICA
// =============================================
void sistemaGestionRecursos() {
    cout << "==========================================\n";
    cout << "🚗 SISTEMA TESLA - ASIGNACIÓN DE RECURSOS\n";
    cout << "     (Versión Dinámica con Reportes)\n";
    cout << "==========================================\n";
    
    SkewHeap gestorTareas;
    int opcion;
    
    // FASE 1: INGRESO DINÁMICO DE TAREAS
    cout << "\n📥 FASE 1: CONFIGURACIÓN DE TAREAS\n";
    cout << "==================================\n";
    
    do {
        mostrarMenuTareas();
        cout << "\nSeleccione una opción: ";
        cin >> opcion;
        
        switch (opcion) {
            case 1: 
                gestorTareas.insertar(Tarea("Control de Frenos de Emergencia", 10, 30)); 
                break;
            case 2: 
                gestorTareas.insertar(Tarea("Detección de Peatones", 10, 25)); 
                break;
            case 3: 
                gestorTareas.insertar(Tarea("Navegación en Tiempo Real", 8, 20)); 
                break;
            case 4: 
                gestorTareas.insertar(Tarea("Monitoreo de Sensores", 7, 15)); 
                break;
            case 5: 
                gestorTareas.insertar(Tarea("Sistema de Entretenimiento", 3, 10)); 
                break;
            case 6: 
                gestorTareas.insertar(Tarea("Actualización de Mapas", 4, 12)); 
                break;
            case 7: 
                gestorTareas.insertar(ingresarTareaManual()); 
                break;
            case 8:
                mostrarEstadisticas(gestorTareas);
                break;
            case 0: 
                cout << "Finalizando ingreso de tareas...\n"; 
                break;
            default: 
                cout << "❌ Opción inválida\n";
        }
        
        cout << endl;
    } while (opcion != 0);
    
    // FASE 2: EJECUCIÓN AUTOMÁTICA
    if (!gestorTareas.estaVacia()) {
        cout << "\n🎯 FASE 2: EJECUCIÓN POR PRIORIDAD\n";
        cout << "==================================\n";
        cout << "El sistema ejecutará las tareas en orden de prioridad...\n\n";
        
        int contador = 1;
        while (!gestorTareas.estaVacia()) {
            cout << "[" << contador << "] ";
            gestorTareas.extraerMaxima();
            contador++;
            
            // Simular pequeño delay para efecto visual
            #ifdef _WIN32
                system("timeout 1 > nul");
            #else
                system("sleep 1");
            #endif
        }
        
        // FASE 3: DEMOSTRACIÓN DE RECURSIVIDAD
        cout << "\n🔄 FASE 3: DEMOSTRACIÓN DE RECURSIVIDAD\n";
        cout << "=====================================\n";
        cout << "Simulando tarea compleja con dependencias...\n\n";
        procesarTareaRecursivo("Esquivar Obstaculo");
    }
    
    // FASE 4: GENERACIÓN DE REPORTE
    cout << "\n📊 FASE 4: GENERACIÓN DE REPORTE\n";
    cout << "===============================\n";
    
    string nombreArchivo;
    cout << "Ingrese nombre para el reporte (o Enter para 'reporte_tesla.txt'): ";
    cin.ignore();
    getline(cin, nombreArchivo);
    
    if (nombreArchivo.empty()) {
        nombreArchivo = "reporte_tesla.txt";
    } else if (nombreArchivo.find(".txt") == string::npos) {
        nombreArchivo += ".txt";
    }
    
    generarReporteTXT(gestorTareas.logEjecucion, 
                     gestorTareas.cpuTotalUtilizado,
                     gestorTareas.getTareasEjecutadas(),
                     gestorTareas.getTareasPendientes(),
                     nombreArchivo);
    
    cout << "\n==========================================\n";
    cout << "✅ SISTEMA COMPLETADO EXITOSAMENTE\n";
    cout << "   Reporte guardado: " << nombreArchivo << "\n";
    cout << "==========================================\n\n";
    
    cout << "Presione Enter para continuar...";
    cin.get();
}

// =============================================
// FUNCIÓN MAIN
// =============================================
int main() {
    sistemaGestionRecursos();
    return 0;
}
