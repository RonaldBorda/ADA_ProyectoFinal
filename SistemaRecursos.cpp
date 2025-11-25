#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

// DECLARACIONES para que funcione con SistemaRecursos.cpp
void limpiarVentana();
void mostrarLogo();
void sistemaGestionRecursos();  // ← TU NUEVA FUNCIÓN

void limpiarVentana()
{
    #ifdef _WIN32 
        system("cls");
    #else 
        system("clear");
    #endif
}

void mostrarLogo(){
    cout << "_______________________________________________\n";
    cout << "                    TESLA\n";
    cout << "              Sistema de Control\n";
    cout << "_______________________________________________\n\n";
}

void mostrarEstadoVehiculo(){
    cout << "Estado del vehículo:\n";
    cout << "Batería: 70%\n";
    cout << "Ubicación: La Molina, Lima\n";
    cout << "Temperatura: 22°C\n";
    cout << "Autonomía: 380 km\n\n";
}

void sistemaNavegacion(){
    limpiarVentana();
    mostrarLogo();
    cout<<"Sistema de Navegación TESLA\n";
    string origen, destino;
    cout<<"Ingresar punto de origen:";
    getline(cin, origen);
    cout<<"Ingresar punto de destino:";
    getline(cin, destino);

    cout << "\nCalculando ruta óptima...\n";
    cout << "Origen: " << origen << "\n";
    cout << "Destino: " << destino << "\n\n";

    cout << "Presiona Enter para continuar...";
    cin.get();
}

// FUNCIONES VACÍAS PARA LAS OTRAS OPCIONES (por ahora)
void gestionBateria() {
    limpiarVentana();
    mostrarLogo();
    cout << "🔋 SISTEMA DE GESTIÓN DE BATERÍA\n";
    cout << "Función en desarrollo...\n";
    cout << "Presiona Enter para continuar...";
    cin.get();
}

void diagnosticoFallos() {
    limpiarVentana();
    mostrarLogo();
    cout << "⚠️ SISTEMA DE DIAGNÓSTICO PREDICTIVO\n";
    cout << "Función en desarrollo...\n";
    cout << "Presiona Enter para continuar...";
    cin.get();
}

void configuracionVehiculo() {
    limpiarVentana();
    mostrarLogo();
    cout << "⚙️ CONFIGURACIÓN DEL VEHÍCULO\n";
    cout << "Función en desarrollo...\n";
    cout << "Presiona Enter para continuar...";
    cin.get();
}

void asistenciaEmergencia() {
    limpiarVentana();
    mostrarLogo();
    cout << "🆘 ASISTENCIA DE EMERGENCIA\n";
    cout << "Función en desarrollo...\n";
    cout << "Presiona Enter para continuar...";
    cin.get();
}

void mostrarMenu(){
    mostrarLogo();
    mostrarEstadoVehiculo();
    cout<<"Bienvenido Hugo\n";
    cout<<"Menú Principal - TESLA\n";
    cout << "[1] Navegación y Planificación de Rutas\n";
    cout << "[2] Gestión de Batería y Energía\n";
    cout << "[3] Diagnóstico Predictivo de Fallos\n";
    cout << "[4] Gestión de Recursos Computacionales\n";  // ← TU OPCIÓN
    cout << "[5] Configuración del Vehículo\n";
    cout << "[6] Asistencia de Emergencia\n";
    cout << "[0] Apagar Sistema\n\n";
}

int main(){
    int opcion;
    do{
        limpiarVentana();
        mostrarMenu();
        cout<<"Seleccionar opción: ";
        cin>>opcion;
        cin.ignore();

        switch (opcion)
        {
        case 1:
            sistemaNavegacion();
            break;
        case 2:
            gestionBateria();
            break;
        case 3:
            diagnosticoFallos();
            break;
        case 4:
            sistemaGestionRecursos();  // ← TU SISTEMA
            break;
        case 5:
            configuracionVehiculo();
            break;
        case 6:
            asistenciaEmergencia();
            break;
        case 0:
            limpiarVentana();
            mostrarLogo();
            cout << "Apagando sistema...\n";
            cout << "Gracias por conducir con nosotros.\n\n";
            break;
        default:
            cout<<"Opción invalida. Presiona Enter...";
            cin.get();
            break;
        }
    }while(opcion != 0);
    return 0;
}
