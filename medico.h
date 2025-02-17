#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip> 

class CitaMedica;

class Medico {
private:
    std::string nombre;
    int ID;
    std::string servicio;
    bool disponibilidad;

public:
    // Constructor por defecto
    Medico()
        : nombre(""), ID(0), servicio(""), disponibilidad(false) {
    }
    // Constructor
    Medico(std::string& nombre, int ID, const std::string& servicio, bool disponibilidad)
        : nombre(nombre), ID(ID), servicio(servicio), disponibilidad(disponibilidad) {
    }

    void setNombre(const std::string& nombre) { this->nombre = nombre; }
    void setID(const int& ID) { this->ID = ID; }
    void setServicio(const std::string& servicio) {
        if (esServicioValido(servicio)) {
            this->servicio = servicio;
        }
        else {
            std::cout << "Error: El servicio ingresado no es valido." << std::endl;
        }
    }
    void setDisponibilidad(const bool& disponibilidad) { this->disponibilidad = disponibilidad; }

    bool esServicioValido(const std::string& servicio) {
        std::ifstream archivo("servicios.txt");
        if (!archivo.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo servicios.txt\n";
            return false;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            // Comparar ignorando mayusculas y minusculas
            if (std::equal(servicio.begin(), servicio.end(), linea.begin(), linea.end(),
                [](char a, char b) { return std::tolower(a) == std::tolower(b); })) {
                return true;
            }
        }
        return false;
    }


    static void verMedicoPorServicio(const std::vector<Medico*>& medicos) {
        Medico tempMedico; 
        std::cin.ignore();
        while (true) {
            std::cout << "Ingrese el servicio: ";
            std::string servicio;
            std::getline(std::cin, servicio);

            for (char& c : servicio) {
                c = tolower(c);  
            }

       
            if (!tempMedico.esServicioValido(servicio)) {
                std::cout << "El servicio ingresado no es valido. Intente nuevamente.\n";
                continue; 
            }

     
            std::cout << "\n--- Medicos en el servicio: " << servicio << " ---\n";
            bool encontrado = false;

            for (const auto& medico : medicos) {

                std::string servicioMedico = medico->getServicio();
                for (char& c : servicioMedico) {
                    c = tolower(c);
                }

                if (servicioMedico == servicio) {
                    medico->mostrarMedico();
                    encontrado = true;
                }
            }

            if (encontrado) {
                break; 
            }
            else {
                std::cout << "No se encontraron medicos con el servicio: " << servicio << "\n";
                break;
            }
        }
    }

  
    static void verMedicoPorID(const std::vector<Medico*>& medicos) {
        int id;
        bool encontrado = false;
        std::cin.ignore();
        while (true) {
            std::cout << "Ingrese el ID del medico: ";
            std::string input;
            std::getline(std::cin, input);

      
            try {
                id = std::stoi(input); 
            }
            catch (const std::invalid_argument&) {
                std::cout << "Por favor, ingrese un ID valido.\n";
                continue; 
            }

          
            for (const auto& medico : medicos) {
                if (medico->getID() == id) {
                    std::cout << "Medico encontrado:\n";
                    medico->mostrarMedico();
                    encontrado = true;
                    break;
                }
            }

            if (encontrado) {
                break;
            }
            else {
                std::cout << "No se encontro un medico con ID: " << id << "\n";
                std::cout << "Desea intentarlo nuevamente? (s = 1/n = 0): ";
                char opcion;
                std::cin >> opcion;
                std::cin.ignore();
                if (opcion == 'n' || opcion == 'N' || opcion == '0') {
                    break;
                }
            }
        }
    }

    static void verMedicoPorNombre(const std::vector<Medico*>& medicos) {
        std::cin.ignore();
        while (true) {
            std::cout << "Ingrese el nombre del medico: ";
            std::string nombre;
            std::getline(std::cin, nombre);

            bool encontrado = false;

            for (const auto& medico : medicos) {
                if (medico->getNombre() == nombre) {
                    std::cout << "Medico encontrado:\n";
                    medico->mostrarMedico();
                    encontrado = true;
                }
            }

            if (encontrado) {
                break;
            }
            else {
                std::cout << "No se encontraron medicos con el nombre: " << nombre << "\n";
                std::cout << "Desea intentarlo nuevamente? (s = 1/n = 0): ";
                char opcion;
                std::cin >> opcion;
                std::cin.ignore();
                if (opcion == 'n' || opcion == 'N' || opcion == '0') {
                    break;
                }
            }
        }
    }

    static void verMedicoPorDisponibilidad(const std::vector<Medico*>& medicos) {
        std::cin.ignore();
        while (true) {
            std::cout << "Ingrese la disponibilidad del medico (1 = disponible / 0 = no disponible): ";
            std::string input;
            std::getline(std::cin, input);
            if (input.length() != 1) {
                std::cout << "Por favor, ingrese un valor valido (1 o 0).\n";
                continue;
            }

        
            char disponibilidad = input[0];
            if (disponibilidad != '0' && disponibilidad != '1') {
                std::cout << "Por favor, ingrese 1 para disponible o 0 para no disponible.\n";
                continue;
            }

            bool disponibilidadBool = (disponibilidad == '1');

            bool encontrado = false;
            for (const auto& medico : medicos) {
                if (medico->getDisponibilidad() == disponibilidadBool) {
                    if (!encontrado) {
                        std::cout << "Medicos encontrados:\n";
                    }
                    medico->mostrarMedico();
                    encontrado = true;
                }
            }

            if (!encontrado) {
                std::cout << "No se encontraron medicos.\n";
            }
            break;
        }
    }

    // FUNCIONES DEL CRUD
            // M�todo para mostrar todos los medicos
    void mostrarMedico() const {
        // Como el dato tipo int omite los ceros a la izq, se usa setw y setfill para mostrarlos
        std::cout << "Dr. " << nombre << ", ID: " << std::setw(4) << std::setfill('0') << ID << "\n"
            << "Servicio: " << servicio << "\n"
            << "Disponibilidad: " << disponibilidad << "\n"
            << "------------------" << "\n";
    }


    void registrarMedico() {
        std::cin.ignore();
        std::cout << "Nombre y apellidos del medico: ";
        std::getline(std::cin, nombre);

        std::cout << "ID del medico (1234): ";
        while (!(std::cin >> ID)) {
            std::cout << "ID invalido. Intenta de nuevo: ";
            std::cin.clear();
            std::cin.ignore();
        }
        std::cin.ignore();

        bool servicioValido = false;
        while (!servicioValido) {
            std::cout << "Servicio del medico: ";
            std::getline(std::cin, servicio);

            if (esServicioValido(servicio)) {
                servicioValido = true;
            }
            else {
                std::cout << "Error: El servicio ingresado no existe. Intenta de nuevo." << std::endl;
            }
        }

        std::cout << "Medico disponible? (1 para si, 0 para no): ";
        while (!(std::cin >> disponibilidad)) {
            std::cout << "Entrada invalida. Intenta de nuevo (1 para si, 0 para no): ";
            std::cin.clear();
            std::cin.ignore();
        }

        std::cin.ignore();
        std::cout << "Medico registrado correctamente: " << nombre << " (ID: " << ID << ")" << std::endl;
    }


    void modificarMedico(std::vector<Medico*>& medicos) {
        int idMedico;
        std::cin.ignore();
        std::cout << "Ingrese el ID del medico a editar: ";
        while (!(std::cin >> idMedico)) {
            std::cout << "Entrada invalida. Intente de nuevo: ";
            std::cin.clear();
            std::cin.ignore();
        }

        auto it = std::find_if(medicos.begin(), medicos.end(),
            [&idMedico](Medico* m) { return m->getID() == idMedico; });

        if (it != medicos.end()) {
            Medico* medico = *it;
            std::cout << "Medico encontrado: \n";
            std::cout << "Dr. " << medico->nombre << "\n"
                << "ID: " << std::setw(4) << std::setfill('0') << medico->ID << "\n"
                << "Servicio: " << medico->servicio << "\n"
                << "Disponibilidad: " << medico->disponibilidad << "\n";

            std::cout << "Ingrese el nuevo nombre del medico: ";
            std::cin.ignore();
            std::getline(std::cin, medico->nombre);
            std::cout << "Ingrese el nuevo ID del medico (1234): ";
            while (!(std::cin >> medico->ID)) {
                std::cout << "ID invalido. Intenta de nuevo: ";
                std::cin.clear();
                std::cin.ignore();
            }
            std::cin.ignore();

            bool servicioValido = false;
            while (!servicioValido) {
                std::cout << "Ingrese el nuevo servicio del medico: ";
                std::getline(std::cin, medico->servicio);

                if (esServicioValido(medico->servicio)) {
                    servicioValido = true;
                }
                else {
                    std::cout << "Error: El servicio ingresado no existe. Intenta de nuevo." << std::endl;
                }
            }
            std::cout << "Esta disponible el medico? (1 = Si / 0 = No): ";
            while (!(std::cin >> medico->disponibilidad)) {
                std::cout << "Entrada invalida. Intenta de nuevo (1 = Si / 0 = No): ";
                std::cin.clear();
                std::cin.ignore();
            }

            std::cout << "Medico modificado correctamente.\n";
        }
        else {
            std::cout << "Medico no encontrado.\n";
        }
    }


    static void eliminarMedico(std::vector<Medico*>& medicos) {
        std::cin.ignore();
        int idMedico;
        std::cout << "Ingrese el ID del medico a eliminar: ";
        while (!(std::cin >> idMedico)) {
            std::cout << "Entrada invalida. Intente de nuevo: ";
            std::cin.clear();
            std::cin.ignore();
        }
        std::cin.ignore();
        std::cout << "Estas seguro de eliminar al medico con ID " << idMedico
            << "? (1 para confirmar, 0 para cancelar): ";
        int confirmar;
        while (!(std::cin >> confirmar) || (confirmar != 1 && confirmar != 0)) { // Validar entrada
            std::cout << "Entrada invalida. Intente de nuevo (1 para confirmar, 0 para cancelar): ";
            std::cin.clear();
            std::cin.ignore();
        }
        std::cin.ignore();
        if (confirmar == 1) {
            auto it = std::remove_if(medicos.begin(), medicos.end(), [&idMedico](Medico* m) {
                return m->getID() == idMedico;
                });

            if (it != medicos.end()) {
                delete* it;
                medicos.erase(it);
                std::cout << "Medico eliminado correctamente.\n";
            }
            else {
                std::cout << "No se encontro un medico con ese ID.\n";
            }
        }
        else {
            std::cout << "Eliminacion cancelada.\n";
        }
    }

    // Metodos de consulta (getters)
    std::string getNombre() const { return nombre; }
    int getID() const { return ID; }
    std::string getServicio() const { return servicio; }
    bool getDisponibilidad() const { return disponibilidad; }
    // Metodo estatico para obtener el n�mero de campos requeridos
    static int camposRequeridos() { return 4; } // Nombre, ID, Servicio, Disponibilidad

};
