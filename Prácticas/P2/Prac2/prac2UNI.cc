// DNI 48762664L ROCA DE TOGORES GINESTAR, JUAN
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include <sstream>
#include <iomanip>

using namespace std;

const int KMAXNIF=10;
const int KMAXNAME=50;
const int KMAXTELEPHONE=14;
const int kMAXFILENAME = 255;

struct Patient{
    string nif;
    string name;
    string telephone;
};

struct PatientBin{
    char nif[KMAXNIF];
    char name[KMAXNAME];
    char telephone[KMAXTELEPHONE];
};

struct Date{
    int day;
    int month;
    int year;
};

struct Analysis{
    unsigned int id;
    char nif[KMAXNIF];
    Date dateAnalysis;
    float weight;
    float height;
};

struct Database{
    unsigned int nextId;
    vector<Patient> patients;
    vector<Analysis> analysis;
};

enum Error {
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

// DECLARACIÓN DE FUNCIONES AUXILIARES
bool validateNIF(string &cadena);
int searchPatient(vector<Patient> patients, string &nif);
bool validateName(string &name);
bool validateTelf(string &telf);
void fixDate(int &num);

// DECLARACIÓN DE FUNCIONES
void error(Error e);
void showMenu();
void addPatient(vector<Patient>& patients);
void loadPatients(vector<Patient>& patients);
void addAnalysis(vector<Patient>& patients, vector<Analysis>& analysis, int* id);
void viewPatient(vector<Patient>& patients, vector<Analysis>& analysis);
void exportAnalysis(vector<Analysis>& analysis);
void importAnalysis(vector<Analysis>& analysis, vector<Patient>& patients, Database& data);
void deletePatient(vector<Patient>& patients, vector<Analysis>& analysis);
void statistics(vector<Patient>& patients, vector<Analysis>& analysis);
void savePatients(vector<Patient>& patients);

//MAIN
int main(int argc, char *argv[]){
    Database data;
    data.nextId=1;
    int id = data.nextId;
    char option;
    loadPatients(data.patients);
    
    bool useFile = false, showStats = false, argswrong = false;
    char fileName[kMAXFILENAME];
    
    if(argc > 1) {
        int i = 1;
        while(i < argc && !argswrong) {
            if(strcmp(argv[i], "-f") == 0) {
                if(useFile || i + 1 >= argc) {  // Verifica si ya se encontró -f o si no hay argumento después
                    argswrong = true;
                } else {
                    useFile = true;
                    strncpy(fileName, argv[i + 1], kMAXFILENAME - 1);  
                    fileName[kMAXFILENAME - 1] = '\0';
                    i += 2;  // Avanza 2 posiciones (-f y nombre archivo)
                }
            } else if(strcmp(argv[i], "-s") == 0) {
                if(showStats) {
                    argswrong = true;
                } else {
                    showStats = true;
                    i++;
                }
            } else {
                argswrong = true;
            }
        }
        
        // Verificar si -s aparece sin -f
        if(!argswrong && showStats && !useFile) {
            argswrong = true;
        }
    }
    
    if(argswrong) {
        error(ERR_ARGS);
        return 1;
    }
    
    if(useFile) {
        ifstream inputFile(fileName);
    
        if(!inputFile) {
            error(ERR_FILE);
        } else {
            string linea;
            while(getline(inputFile, linea)) {
                stringstream ss(linea);
                string nif, fecha, peso, altura, imc;
                
                getline(ss, nif, ';');
                getline(ss, fecha, ';');
                getline(ss, peso, ';');
                getline(ss, altura, ';');
                getline(ss, imc, '\n');  // Cambiado a '\n' para coincidir con el otro código
    
                if(searchPatient(data.patients, nif) != -1) {
                    Analysis newAnalysis;
                    newAnalysis.id = data.nextId;
                    data.nextId++;
                    strncpy(newAnalysis.nif, nif.c_str(), KMAXNIF - 1);
                    newAnalysis.nif[KMAXNIF - 1] = '\0';
                    newAnalysis.weight = stof(peso);
                    newAnalysis.height = stof(altura);
    
                    for(size_t i = 0; i < fecha.length(); i++) {
                        if(fecha[i] == '/') {
                            fecha[i] = ' ';
                        }
                    }
                    stringstream aux(fecha);
                    aux >> newAnalysis.dateAnalysis.day >> newAnalysis.dateAnalysis.month >> newAnalysis.dateAnalysis.year;  // Cambiado << por >>
    
                    data.analysis.push_back(newAnalysis);
                } else {
                    ofstream wrongPatients("wrong_patients.txt", ios::app);
                    if(wrongPatients.is_open()) {  // Añadida comprobación de apertura
                        wrongPatients << nif << endl;
                    }
                    wrongPatients.close();
                }
            }
            inputFile.close();
        }
    }
    
    if(showStats) {
        statistics(data.patients,data.analysis);  
        return 0;
    }

    do {
        showMenu();
        cin >> option;
        cin.ignore();
        
        switch(option){
            case '1':
                addPatient(data.patients);
                break;
            case '2':
                viewPatient(data.patients, data.analysis);
                break;
            case '3':
                deletePatient(data.patients, data.analysis);
                break;
            case '4':
                savePatients(data.patients);
                break;
            case '5':
                addAnalysis(data.patients, data.analysis, &id);
                break;
            case '6':
                exportAnalysis(data.analysis);
                break;
            case '7':
                importAnalysis(data.analysis, data.patients, data);
                break;
            case '8':
                statistics(data.patients, data.analysis);
                break;
            case 'q':
                break;
            default:
                error(ERR_OPTION);
        }
    } while(option != 'q');
   
    return 0;
}
// DEFINICIÓN DE LAS FUNCIONES TANTO AUXILIARES COMO NO
// FUNCIONES AUXILIARES
bool validateNIF(string &cadena) {                  // Función auxiliar para comprobar el NIF introducido
    if(cadena.length()!=9) {                    // Si la cadena no tiene 9 carácteres = false
        return false;
    }

    int contnum=0, contletras=0;                // Contador auxiliar 

    for(char& c : cadena) {                      // Recorrer string
        if(isdigit(c)) {
            contnum++;
        } else if(isalpha(c)) {
            contletras++;
        } 
    }
    
    for(int i = 0; i < 8; i++) {
        if(!isdigit(cadena[i])) {
            return false;
        }
    }

    if(!isalpha(cadena[8])) {
        return false;
    }

    if(contnum != 8 || contletras != 1) {
        return false;  
    }
    return true;
}
int searchPatient(vector<Patient> patients, string &nif) {      // Función auxiliar para comprobar si existe el paciente
    int aux = - 1;
    for(size_t i = 0; i < patients.size(); i++) {               // Recorrer el vector de "patients"
        if(patients[i].nif == nif) {
            aux = i;                                           // Si este existe la función devolverá la posición del vector 
        }
    }
    return aux;  // Devuelve -1 si el paciente no existe
}
bool validateName(string &name) {                   // Función auxiliar para comprobar si el nombre tiene al menos 3 carácteres
    if(name.length() < 3) {
        return false;
    } else {
        return true;
    }
}
bool validateTelf(string &telf) {               // Función auxiliar para comprobar los requesitos pedidos del teléfono del paciente
    if(telf.length() < 11 || telf.length() > 13) {      // Comprueba la correcta longitud del número de teléfono
        return false;
    }

    if(telf[0] != '+') {        // Comprueba si se ha puesta el prefijo
        return false;
    }

    for(size_t i = 1; i < telf.length(); i++) {     // Comprueba si hay una letra en el número introducido
        if(!isdigit(telf[i])) {
            return false;
        }
    }

    return true;        // Si el número es correcto, este devolverá true
}
void fixDate(int &num) {        // Función auxiliar para arreglar las fecchas y ser mostradas como se  debe 1 => 01
    string aux = to_string(num);

    int cont = 0;

    for(auto& c : aux) {
        if(isdigit(c)) {
            cont++;
        }
    }

    if(cont != 2) {
        cout << "0" << aux;
    } else {
        cout << aux;
    }
}
// FUNCIONES
void error(Error e){ // Función de mostrar errores
    switch (e){
        case ERR_ARGS: cout << "ERROR: wrong arguments" << endl;
            break;
        case ERR_FILE: cout << "ERROR: cannot open file" << endl;
            break;
        case ERR_OPTION: cout << "ERROR: wrong option" << endl;
            break;
        case ERR_PATIENT_EXISTS: cout << "ERROR: patient already exists" << endl;
            break;
        case ERR_PATIENT_NOT_EXISTS: cout << "ERROR: patient does not exist" << endl;
            break;
        case ERR_WRONG_DATE: cout << "ERROR: wrong date" << endl;
            break;
        case ERR_WRONG_NAME: cout << "ERROR: wrong name " << endl;
            break;
        case ERR_WRONG_NIF: cout << "ERROR: wrong NIF " << endl;
            break;
        case ERR_WRONG_NUMBER: cout << "ERROR: wrong number " << endl;
            break;
        case ERR_WRONG_TELEPHONE: cout << "ERROR: wrong telephone number" << endl;
    }
}
void showMenu() { // Función de mostrar el menú
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
void addPatient(vector<Patient>& patients) {  // Función de añadir paciente al vector
    Patient newPatient;                 // Variable para añadir un nuevo paciente
    bool nifVal = false;                // Variable auxiliar para comprobar que el NIF cumple todos los requesitos
    bool nameVal = false;               // Vatiable auxiliar para comprobar que el nombre cumple todos los requesitos
    bool telVal = false;                // Variable auxiliar para comprobar que el número de teléfono cumple todos los requesitos

    do {
        cout << "Enter NIF: ";                           
        getline(cin, newPatient.nif);               // Escanea el nif introducido por el usuario
        
        if(newPatient.nif.empty()) {                // Si la cadena está vacia te manda al menú principal
            return;
        }

        if(!validateNIF(newPatient.nif)) {            // Si después de la comprobación este devuelve false, imprime el error
            error(ERR_WRONG_NIF);
            nifVal = false;                
        } else {
            int a = searchPatient(patients,newPatient.nif); // variable auxiliar que recoje el valor de la función auxiliar 
            if(a != -1) {
                error(ERR_PATIENT_EXISTS);
            } else {
                nifVal = true;              // Es "true" si cumple todas las condiciones 
            } 
        }     
    } while((!nifVal));
    
    do {
        cout << "Enter name: ";
        getline(cin, newPatient.name);      // Escanea el nombre introducido por el usuario

        if(!validateName(newPatient.name)) {    // Si el nombre tiene menos de 3 carácteres devuelve false
            error(ERR_WRONG_NAME);
        } else {
            nameVal = true;         // Si cumple los requesitos, el módulo continua
        }
    } while(!nameVal);

    do {
        cout << "Enter telephone: ";
        getline(cin, newPatient.telephone);     // Escanea el número introducido por el usuario

        if(!validateTelf(newPatient.telephone)) {       // Si el num. de teléfono  no cumple los requesitos este es false
            error(ERR_WRONG_TELEPHONE);
        } else {
            telVal = true;      // Si cumple los requesitos, este será true
        }
    } while(!telVal);

    patients.push_back(newPatient);     // Añadir el paciente al vector de pacientes
}
void loadPatients(vector<Patient>& patients) {
    ifstream archivo("patients.bin", ios::binary);

    PatientBin patBin;
    while(archivo.read(reinterpret_cast<char*>(&patBin), sizeof(PatientBin))) {
        Patient saveP;
        saveP.name = patBin.name;
        saveP.nif = patBin.nif;
        saveP.telephone = patBin.telephone;
        patients.push_back(saveP);
    }

    archivo.close();
}
void addAnalysis(vector<Patient>& patients, vector<Analysis>& analysis, int* id) {
    Analysis newAnalysis;       // Variable del struct
    Database data;

    bool nifVal;        // Variable auxiliar para comprobar si el nif introducido por el usuario es valido o no

    do{
        cout << "Enter NIF: ";
        cin.getline(newAnalysis.nif, sizeof(newAnalysis.nif));      // Lee el nif introducido por el usuario

        if(newAnalysis.nif[0] == '\0') {
            return;
        }
        string texto = newAnalysis.nif;     // Hay que pasar el nif a string debido a que la funcion auxiliar llama a un string
        int a = searchPatient(patients, texto);
        if(a == -1) {
            nifVal = false;
            error(ERR_PATIENT_NOT_EXISTS);
        } else {
            nifVal = true;          // Si el nif introducido por el usuario es correcto = true
        }
    } while(!nifVal);

    bool dateVal;       // Variable auxiliar para comprobar si el nif introducido por el usuario es valido o no

    do {
        string aux;     // Variable auxiliar para escanear en el formato pedido la fecha
        cout << "Enter date (day/month/year): ";
        getline(cin, aux);

        for(char& c : aux ) {       // Reemplazar las barras del string por espacios
            if(c == '/') {
                c = ' ';
            }
        }     
        
        stringstream date(aux);
        date >> newAnalysis.dateAnalysis.day >> newAnalysis.dateAnalysis.month >> newAnalysis.dateAnalysis.year;        // Para asignar los datos introducidos 

        if(newAnalysis.dateAnalysis.day < 1 || newAnalysis.dateAnalysis.day > 31 ) {        // Comprueba si el dia introducido está bien
            dateVal = false;
        } else if(newAnalysis.dateAnalysis.month < 1 || newAnalysis.dateAnalysis.month > 12) {      // Comprueba si el mes introducido está bien
            dateVal = false;
        } else if(newAnalysis.dateAnalysis.year < 2025 || newAnalysis.dateAnalysis.year > 2050) {       // Comprueba si el año introducido está bien
            dateVal = false;
        } else {
            dateVal = true;
        }

        if(!dateVal) {
            error(ERR_WRONG_DATE);          // Si algun dato está mal, imprime por pantalla el error
        }
    } while(!dateVal);

    bool weightVal;     // Variable auxiliar para comprobar si el peso introducido por el usuario es valido o no

    do {
        cout << "Enter weight: ";
        cin >> newAnalysis.weight;      // Lee el peso introducido por el usuario

        if(newAnalysis.weight < 0) {        // Comprueba si este es positivo
            error(ERR_WRONG_NUMBER);
            weightVal = false;
        } else {
            weightVal = true;
        }
    } while(!weightVal);
    
    bool heightVal;         // Variable auxiliar para comprobar si la altura introducido por el usuario es valido o no

    do {
        cout << "Enter height: ";
        cin >> newAnalysis.height;      // Lee la altura introducida por el usuario

        if(newAnalysis.height < 0) {        // Comprueba si esta es positiva
            error(ERR_WRONG_NUMBER);
            heightVal = false;
        } else {
            heightVal = true;
        }
    } while(!heightVal);
    
    data.nextId = (*id);
    newAnalysis.id = data.nextId;
    (*id)++;

    analysis.push_back(newAnalysis);        // Añade el analisis al vector
}
void viewPatient(vector<Patient>& patients, vector<Analysis>& analysis) {  // Función de ver pacientes
    Patient searchP;            // Variable del struct
    int a;                      // Variable auxiliar 
    do {
        cout << "Enter NIF: ";
        getline(cin,searchP.nif);       // Lee el nif introducido por el usuario

        if(searchP.nif.empty()) {       // Si se introduce una cadena vacia te manda al menú 
            return;
        }
        if(!validateNIF(searchP.nif)) {
            error(ERR_WRONG_NIF);
            a = -1;
        } else {
            a = searchPatient(patients, searchP.nif);       // a=-1 si no existe
            if(a == -1) {
                error(ERR_PATIENT_NOT_EXISTS);
            }
        }
    } while(a == -1);

    for(size_t i = 0; i < patients.size(); i++) {       // Recorrer el vector de pacientes
        if(patients[i].nif == searchP.nif)  {
            cout << "NIF: " << patients[i].nif << endl
                 << "Name: " << patients[i].name << endl
                 << "Telephone: " << patients[i].telephone << endl;
            bool hasAn = false;                                     // Variable auxiliar para saber si el paciente tiene analisis
            for(size_t a = 0; a < analysis.size(); a++) {       // recorrer el vector de analisis para comprobar si el paciente tiene analisis
                if(patients[i].nif == analysis[a].nif) {        // Imprime todos los datos de los analisis que tenga ese paciente
                    if (!hasAn) {                               // Imprimir encabezado solo una vez
                        cout << "Id\tDate\t\tHeight\tWeight" << endl;
                        hasAn = true;
                    }
                    cout << analysis[a].id << "\t";
                    fixDate(analysis[a].dateAnalysis.day);
                    cout << "/";
                    fixDate(analysis[a].dateAnalysis.month);
                    cout << "/" << analysis[a].dateAnalysis.year << "\t";
                    cout << analysis[a].height << "\t"
                         << analysis[a].weight << endl;
                }
            }
        }
    }
}
void exportAnalysis(vector<Analysis>& analysis) {
    ofstream archivo("analysis.bin", ios::binary | ios::trunc);      // Abre el archivo en modo binario y sobreescribe

    if(!archivo) {          // Si este no existe te envia al menu

    } else {
        for (const auto& analBin : analysis) { // Usamos referencia constante para eficiencia
            archivo.write(reinterpret_cast<const char*>(&analBin), sizeof(Analysis)); // Guardar cada elemento
        }
    }
    archivo.close();        // Cerrar el archivo
}
void importAnalysis(vector<Analysis>& analysis, vector<Patient>& patients, Database& data) {
    ifstream archivo("analysis.bin", ios::binary);  // Abre archivo binario
    ofstream txt("wrong_patients.txt", ios::app);   // Agrega al final del archivo

    if (!archivo || !txt) {  
        error(ERR_FILE);    
    } else {
        Analysis analBin;
        while (archivo.read(reinterpret_cast<char*>(&analBin), sizeof(Analysis))) {
            string aux(analBin.nif);  // Convierte nif de char[] a string
            int a = searchPatient(patients, aux);
    
            if (a == -1) {  
                txt << aux << "\n";  // Guarda NIF en wrong_patients.txt
            } else {  
                Analysis saveAn = analBin;  // Copia la analítica
                saveAn.id = data.nextId++;  // Asigna un nuevo ID
                analysis.push_back(saveAn); // Agrega al vector analysis
            }
        }
    }
    archivo.close();
    txt.close();
}
void deletePatient(vector<Patient>& patients, vector<Analysis>& analysis) {     // Función de borrar pacientes 
    Patient deleteP;        //Variable del struct
    int a;      // Variable auxiliar 
    bool aux;
    do {
        cout << "Enter NIF: ";
        getline(cin, deleteP.nif);          // Lee el nif introducido por el usuario

        if(deleteP.nif.empty()) {           // Si este introduce una cadena vacia te envia al menú principal
            return;
        }
        if(!validateNIF(deleteP.nif)) {
            aux = false;
            error(ERR_WRONG_NIF);
        } else {
            a = searchPatient(patients, deleteP.nif);       // Se le asigna el valor resultante a la variable "a" de la comprobación de su existencia

            if(a == -1) {
                error(ERR_PATIENT_NOT_EXISTS);
                aux = false;
            } else if(a != -1) {
                for(size_t i = 0; i < patients.size(); i++) {       // For para recorrer el vector de pacientes y eliminar el paciente 
                    if(patients[i].nif == deleteP.nif) {
                        patients.erase(patients.begin() + i);
                    }
                }
                
                for(size_t n = analysis.size() - 1; n > 0; n--){        // For para recorrer el vector de los analisis y eliminar los analisis de dichoo paciente
                    if(analysis[n].nif == deleteP.nif) {
                        analysis.erase(analysis.begin() + n);
                    }
                }
                aux = true;
            }
        }
    } while(!aux);
}
void statistics(vector<Patient>& patients, vector<Analysis>& analysis) {            // Función de mostrar las estadísticas
    ofstream txt("statistics.txt", ios::trunc);

    for (const auto& a : analysis) {
        float height_m = a.height / 100;
        float imc = a.weight / (height_m * height_m);
        string comp;

        if (imc < 18.5) {
            comp = "Underweight";
        } else if (imc < 25 && imc >= 18.5) {
            comp = "Healthy";
        } else if (imc < 30 && imc >=25) {
            comp = "Overweight";
        } else {
            comp = "Obesity";
        }

        ostringstream day, month;
        day << setw(2) << setfill('0') << a.dateAnalysis.day;
        month << setw(2) << setfill('0') << a.dateAnalysis.month;

        cout << a.nif << ";"
             << day.str() << "/" 
             << month.str() << "/" 
             << a.dateAnalysis.year << ";"
             << a.weight << ";" 
             << a.height << ";" 
             << comp << endl;

        txt << a.nif << ";"
            << day.str() << "/" 
            << month.str() << "/" 
            << a.dateAnalysis.year << ";"
            << a.weight << ";" 
            << a.height << ";" 
            << comp << endl;
    }

    txt.close();
}   
void savePatients(vector<Patient>& patients) {      // Función de de guardar pacientes
    ofstream archivo("patients.bin", ios::binary | ios::trunc);      // Para abrirlo en binario y sobreescribirlo lo anteriormente escrito

    if(!archivo) {      // Si no se encuentra el archivo te manda al menú

    } else {
        for(const auto& patient : patients) {       // Para recorrer el vector. Si no hubiese que modificar el tamaño del  nombre, se podria hacer de la otra forma también
            PatientBin PatBin;      // Variable del struct de pacientes binarios
    
            strncpy(PatBin.name, patient.name.c_str(), KMAXNAME - 1);       // Copiar el nombre del paciente a la variable para ser pasado al archivo
            PatBin.name[KMAXNAME - 1] = '\0';       // Asegura una terminación nula
    
            strncpy(PatBin.nif, patient.nif.c_str(), KMAXNIF - 1);       // Copiar el nif del paciente a la variable para ser pasado al archivo
            PatBin.nif[KMAXNIF - 1] = '\0';      // asegura una terminación nula
    
            strncpy(PatBin.telephone, patient.telephone.c_str(), KMAXTELEPHONE - 1);     // Copiar el teléfono del paciente a la variable para ser pasado al archivo
            PatBin.telephone[KMAXTELEPHONE - 1] = '\0';      // Asegura un terminación nula
            
            archivo.write(reinterpret_cast<const char*>(&PatBin), sizeof(PatientBin));        // Escribe en el archivo toda la información        // Escribe 
        }
    }
    archivo.flush();
    archivo.close();        // Cerrar el archivo
}