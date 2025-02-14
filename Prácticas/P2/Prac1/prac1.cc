#include <iostream>
#include <cstdlib> // Para rand() y srand()
#include <cstring> // Para strcpy(), strcat() y strcmp()

using namespace std;
int ID=0;
const int kTEAMNAME=40; // Máximo tamaño del nombre de un equipo
const int kPLAYERNAME=50; // Máximo tamaño del nombre de un jugador
const int kPLAYERS=5; // Número de jugadores por equipo
const int kMAXTEAMS=20; // Número máximo de equipos
const int kMINTEAMS=2; // Número mínimo de equipos

// Registro para los jugadores
struct Player{
    char name[kPLAYERNAME];
    unsigned int goals;
    bool best;
};

// Registro para los equipos
struct Team{
    unsigned int id;
    char name[kTEAMNAME];
    unsigned int wins;
    unsigned int losses;
    unsigned int draws;
    unsigned int points;
    Player players[kPLAYERS];
};

// Tipos de error posibles
enum Error{
    ERR_EMPTY,
    ERR_MAX_TEAMS,
    ERR_NO_LEAGUE,
    ERR_NO_TEAMS,
    ERR_NOT_EXIST,
    ERR_NUM_TEAMS,
    ERR_OPTION
};

/* Función que muestra los mensajes de error
e: tipo de error a mostrar
return: nada
*/
void error(Error e){
    switch(e){
        case ERR_EMPTY: cout << "ERROR: empty string" << endl;
            break;
        case ERR_MAX_TEAMS: cout << "ERROR: maximum number of teams reached" << endl;
            break;
        case ERR_NO_LEAGUE: cout << "ERROR: there are no standings" << endl;
            break;
        case ERR_NO_TEAMS: cout << "ERROR: there are no teams" << endl;
            break;
        case ERR_NOT_EXIST: cout << "ERROR: team does not exist" << endl;
            break;
        case ERR_NUM_TEAMS: cout << "ERROR: wrong number of teams" << endl;
            break;
        case ERR_OPTION: cout << "ERROR: wrong option" << endl;
    }
}

/* Función que simula los goles marcados por los jugadores
team: equipo para el que vamos a simular los goles marcados
return: número de goles totales del equipo
*/
int simulateGoals(Team &team){
    int goals=0;
    
    for(int i=0;i<kPLAYERS;i++){
        int goal=rand()%2; // Genera 0 o 1 (máximo un gol por robot en cada partido)
        team.players[i].goals+=goal;
        goals+=goal;
    }

    return goals;
}

/* Función que muestra el menú de opciones
return: nada
*/
void showMenu(){
    cout << "1- Add team" << endl
         << "2- Add all teams" << endl
         << "3- Delete team" << endl
         << "4- Show teams" << endl
         << "5- Play league" << endl
         << "6- Show standings" << endl
         << "7- Show best players" << endl
         << "q- Quit" << endl
         << "Option: ";
}
Team addTeam(){
    int numeroActualEquipos=5;//no se como mirar el número de equipos
    Team equipo;
    if(numeroActualEquipos>kMAXTEAMS){
        error(ERR_MAX_TEAMS);
    }
    else{
        cout<<"Enter team name: ";
        char nombre[kTEAMNAME];
        string nombreEquipo;
        cin>>nombreEquipo;
        equipo.id=ID;
        ID++;



//MODIFICACION
        equipo.name=nombre;//ver porque da fallo
        
        for (int j = 0; j < nombreEquipo.length(); j++)
        {
            equipo.name[j]=nombreEquipo[j];
        }
        
        
        
        equipo.wins=0;
        equipo.losses=0;
        equipo.draws=0;
        equipo.points=0;

        for(int i=0;i<5;i++){
            Player jugador;
            string name=equipo.name;










//MODIFICACION
            char nombre[20];
            //char nombre[20]=name;//ver porque da fallo
            
            
            for(int j=0;j<name.length();j++)
            nombre[i]=name[i];
            
           
            










            for(int j=0;j<kPLAYERNAME;j++){
                jugador.name[j]=nombre[j];
            }
            jugador.goals=0;
            jugador.best=false;
            equipo.players[i]=jugador;
        }


    } 
    return equipo;
}
void showTeams(){
}
// Función principal. Tendrás que añadir más código tuyo
int main(){
    char option;

    srand(888); // Fija la semilla del generador de números aleatorios. ¡NO TOCAR!
    
    do{
        showMenu();
        cin >> option;
        cin.ignore(); // Para evitar que el salto de línea se quede en el buffer de teclado y luego pueda dar problemas si usas "getline"
        
        switch(option){
            case '1': // Llamar a la función "addTeam" para añadir un nuevo equipo
                break;
            case '2': // Llamar a la función "addAllTeams" para añadir todos los equipos de una vez
                break;
            case '3': // Llamar a la función "deleteTeam" para borrar un equipo
                break;
            case '4': // Llamar a la función "showTeams" para mostrar los datos de los equipos
                break;
            case '5': // Llamar a la función "playLeague" para simular los resultados de la competición
                break;
            case '6': // Llamar a la función "showStandings" para mostrar la clasificación final
                break;
            case '7': // Llamar a la función "showBestPlayers" para mostrar los mejores jugadores de cada equipo
                break;
            case 'q': break;
            default: error(ERR_OPTION);
        }
    }while(option!='q');
}
