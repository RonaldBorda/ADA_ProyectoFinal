#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

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


}
void mostrarMenu(){
    mostrarLogo();
    mostrarEstadoVehiculo();
    cout<<"Bienvenido Hugo\n";
    cout<<"Menú Principal - TESLA\n";
    cout << "[1] Navegación y Planificación de Rutas\n";
    cout << "[2] Gestión de Batería y Energía\n";
    cout << "[3] Diagnóstico Predictivo de Fallos\n";
    cout << "[4] Gestión de Recursos Computacionales\n";
    cout << "[5] Configuración del Vehículo\n";
    cout << "[6] Asistencia de Emergencia\n";
    cout << "[0] Apagar Sistema\n\n";
}

int main(){
    int opcion;
    do{
        mostrarMenu();
        cout<<"Seleccionar opción: ";
        cin>>opcion;
        cin.ignore();

        switch (opcion)
        {
        case 1:
            sistemaNavegacion();
            break;
        case 0:
            mostrarLogo();
            cout << "Apagando sistema...\n";
            cout << "Gracias por conducir con nosotros.\n\n";
            break;
        default:
            cout<<"Opción invalida";
            break;
        }
    }while(opcion != 0);
    return 0;
}