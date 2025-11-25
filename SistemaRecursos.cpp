#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

// CLASE TAREA PARA REPRESENTAR CADA PROCESO
class Tarea {
public:
    string nombre;
    int prioridad;  // 1-10 (10 = maxima prioridad)
    int cpu_necesario;
    
    Tarea(string n, int p, int cpu) {
        nombre = n;
        prioridad = p;
        cpu_necesario = cpu;
    }
    
    bool operator<(const Tarea& otra) const {
        return prioridad < otra.prioridad;
    }
};

// SKEW HEAP (TU ALGORITMO PRINCIPAL)
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
    SkewHeap() : raiz(nullptr) {}
    
    void insertar(Tarea tarea) {
        Nodo* nuevoNodo = new Nodo(tarea);
        raiz = mezclar(raiz, nuevoNodo);
        cout << "Tarea INSERTADA: " << tarea.nombre << " (Prioridad: " << tarea.prioridad << ")" << endl;
    }
    
    Tarea extraerMaxima() {
        if (!raiz) {
            throw runtime_error("No hay tareas en el heap");
        }
        
        Tarea maxima = raiz->tarea;
        raiz = mezclar(raiz->izquierdo, raiz->derecho);
        cout << "EJECUTANDO: " << maxima.nombre << " (Prioridad: " << maxima.prioridad << ")" << endl;
        return maxima;
    }
    
    bool estaVacia() {
        return raiz == nullptr;
    }
};

// FUNCION RECURSIVA (TU SEGUNDO ALGORITMO)
void procesarTareaRecursivo(string nombreTarea, int nivel = 0) {
    string indentacion(nivel * 2, ' ');
    cout << indentacion << "PROCESANDO: " << nombreTarea << endl;
    
    if (nombreTarea == "Esquivar Obstaculo") {
        procesarTareaRecursivo("Calcular Trayectoria", nivel + 1);
        procesarTareaRecursivo("Preparar Direccion", nivel + 1);
        procesarTareaRecursivo("Ajustar Velocidad", nivel + 1);
    }
    
    cout << indentacion << "COMPLETADO: " << nombreTarea << endl;
}

// TU FUNCION PRINCIPAL
void sistemaGestionRecursos() {
    cout << "==========================================" << endl;
    cout << "SISTEMA DE ASIGNACION DE RECURSOS TESLA" << endl;
    cout << "==========================================" << endl;
    
    SkewHeap gestorTareas;
    
    cout << "\n1. INSERTANDO TAREAS:" << endl;
    gestorTareas.insertar(Tarea("Reproducir Musica", 2, 5));
    gestorTareas.insertar(Tarea("Navegacion", 6, 15));
    gestorTareas.insertar(Tarea("Deteccion de Peaton", 10, 30));
    gestorTareas.insertar(Tarea("Control de Frenos", 10, 25));
    
    cout << "\n2. EJECUCION POR PRIORIDAD:" << endl;
    while (!gestorTareas.estaVacia()) {
        gestorTareas.extraerMaxima();
    }
    
    cout << "\n3. DEMOSTRACION RECURSIVIDAD:" << endl;
    procesarTareaRecursivo("Esquivar Obstaculo");
    
    cout << "\nPresiona Enter para continuar...";
    cin.get();
}
