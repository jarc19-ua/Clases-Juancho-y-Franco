// DNI 48720625R Juan Alejandro Reyes Cava
#include <iostream>
#include <fstream> // Para usar ficheros
#include <vector>  // Para usar vector
#include <cctype>  // isdigit() isalpha()
#include <string.h>
#include <cstring>
#include <sstream>

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
    ERR_FILE_NOT_EXISTS,
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
    case ERR_FILE_NOT_EXISTS:
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
        cout << "ERROR: wrong name" << endl;
        break;
    case ERR_WRONG_NIF:
        cout << "ERROR: wrong NIF" << endl;
        break;
    case ERR_WRONG_NUMBER:
        cout << "ERROR: wrong number " << endl;
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

bool Argumentos(int argc, char *argv[], bool &fichero, string &nombreFichero, bool &mostrarEstadisticas)
{
    fichero = false;
    mostrarEstadisticas = false;
    bool argumentosMal = false;

    stringstream aux;
    if (argc > 1)
    {
        int i = 1;
        while (i < argc && !argumentosMal)
        {
            if (strcmp(argv[i], "-f") == 0)
            {
                if (fichero || i + 1 >= argc)
                {
                    argumentosMal = true;
                }
                else
                {
                    fichero = true;
                    nombreFichero = (string)argv[i + 1];
                    i += 2;
                }
            }
            else if (strcmp(argv[i], "-s") == 0)
            {
                if (mostrarEstadisticas)
                {
                    argumentosMal = true;
                }
                else
                {
                    mostrarEstadisticas = true;
                    i++;
                }
            }
            else
            {
                argumentosMal = true;
            }
        }

        if (!argumentosMal && mostrarEstadisticas && !fichero)
        {
            argumentosMal = true;
        }
    }

    if (argumentosMal)
    {
        error(ERR_ARGS);
    }

    return !argumentosMal;
}

int searchPatient(string nif, Database &data)
{
    int posicion = -1;
    for (size_t i = 0; i < data.patients.size(); i++)
    {
        if (data.patients[i].nif == nif)
            posicion = i;
    }
    return posicion;
}

bool ValidarNif(string nif)
{
    bool nifValido = false;
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

    return nifValido;
}

void addPatient(Database &data)
{
    string nif;
    string name;
    string telephone;
    bool nifValido = false;
    do
    {
        cout << "Enter NIF: ";
        getline(cin, nif);
        if (nif == "")
        {
            nifValido = true;
        }
        else
        {
            nifValido = ValidarNif(nif);
            if (nifValido)
            {
                if (searchPatient(nif, data) != -1)
                {
                    error(ERR_PATIENT_EXISTS);
                    nifValido = false;
                }
                else
                {
                    bool nombreValido = false;
                    do
                    {
                        cout << "Enter name: ";
                        getline(cin, name);
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
                        cout << "Enter telephone: ";
                        getline(cin, telephone);
                        if (telephone[0] == '+' && telephone.size() <= 13 && telephone.size() >= 11)
                        {
                            telefonoValido = true;
                        }
                        else
                        {
                            error(ERR_WRONG_TELEPHONE);
                        }
                    } while (!telefonoValido);
                    Patient paciente;
                    paciente.name = name;
                    paciente.nif = nif;
                    paciente.telephone = telephone;

                    data.patients.push_back(paciente);
                    nifValido = true;
                }
            }
        }
    } while (!nifValido);
}

void viewPatient(Database &data)
{
    string nif;
    int posicion = -1;
    do
    {
        cout << "Enter NIF: ";
        getline(cin, nif);
        if (nif.empty())
        {
            return;
        }

        if (ValidarNif(nif))
        {
            posicion = searchPatient(nif, data);

            if (posicion == -1)
            {
                error(ERR_PATIENT_NOT_EXISTS);
            }
        }
    } while (posicion == -1);
    Patient paciente = data.patients[posicion];
    cout << "NIF: " << paciente.nif << endl;
    cout << "Name: " << paciente.name << endl;
    cout << "Telephone: " << paciente.telephone << endl;
    bool hayAnalisis = true;
    {

        for (size_t j = 0; j < data.analysis.size(); j++)
        {

            if (data.analysis[j].nif == paciente.nif)
            {
                if (hayAnalisis)
                {
                    cout << "Id\tDate\tHeight\tWeight" << endl;
                    hayAnalisis = false;
                }
                cout << data.analysis[j].id << "\t";
                if (data.analysis[j].dateAnalysis.day < 10 && data.analysis[j].dateAnalysis.day >= 1)
                {
                    cout << "0";
                }
                cout << data.analysis[j].dateAnalysis.day << "/";
                if (data.analysis[j].dateAnalysis.month < 10 && data.analysis[j].dateAnalysis.day >= 1)
                {
                    cout << "0";
                }
                cout << data.analysis[j].dateAnalysis.month << "/";
                cout << data.analysis[j].dateAnalysis.year << "\t";
                cout << data.analysis[j].height << "\t";
                cout << data.analysis[j].weight << endl;
            }
        }
    }
}

void deletePatient(Database &data)
{
    bool nifValido = false;
    string nif = "";

    do
    {
        cout << "Enter NIF:";
        getline(cin, nif);
        if (nif.empty())
        {
            return;
        }
        if (!ValidarNif(nif))
        {
            error(ERR_WRONG_NIF);
            nifValido = false;
        }
        else
        {
            int posicion = searchPatient(nif, data);
            if (posicion == -1)
            {
                error(ERR_PATIENT_NOT_EXISTS);
                nifValido = false;
            }
            else
            {

                for (size_t i = data.analysis.size() - 1; i > 0; i--)
                {
                    if (strcmp(data.analysis[i].nif, nif.c_str()) == 0)
                    {
                        data.analysis.erase(data.analysis.begin() + i);
                    }
                }

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
        for (size_t i = 0; i < data.patients.size(); i++)
        {
            PatientBin pacienteBinario;
            strncpy(pacienteBinario.nif, data.patients[i].nif.c_str(), KMAXNIF);
            strncpy(pacienteBinario.name, data.patients[i].name.c_str(), KMAXNAME);
            strncpy(pacienteBinario.telephone, data.patients[i].telephone.c_str(), KMAXTELEPHONE);
            pacienteBinario.name[KMAXNAME - 1] = '\0';
            pacienteBinario.nif[KMAXNIF - 1] = '\0';
            pacienteBinario.telephone[KMAXTELEPHONE - 1] = '\0';
            fichero.write((const char *)&pacienteBinario, sizeof(PatientBin));
        }
        fichero.flush();
        fichero.close();
    }
}
void addAnalysis(Database &data)
{
    string nif;
    bool nifValido = false;
    int posicion;

    do
    {
        cout << "Enter NIF:";
        getline(cin, nif);
        if (nif.empty())
        {
            return;
        }

        posicion = searchPatient(nif, data);
        if (posicion == -1)
        {
            error(ERR_PATIENT_NOT_EXISTS);
            nifValido = false;
        }
        else
        {
            nifValido = true;
        }

    } while (!nifValido);

    nifValido = true;
    Date date;
    string fecha;
    char slash;
    bool fechaValida = false;
    do
    {
        cout << "Enter date (day/month/year):";
        cin >> date.day;
        cin >> slash;
        cin >> date.month;
        cin >> slash;
        cin >> date.year;
        if (date.day >= 1 && date.day <= 31 && date.month >= 1 && date.month <= 12 && date.year >= 2025 && date.year <= 2050)
        {
            fechaValida = true;
        }
        else
        {
            error(ERR_WRONG_DATE);
        }
    } while (!fechaValida);

    bool pesoValido = false;
    float weight;
    do
    {
        cout << "Enter weight:";
        cin >> weight;
        if (weight > 0)
        {
            pesoValido = true;
        }
        else
        {
            error(ERR_WRONG_NUMBER);
        }
    } while (!pesoValido);
    bool alturaValida = false;
    float height;
    string altura;
    do
    {
        cout << "Enter height:";
        cin >> height;
        if (height > 0)
        {
            alturaValida = true;
        }
        else
        {
            error(ERR_WRONG_NUMBER);
        }
    } while (!alturaValida);

    Analysis newAnalysis;
    newAnalysis.id = data.nextId;
    data.nextId++;
    strncpy(newAnalysis.nif, nif.c_str(), KMAXNIF);
    newAnalysis.dateAnalysis = date;
    newAnalysis.weight = weight;
    newAnalysis.height = height;
    data.analysis.push_back(newAnalysis);
}

void exportAnalysis(const Database &data)
{
    ofstream fichero("analysis.bin", ios::binary);
    if (fichero.is_open())
    {
        for (size_t i = 0; i < data.analysis.size(); i++)
        {
            fichero.write((const char *)&data.analysis[i], sizeof(Analysis));
        }
        fichero.close();
    }
}

void importAnalysis(Database &data)
{

    ifstream fichero("analysis.bin", ios::binary);
    ofstream wrongPatientsFile("wrong_patients.txt", ios::app);

    if (!wrongPatientsFile.is_open() || !fichero.is_open())
    {
        error(ERR_FILE_NOT_EXISTS);
    }
    else
    {
        Analysis analysis;
        while (fichero.read((char *)&analysis, sizeof(Analysis)))
        {
            string aux(analysis.nif);
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

    fichero.close();
    wrongPatientsFile.close();
}
void Statistics(Database &data)
{
    ofstream txt("statistics.txt", ios::trunc);
    float IMC = 0;

    for (size_t i = 0; i < data.analysis.size(); i++)
    {
        cout << data.analysis[i].nif << ";";
        txt << data.analysis[i].nif << ";";

        if (data.analysis[i].dateAnalysis.day < 10 && data.analysis[i].dateAnalysis.day >= 1)
        {
            cout << "0";
            txt << "0";
        }
        cout << data.analysis[i].dateAnalysis.day << "/";
        txt << data.analysis[i].dateAnalysis.day << "/";
        if (data.analysis[i].dateAnalysis.month < 10 && data.analysis[i].dateAnalysis.day >= 1)
        {
            cout << "0";
            txt << "0";
        }
        cout << data.analysis[i].dateAnalysis.month << "/";
        txt << data.analysis[i].dateAnalysis.month << "/";

        cout << data.analysis[i].dateAnalysis.year << ";";
        txt << data.analysis[i].dateAnalysis.year << ";";

        cout << data.analysis[i].weight << ";";
        txt << data.analysis[i].weight << ";";

        cout << data.analysis[i].height << ";";
        txt << data.analysis[i].height << ";";

        float altura = data.analysis[i].height / 100;
        IMC = data.analysis[i].weight / (altura * altura);

        if (IMC < 18.5)
        {
            cout << "Underweight" << endl;
            txt << "Underweight" << endl;
        }
        if (IMC >= 18.5 && IMC < 25)
        {
            cout << "Healthy" << endl;
            txt << "Healthy" << endl;
        }
        if (IMC >= 25.0 && IMC < 30)
        {
            cout << "Overweight" << endl;
            txt << "Overweight" << endl;
        }
        if (IMC >= 30.0)
        {
            cout << "Obesity" << endl;
            txt << "Obesity" << endl;
        }
    }
}
void loadPatients(Database &data)
{
    ifstream archivo("patients.bin", ios::binary);

    PatientBin pacienteBinario;
    while (archivo.read(reinterpret_cast<char *>(&pacienteBinario), sizeof(PatientBin)))
    {
        Patient pacienteAux;
        pacienteAux.name = pacienteBinario.name;
        pacienteAux.nif = pacienteBinario.nif;
        pacienteAux.telephone = pacienteBinario.telephone;
        data.patients.push_back(pacienteAux);
    }
    archivo.close();
}

/*
Función principal: Tendrás que añadir más código tuyo
return: 0
*/
int main(int argc, char *argv[])
{

    bool fichero;
    string nombreFichero;
    bool mostrarEstadisticas = false;
    char nombreFichero2[255];
    if (Argumentos(argc, argv, fichero, nombreFichero, mostrarEstadisticas))
    {

        strcpy(nombreFichero2, nombreFichero.c_str());
        Database data;
        data.nextId = 1;
        loadPatients(data);

        if (fichero)
        {
            ifstream inputFile(nombreFichero2);

            if (!inputFile)
            {
                error(ERR_FILE_NOT_EXISTS);
            }
            else
            {
                string linea;
                while (getline(inputFile, linea))
                {
                    stringstream streamLinea(linea);
                    string nif, fecha, peso, altura, imc;

                    getline(streamLinea, nif, ';');
                    getline(streamLinea, fecha, ';');
                    getline(streamLinea, peso, ';');
                    getline(streamLinea, altura, ';');
                    getline(streamLinea, imc, '\n');

                    if (searchPatient(nif, data) != -1)
                    {
                        Analysis analisis;
                        analisis.id = data.nextId;
                        data.nextId++;
                        strncpy(analisis.nif, nif.c_str(), KMAXNIF - 1);
                        analisis.nif[KMAXNIF - 1] = '\0';
                        analisis.weight = stof(peso);
                        analisis.height = stof(altura);

                        for (size_t i = 0; i < fecha.length(); i++)
                        {
                            if (fecha[i] == '/')
                            {
                                fecha[i] = ' ';
                            }
                        }
                        stringstream streamAux(fecha);
                        streamAux >> analisis.dateAnalysis.day >> analisis.dateAnalysis.month >> analisis.dateAnalysis.year; // Cambiado << por >>

                        data.analysis.push_back(analisis);
                    }
                    else
                    {
                        ofstream wrongPatients("wrong_patients.txt", ios::app);
                        if (wrongPatients.is_open())
                        { // Añadida comprobación de apertura
                            wrongPatients << nif << endl;
                        }
                        wrongPatients.close();
                    }
                }
                inputFile.close();
            }
        }

        if (mostrarEstadisticas)
        {
            Statistics(data);
        }
        else
        {
            char option = 'q';

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
                    importAnalysis(data);
                    break;
                case '8': // Llamar a la función "statistics" para guardar las preguntas en fichero
                    Statistics(data);
                    break;
                case 'q': // Salir del programa
                    break;
                default:
                    error(ERR_OPTION);
                }
            } while (option != 'q');
        }
    }

    return 0;
}