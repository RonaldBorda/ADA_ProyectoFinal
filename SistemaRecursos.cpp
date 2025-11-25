#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <sstream>
using namespace std;

// CLASE TAREA
class Tarea {
public:
    string nombre;
    int prioridad;
    int cpu_necesario;
    string estado;
    
    Tarea(string n, int p, int cpu) : nombre(n), prioridad(p), cpu_necesario(cpu), estado("Pendiente") {}
    
    bool operator<(const Tarea& otra) const {
        return prioridad < otra.prioridad;
    }
    
    string toString() const {
        return nombre + " | Prioridad: " + to_string(prioridad) + 
               " | CPU: " + to_string(cpu_necesario) + "% | Estado: " + estado;
    }
};

// SKEW HEAP CORREGIDO
class SkewHeap {
private:
    struct Nodo {
        Tarea tarea;
        Nodo* izquierdo;
        Nodo* derecho;
        Nodo(Tarea t) : tarea(t), izquierdo(nullptr), derecho(nullptr) {}
    };
    
    Nodo* raiz;
    int tareasPendientes;
    int tareasEjecutadas;
    
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
    
    void destruirHeap(Nodo* nodo) {
        if (nodo) {
            destruirHeap(nodo->izquierdo);
            destruirHeap(nodo->derecho);
            delete nodo;
        }
    }
    
public:
    SkewHeap() : raiz(nullptr), tareasPendientes(0), tareasEjecutadas(0) {}
    
    ~SkewHeap() {
        destruirHeap(raiz);
    }
    
    void insertar(Tarea tarea) {
        Nodo* nuevoNodo = new Nodo(tarea);
        raiz = mezclar(raiz, nuevoNodo);
        tareasPendientes++;
        cout << "✅ Tarea INSERTADA: " << tarea.nombre << " (Prioridad: " << tarea.prioridad << ")" << endl;
    }
    
    Tarea extraerMaxima() {
        if (!raiz) {
            throw runtime_error("No hay tareas en el heap");
        }
        
        Tarea maxima = raiz->tarea;
        raiz = mezclar(raiz->izquierdo, raiz->derecho);
        tareasPendientes--;
        tareasEjecutadas++;
        cout << "🎯 EJECUTANDO: " << maxima.nombre << " (Prioridad: " << maxima.prioridad << ")" << endl;
        return maxima;
    }
    
    // MÉTODOS CONST - ahora pueden ser usados con objetos constantes
    int getTareasPendientes() const {
        return tareasPendientes;
    }
    
    int getTareasEjecutadas() const {
        return tareasEjecutadas;
    }
    
    bool estaVacia() const {
        return raiz == nullptr;
    }
    
    int getTotalTareas() const {
        return tareasPendientes + tareasEjecutadas;
    }
};

// FUNCIÓN DE ESTADÍSTICAS CORREGIDA
void mostrarEstadisticas(const SkewHeap& gestorTareas) {
    cout << "\n=== ESTADÍSTICAS DEL SISTEMA ===\n";
    cout << "Tareas pendientes: " << gestorTareas.getTareasPendientes() << "\n";
    cout << "Tareas ejecutadas: " << gestorTareas.getTareasEjecutadas() << "\n";
    cout << "Total de tareas: " << gestorTareas.getTotalTareas() << "\n";
    
    int total = gestorTareas.getTotalTareas();
    if (total > 0) {
        int porcentaje = (gestorTareas.getTareasEjecutadas() * 100) / total;
        cout << "Progreso: " << porcentaje << "% completado\n";
    } else {
        cout << "Progreso: 0% (sin tareas)\n";
    }
    
    cout << "================================\n";
}

// GESTOR DE REPORTES
class GestorReportes {
private:
    string nombreArchivo;
    
    string obtenerFechaHora() {
        time_t ahora = time(nullptr);
        tm* tiempoLocal = localtime(&ahora);
        stringstream ss;
        ss << put_time(tiempoLocal, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
public:
    GestorReportes(string nombre = "reporte_tesla.txt") : nombreArchivo(nombre) {}
    
    void generarReporte(const SkewHeap& gestorTareas, const string& titulo = "REPORTE SISTEMA TESLA") {
        ofstream archivo(nombreArchivo);
        
        if (!archivo.is_open()) {
            throw runtime_error("Error al crear archivo de reporte");
        }
        
        archivo << "==========================================\n";
        archivo << "         " << titulo << "\n";
        archivo << "==========================================\n";
        archivo << "Fecha y hora: " << obtenerFechaHora() << "\n";
        archivo << "Tareas pendientes: " << gestorTareas.getTareasPendientes() << "\n";
        archivo << "Tareas ejecutadas: " << gestorTareas.getTareasEjecutadas() << "\n";
        archivo << "Total de tareas: " << gestorTareas.getTotalTareas() << "\n";
        archivo << "==========================================\n\n";
        
        archivo << "ESTADÍSTICAS:\n";
        archivo << "-------------\n";
        
        int total = gestorTareas.getTotalTareas();
        if (total > 0) {
            int porcentaje = (gestorTareas.getTareasEjecutadas() * 100) / total;
            archivo << "Progreso del sistema: " << porcentaje << "%\n";
        } else {
            archivo << "Progreso del sistema: 0%\n";
        }
        
        archivo << "\nSistema " << (gestorTareas.estaVacia() ? "INACTIVO" : "ACTIVO") << "\n";
        
        archivo.close();
        cout << "📄 Reporte generado: " << nombreArchivo << endl;
    }
};

// INTERFAZ DE USUARIO
class InterfazUsuario {
public:
    static void mostrarMenu() {
        cout << "\n==========================================" << endl;
        cout << "🚗 SISTEMA TESLA - GESTIÓN DE TAREAS" << endl;
        cout << "==========================================" << endl;
        cout << "1. Agregar nueva tarea" << endl;
        cout << "2. Ejecutar siguiente tarea" << endl;
        cout << "3. Mostrar estadísticas" << endl;
        cout << "4. Generar reporte" << endl;
        cout << "5. Salir" << endl;
        cout << "==========================================" << endl;
        cout << "Seleccione una opción: ";
    }
    
    static Tarea solicitarTarea() {
        string nombre;
        int prioridad, cpu;
        
        cout << "\n--- AGREGAR NUEVA TAREA ---" << endl;
        
        cout << "Nombre de la tarea: ";
        cin.ignore();
        getline(cin, nombre);
        
        cout << "Prioridad (1-10, 10=máxima): ";
        cin >> prioridad;
        
        while (prioridad < 1 || prioridad > 10) {
            cout << "Error: La prioridad debe ser entre 1 y 10: ";
            cin >> prioridad;
        }
        
        cout << "Recurso CPU necesario (%): ";
        cin >> cpu;
        
        while (cpu < 1 || cpu > 100) {
            cout << "Error: El CPU debe ser entre 1% y 100%: ";
            cin >> cpu;
        }
        
        return Tarea(nombre, prioridad, cpu);
    }
};

// SISTEMA PRINCIPAL
int main() {
    cout << "==========================================" << endl;
    cout << "🚗 SISTEMA TESLA - GESTIÓN INTELIGENTE" << endl;
    cout << "==========================================" << endl;
    
    SkewHeap gestorTareas;
    GestorReportes gestorReportes;
    int opcion;
    
    do {
        InterfazUsuario::mostrarMenu();
        cin >> opcion;
        
        try {
            switch (opcion) {
                case 1:
                    gestorTareas.insertar(InterfazUsuario::solicitarTarea());
                    break;
                case 2:
                    if (!gestorTareas.estaVacia()) {
                        gestorTareas.extraerMaxima();
                    } else {
                        cout << "⚠️  No hay tareas pendientes." << endl;
                    }
                    break;
                case 3:
                    mostrarEstadisticas(gestorTareas);
                    break;
                case 4:
                    gestorReportes.generarReporte(gestorTareas);
                    break;
                case 5:
                    cout << "👋 Saliendo del sistema..." << endl;
                    break;
                default:
                    cout << "❌ Opción inválida!" << endl;
            }
        } catch (const exception& e) {
            cout << "❌ Error: " << e.what() << endl;
        }
        
    } while (opcion != 5);
    
    return 0;
}
