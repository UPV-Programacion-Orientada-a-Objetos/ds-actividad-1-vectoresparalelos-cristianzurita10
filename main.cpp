#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // Necesario para procesar cada línea del CSV
#include <limits>  // Necesario para validar la entrada numérica
using namespace std;
// Prototipos de las funciones
int contarRegistros(const string& rutaArchivo);
void cargarAtletas(const string& rutaArchivo, string nombres[], string paises[], string disciplinas[], string generos[], int medallas[], int total);
void buscarAtleta(const string nombres[], const string paises[], const int medallas[], int total);
void calcularMedallasPorPais(const string paises[], const int medallas[], int total);
void encontrarAtletaTop(const string nombres[], const int medallas[], int total);
void mostrarMenu();
int main(int argc, char* argv[]) {
    string rutaArchivo;
    if (argc < 2) {
        cout << "No se proporciono el archivo como argumento." << endl;
        cout << "Por favor, ingrese la ruta del archivo CSV: ";
        getline(cin, rutaArchivo);
    } else {
        rutaArchivo = argv[1];
    }
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo '" << rutaArchivo << "'." << endl;
        return 1; // Termina el programa con un código de error
    }
    archivo.close();
    int totalAtletas = contarRegistros(rutaArchivo);
    if (totalAtletas == 0) {
        cout << "El archivo esta vacio o solo contiene la cabecera." << endl;
        return 0;
    }
    // Declaración de los vectores paralelos estáticos
    string nombres[totalAtletas];
    string paises[totalAtletas];
    string disciplinas[totalAtletas];
    string generos[totalAtletas];
    int medallas[totalAtletas];
    cargarAtletas(rutaArchivo, nombres, paises, disciplinas, generos, medallas, totalAtletas);
    int opcion = 0;
    do {
        mostrarMenu();
        cin >> opcion;
        // Validación de entrada para el menú
        while (cin.fail()) {
            cout << "Entrada invalida. Por favor, ingrese un numero: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> opcion;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpia el buffer de entrada
        switch (opcion) {
            case 1:
                buscarAtleta(nombres, paises, medallas, totalAtletas);
                break;
            case 2:
                calcularMedallasPorPais(paises, medallas, totalAtletas);
                break;
            case 3:
                encontrarAtletaTop(nombres, medallas, totalAtletas);
                break;
            case 4:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
                break;
        }
        cout << endl;
    } while (opcion != 4);
    return 0;
}
//Muestra el menú de opciones al usuario.
void mostrarMenu() {
    cout << "----------------------------------------" << endl;
    cout << "  Registro de Atletas Olimpicos" << endl;
    cout << "----------------------------------------" << endl;
    cout << "1. Buscar informacion de un atleta" << endl;
    cout << "2. Calcular total de medallas por pais" << endl;
    cout << "3. Encontrar atleta con mas medallas de oro" << endl;
    cout << "4. Salir" << endl;
    cout << "Seleccione una opcion: ";
}
//Cuenta el número de líneas de datos en el archivo CSV (ignorando la cabecera).
int contarRegistros(const string& rutaArchivo) {
    ifstream archivo(rutaArchivo);
    string linea;
    int contador = 0;
    getline(archivo, linea); // Ignora la primera línea (cabecera)
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            contador++;
        }
    }
    archivo.close();
    return contador;
}
//carga los datos del archivo CSV en los vectores paralelos.
void cargarAtletas(const string& rutaArchivo, string nombres[], string paises[], string disciplinas[], string generos[], int medallas[], int total) {
    ifstream archivo(rutaArchivo);
    string linea, cabecera;
    getline(archivo, cabecera); // Lee y descarta la cabecera del CSV
    int i = 0;
    while (getline(archivo, linea) && i < total) {
        stringstream ss(linea);
        string medallasStr;
        getline(ss, nombres[i], ',');
        getline(ss, paises[i], ',');
        getline(ss, disciplinas[i], ',');
        getline(ss, generos[i], ',');
        getline(ss, medallasStr, ',');
        // Manejo de posible error en conversión de medallas
        try {
            medallas[i] = stoi(medallasStr);
        } catch (const std::invalid_argument& e) {
            medallas[i] = 0; // Asigna 0 si el dato no es un número
        }       
        i++;
    }
    archivo.close();
}
// Busca un atleta por nombre y muestra su información.
void buscarAtleta(const string nombres[], const string paises[], const int medallas[], int total) {
    string nombreBuscado;
    cout << "Ingrese el nombre del atleta a buscar: ";
    getline(cin, nombreBuscado);
    bool encontrado = false;
    for (int i = 0; i < total; ++i) {
        if (nombres[i] == nombreBuscado) {
            cout << "\n--- Informacion del Atleta ---" << endl;
            cout << "Nombre: " << nombres[i] << endl;
            cout << "Pais: " << paises[i] << endl;
            cout << "Medallas de Oro: " << medallas[i] << endl;
            encontrado = true;
            break; // Termina la búsqueda una vez que lo encuentra
        }
    }
    if (!encontrado) {
        cout << "Atleta '" << nombreBuscado << "' no encontrado en el registro." << endl;
    }
}
//Suma y muestra el total de medallas de oro para un país específico.
void calcularMedallasPorPais(const string paises[], const int medallas[], int total) {
    string paisBuscado;
    cout << "Ingrese el pais para calcular el total de medallas: ";
    getline(cin, paisBuscado);
    int totalMedallas = 0;
    bool paisEncontrado = false;
    for (int i = 0; i < total; ++i) {
        if (paises[i] == paisBuscado) {
            totalMedallas += medallas[i];
            paisEncontrado = true;
        }
    }
    if (paisEncontrado) {
        cout << "El total de medallas de oro para " << paisBuscado << " es: " << totalMedallas << endl;
    } else {
        cout << "No hay atletas registrados para el pais '" << paisBuscado << "'." << endl;
    }
}
//Encuentra y muestra al atleta con la mayor cantidad de medallas de oro.
void encontrarAtletaTop(const string nombres[], const int medallas[], int total) {
    if (total == 0) {
        cout << "No hay atletas registrados." << endl;
        return;
    }
    int maxMedallas = -1;
    int indiceAtletaTop = -1;
    for (int i = 0; i < total; ++i) {
        if (medallas[i] > maxMedallas) {
            maxMedallas = medallas[i];
            indiceAtletaTop = i;
        }
    }
    cout << "--- Atleta con Mas Medallas de Oro ---" << endl;
    cout << "Nombre: " << nombres[indiceAtletaTop] << endl;
    cout << "Medallas: " << medallas[indiceAtletaTop] << endl;
}