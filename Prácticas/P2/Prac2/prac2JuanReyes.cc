// DNI 48720625R Juan Alejandro Reyes Cava
#include <iostream>
#include <fstream> // Para usar ficheros
#include <vector>  // Para usar vector
#include <cctype>  // isdigit() isalpha()
using namespace std;

const int KMAXNIF = 10;
const int KMAXNAME = 50;
const int KMAXTELEPHONE = 14;

// Registro para los pacientes
struct Patient
{
    string nif;
    string name;
    string telephone;
};

// Registro para los pacientes en fichero binario
struct PatientBin
{
    char nif[KMAXNIF];
    char name[KMAXNAME];
    char telephone[KMAXTELEPHONE];
};

// Registro para las fechas
struct Date
{
    int day;
    int month;
    int year;
};

// Registro para las analíticas
struct Analysis
{
    unsigned int id;
    char nif[KMAXNIF];
    Date dateAnalysis;
    float weight;
    float height;
};

// Registro para la base de datos
struct Database
{
    unsigned int nextId;
    vector<Patient> patients;
    vector<Analysis> analysis;
};

// Tipos de errores posibles
enum Error
{
    ERR_ARGS,
    ERR_FILE,
    ERR_OPTION,
    ERR_PATIENT_EXISTS,
    ERR_PATIENT_NOT_EXISTS,
    ERR_WRONG_DATE,
    ERR_WRONG_NAME,
    ERR_WRONG_NIF,
    ERR_WRONG_NUMBER,
    ERR_WRONG_TELEPHONE
};

/*
Función que muestra los distintos tipos de errores posibles
e: tipo de error a mostrar
return: void
*/
void error(Error e)
{
    switch (e)
    {
    case ERR_ARGS:
        cout << "ERROR: wrong arguments" << endl;
        break;
    case ERR_FILE:
        cout << "ERROR: cannot open file" << endl;
        break;
    case ERR_OPTION:
        cout << "ERROR: wrong option" << endl;
        break;
    case ERR_PATIENT_EXISTS:
        cout << "ERROR: patient already exists" << endl;
        break;
    case ERR_PATIENT_NOT_EXISTS:
        cout << "ERROR: patient does not exist" << endl;
        break;
    case ERR_WRONG_DATE:
        cout << "ERROR: wrong date" << endl;
        break;
    case ERR_WRONG_NAME:
        cout << "ERROR: wrong name " << endl;
        break;
    case ERR_WRONG_NIF:
        cout << "ERROR: wrong NIF " << endl;
        break;
    case ERR_WRONG_NUMBER:
        cout << "ERROR: wrong name " << endl;
        break;
    case ERR_WRONG_TELEPHONE:
        cout << "ERROR: wrong telephone number" << endl;
    }
}

/*
Función que muestra el menú de opciones
return: void
*/
void showMenu()
{
    cout << "1- Add patient" << endl
         << "2- View Patient" << endl
         << "3- Delete patient" << endl
         << "4- Save patients" << endl
         << "5- Add analysis" << endl
         << "6- Export analysis" << endl
         << "7- Import analysis" << endl
         << "8- Statistics" << endl
         << "q- Quit" << endl
         << "Option: ";
}

void addPatient()
{
    string nif;
    string name;
    string telephone;

    bool nifValido = false;
        
    do{

            cout << "Enter NIF:" << endl;
            cin >> nif;
            if (nif.size() != 9)
            {
                error(ERR_WRONG_NIF);
            }
            else
            {
                nifValido = true;
            for(int i = 0; i<8; i++){
                
                if(isdigit(nif[i])==false){
                    nifValido=false;
                        // falta comprobar si la ultima es una letra
                }
            }

         
            }
        }
        while (nifValido == false);

        cout << "Enter name:" << endl;
        cin >> name;
        // Realizar comprobaciones searchPatient

        cout << "Enter telephone:" << endl;
        cin >> telephone;
        // Realizar comprobaciones
        /**/

        // Aqui los datos estan bien
        Patient paciente;
        paciente.name = name;
        paciente.nif = nif;
        paciente.telephone = telephone;
    }

void viewPatient()
{
    string nif;

    cout << "Enter NIF:" << endl;
    cin >> nif;
}
/*
Función principal: Tendrás que añadir más código tuyo
return: 0
*/
int main(int argc, char *argv[])
{
    Database data;
    data.nextId = 1;
    char option;

    do
    {
        showMenu();
        cin >> option;
        cin.ignore();

        switch (option)
        {
        case '1': // Llamar a la función "addPatient" para añadir una nueva ficha de paciente
            break;
        case '2': // Llamar a la función "viewPatient" para ver la información de un paciente
            break;
        case '3': // Llamar a la función "deletePatient" para eliminar una ficha de paciente
            break;
        case '4': // Llamar a la función "savePatients" para guardar las fichas de pacientes en fichero binario
            break;
        case '5': // Llamar a la función "addAnalysis" para anadir una analítica
            break;
        case '6': // Llamar a la función "exportAnalysis" para exportar las analiticas realizadas a fichero binario
            break;
        case '7': // Llamar a la función "importAnalysis" para importar las analiticas en fichero binario
            break;
        case '8': // Llamar a la función "statistics" para guardar las preguntas en fichero
            break;
        case 'q': // Salir del programa
            break;
        default:
            error(ERR_OPTION);
        }
    } while (option != 'q');

    return 0;
}