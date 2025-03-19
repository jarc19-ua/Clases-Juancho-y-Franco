// DNI 48720625R Juan Alejandro Reyes Cava
#include <iostream>
#include <fstream> // Para usar ficheros
#include <vector>  // Para usar vector
#include <cctype>  // isdigit() isalpha()
#include <string.h>
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

int searchPatient(string nif, Database &data)
{
    int posicion = -1;
    for (int i = 0; i < data.patients.size(); i++)
    {
        if (data.patients[i].nif == nif)
            posicion = i;
    }
    return posicion;
}
void addPatient(Database &data)
{
    string nif;
    string name;
    string telephone;
    bool nifValido = false;
    do
    {
        cout << "Enter NIF:" << endl;
        cin >> nif;

        if (nif.size() == 9)
        {
            nifValido = true;
            for (int i = 0; i < 8; i++)
            {
                if (!isdigit(nif[i]))
                {
                    nifValido = false;
                }
            }
            if (!isalpha(nif[8]))
            {
                nifValido = false;
            }
        }

        if (!nifValido)
        {
            error(ERR_WRONG_NIF);
        }
        else
        {
            if (searchPatient(nif, data) == -1)
            {

                bool nombreValido = false;
                do
                {
                    cout << "Enter name:" << endl;
                    cin >> name;
                    if (name.size() >= 3)
                    {
                        nombreValido = true;
                    }
                    else
                    {
                        error(ERR_WRONG_NAME);
                    }
                } while (!nombreValido);

                bool telefonoValido = false;
                do
                {
                    cout << "Enter telephone:" << endl;
                    cin >> telephone;
                    if (telephone[0] == '+' && telephone.size() <= 13 && telephone.size() >= 11)
                    {
                        telefonoValido = true;
                    }
                    else
                    {
                        error(ERR_WRONG_TELEPHONE);
                    }
                } while (!telefonoValido);

                // Aqui los datos estan bien
                Patient paciente;
                paciente.name = name;
                paciente.nif = nif;
                paciente.telephone = telephone;

                data.patients.push_back(paciente);
            }
            else
            {
                error(ERR_PATIENT_EXISTS);
            }
        }

    } while (!nifValido);
}

void viewPatient(Database &data) // TODO: Mostrar analisis
{
    string nif;
    bool nifValido = false;
    do
    {
        cout << "Enter NIF:" << endl;
        cin >> nif;

        int posicion = searchPatient(nif, data);

        if (posicion != -1)
        {
            nifValido = true;
            Patient paciente = data.patients[posicion];
            cout << "NIF: " << paciente.nif << endl;
            cout << "Name: " << paciente.name << endl;
            cout << "Telephone: " << paciente.telephone << endl;

            bool hayAnalisys = false;

            for (int i = 0; i < data.analysis.size(); i++)
            {
                if (strcmp(data.analysis[i].nif, paciente.nif.c_str()) == 0)
                {
                    hayAnalisys = true;
                    cout << "id\tdate\theight\tweight" << endl;
                    for (int j = 0; j < data.analysis.size(); j++)
                    {
                        cout << data.analysis[j].id << data.analysis[j].weight << data.analysis[j].height;
                    }
                }
            }
        }
        else
        {
            error(ERR_PATIENT_NOT_EXISTS);
        }
    } while (!nifValido);
}

void deletePatient(Database &data)
{
    bool nifValido = false;
    string nif = "";

    do
    {
        cout << "Enter NIF:" << endl;
        cin >> nif;
        if (nif.empty())
        {
            nifValido = true;
        }
        else
        {
            int posicion = searchPatient(nif, data);
            if (posicion == -1)
            {
                error(ERR_PATIENT_NOT_EXISTS);
            }
            else
            {
                // TODO: Eliminar analisis
                data.patients.erase(data.patients.begin() + posicion);
                nifValido = true;
            }
        }
    } while (!nifValido);
}

void savePatients(const Database &data)
{
    ofstream fichero("patients.bin", ios::binary);
    if (fichero.is_open())
    {
        for (int i = 0; i < data.patients.size(); i++)
        {
            PatientBin pacienteBinario;
            strncpy(pacienteBinario.nif, data.patients[i].nif.c_str(), KMAXNIF);
            strncpy(pacienteBinario.name, data.patients[i].name.c_str(), KMAXNAME);
            strncpy(pacienteBinario.telephone, data.patients[i].telephone.c_str(), KMAXTELEPHONE);
            fichero.write((const char *)&pacienteBinario, sizeof(PatientBin));
        }
        fichero.close();
    }
}

void addAnalysis(Database &data)
{
    string nif;
    bool nifValido = false;
    do
    {
        cout << "Enter NIF: " << endl;
        cin >> nif;
        if (!nif.empty())
        {
            int posicion = searchPatient(nif, data);

            if (posicion != -1)
            {
                nifValido = true;
                // Pedimos Fecha
                Date date;
                string fecha;
                char slash; // Para leer los '/' del formato de fecha
                bool fechaValida = false;
                do
                {
                    cout << "Enter date (day/month/year):" << endl; // 12/3/2025
                    cin >> date.day;
                    cin >> slash;
                    cin >> date.month;
                    cin >> slash;
                    cin >> date.year;

                    if (date.day >= 1 && date.day <= 31 && date.month >= 1 && date.month <= 12 && date.year >= 2025 && date.year <= 2050)
                    {
                        fechaValida = true;
                        bool pesoValido = false;
                        float weight;
                        string peso;
                        // Pedimos Peso
                        do
                        {
                            cout << "Enter weight: " << endl;
                            cin >> peso;
                            weight = stof(peso);
                            if (weight > 0)
                            {
                                pesoValido = true;
                                bool alturaValida = false;
                                float height;
                                string altura;
                                do
                                {
                                    cout << "Enter height: " << endl;
                                    cin >> altura;
                                    height = stof(altura);

                                    if (height > 0)
                                    {
                                        alturaValida = true;
                                        // Crear el analisis y guardarlo en la Database
                                        Analysis newAnalysis;
                                        newAnalysis.id = data.nextId;
                                        data.nextId++;
                                        strncpy(newAnalysis.nif, nif.c_str(), KMAXNIF);
                                        newAnalysis.dateAnalysis = date;
                                        newAnalysis.weight = weight;
                                        newAnalysis.height = height;
                                        data.analysis.push_back(newAnalysis);
                                    }
                                    else
                                    {
                                        error(ERR_WRONG_NUMBER);
                                    }
                                } while (!alturaValida);
                            }
                            else
                            {
                                error(ERR_WRONG_NUMBER);
                            }
                        } while (!pesoValido);
                    }
                    else
                    {
                        error(ERR_WRONG_DATE);
                    }
                } while (!fechaValida);
            }
            else
            {
                error(ERR_PATIENT_NOT_EXISTS);
            }
        }
    } while (!nifValido);
}

void exportAnalysis(const Database &data)
{
    ofstream fichero("analysis.bin", ios::binary);
    if (fichero.is_open())
    {
        for (int i = 0; i < data.analysis.size(); i++)
        {
            fichero.write((const char *)&data.analysis[i], sizeof(Analysis));
        }
        fichero.close();
    }
}

void importAnalysis(Database &data)
{
//Leemos fichero
    ifstream fichero("analysis.bin", ios::binary);
    if (fichero.is_open())
    {
        //abrimos fichero
        ofstream wrongPatientsFile("wrong_patients.txt", ios::app);
        if (wrongPatientsFile.is_open())
        {
            while (fichero)
            {
                Analysis analysis;
                fichero.read((char *)&analysis, sizeof(Analysis));
                int posicion = searchPatient(analysis.nif, data);
                if (posicion == -1)
                {
                    wrongPatientsFile << analysis.nif << endl;
                }
                else
                {
                    analysis.id = data.nextId;
                    data.nextId++;
                    data.analysis.push_back(analysis);
                }
            }
        }
        else
        {
            error(ERR_FILE); // TODO Preguntar al profe porque en el enunciado pone ERR_FILE_NOT_EXISTS y en la practica pone ERR_FILE
        }

        fichero.close();
        wrongPatientsFile.close();
    }
    else
    {
        error(ERR_FILE); // TODO Preguntar al profe porque en el enunciado pone ERR_FILE_NOT_EXISTS y en la practica pone ERR_FILE
    }
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
            addPatient(data);
            break;
        case '2': // Llamar a la función "viewPatient" para ver la información de un paciente
            viewPatient(data);
            break;
        case '3': // Llamar a la función "deletePatient" para eliminar una ficha de paciente
            deletePatient(data);
            break;
        case '4': // Llamar a la función "savePatients" para guardar las fichas de pacientes en fichero binario
            savePatients(data);
            break;
        case '5': // Llamar a la función "addAnalysis" para anadir una analítica
            addAnalysis(data);
            break;
        case '6': // Llamar a la función "exportAnalysis" para exportar las analiticas realizadas a fichero binario
            exportAnalysis(data);
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