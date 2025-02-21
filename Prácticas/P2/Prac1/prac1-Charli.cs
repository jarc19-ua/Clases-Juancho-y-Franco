#include <iostream>
#include <cstdlib> // Para rand() y srand()
#include <cstring> // Para strcpy(), strcat() y strcmp()

using namespace std;

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

void addTeam(Team teams[], int &numTeams, int &nextId);
void deleteTeam(Team teams[], int &numTeams);
void showTeams(Team teams[], int numTeams);
void showBestPlayers(Team teams[], int numTeams, bool jugo);

void addAllTeams(Team teams[], int &numTeams, int &nextId, bool &jugo);
void playLeague(Team teams[], int numTeams, bool &jugo);
void showStandings(Team teams[], int numTeams, bool jugo);

// Función principal. Tendrás que añadir más código tuyo
int main(){

    Team teams[kMAXTEAMS];
    int numTeams = 0;
    int nextId = 0;
    bool played = false;

    char option;

    srand(888); // Fija la semilla del generador de números aleatorios. ¡NO TOCAR!
    
    do{
        showMenu();
        cin >> option;
        cin.ignore(); // Para evitar que el salto de línea se quede en el buffer de teclado y luego pueda dar problemas si usas "getline"
        
        switch(option){
            case '1': addTeam(teams, numTeams, nextId); // Llamar a la función "addTeam" para añadir un nuevo equipo
                break;
            case '2': addAllTeams(teams, numTeams, nextId, played); // Llamar a la función "addAllTeams" para añadir todos los equipos de una vez
                break;
            case '3': deleteTeam(teams, numTeams);// Llamar a la función "deleteTeam" para borrar un equipo
                break;
            case '4': showTeams(teams, numTeams);// Llamar a la función "showTeams" para mostrar los datos de los equipos
                break;
            case '5': playLeague(teams, numTeams, played);// Llamar a la función "playLeague" para simular los resultados de la competición
                break;
            case '6': showStandings(teams, numTeams, played);// Llamar a la función "showStandings" para mostrar la clasificación final
                break;
            case '7': showBestPlayers(teams, numTeams, played);// Llamar a la función "showBestPlayers" para mostrar los mejores jugadores de cada equipo
                break;
            case 'q': break;
            default: error(ERR_OPTION);
        }
    }while(option!='q');
}

void addTeam(Team teams[], int &numTeams, int &nextId){
    if(numTeams == kMAXTEAMS){
        error(ERR_MAX_TEAMS); // si ha llegado al limite de la liga
    }
    else{
        //rellenamos numTeam
        teams[numTeams].id = nextId;
        nextId++;
        teams[numTeams].wins = 0;
        teams[numTeams].losses = 0;
        teams[numTeams].draws = 0;
        teams[numTeams].points = 0;

        cout << "Enter team name:";
        cin.getline(teams[numTeams].name , kTEAMNAME);
        if(strlen(teams[numTeams].name) == 0){ // si esta vacio el nombre
            sprintf(teams[numTeams].name, "Team_%d", nextId - 1);
        }
        //creaMos players
        for(int i = 0; i < kPLAYERS; i++){
            teams[numTeams].players[i].best = false;
            teams[numTeams].players[i].goals = 0;
            sprintf(teams[numTeams].players[i].name, "%s-R%d", teams[numTeams].name, i + 1);
        }
        numTeams++;
    }
}

void addAllTeams(Team teams[], int &numTeams, int &nextId, bool &played){
   char res;
   char number[3];
   bool fill;
   if(numTeams > 0){
        do{
            cout << "Do you want to delete existing teams (y/n)? ";
            cin >> res;
            cin.ignore();
        } while(res != 'y' && res != 'Y' && res != 'n' && res != 'N');
        if(res == 'Y' || res == 'y'){
            numTeams = 0;
            nextId = 0;
            fill = true;
        } else {
            fill = false;
        }

    }
    else {
        fill = true;
    }
    if (fill == true){
            do{
                cout << "Enter number of teams: ";
                cin >> numTeams;
                cin.ignore();
                if(numTeams < kMINTEAMS || numTeams > kMAXTEAMS){
                    error(ERR_NUM_TEAMS);
                }
            } while(numTeams < kMINTEAMS || numTeams > kMAXTEAMS);
            played = false;
            for(int i = 0; i < numTeams; i++){
                sprintf(teams[i].name, "Team_%d", i);
                teams[i].wins = teams[i].losses = teams[i].draws = teams[i].points = 0;
                for(int j = 0; j < kPLAYERS; j++){
                    teams[i].players[j].goals = 0;
                    teams[i].players[j].best = false;
                    strcpy(teams[i].players[j].name, teams[i].name);
                    strcat(teams[i].players[j].name, "-R");
                    sprintf(number, "%d", j + 1);
                    strcat(teams[i].players[j].name, number);
                }
            }
            nextId = numTeams;

        }
   
} 

int searchTeam(Team teams[], int numTeams, char teamName[]){
    int pos;
    pos = -1;
    for(int i = 0; i < numTeams && pos == -1; i++){
        if(strcmp(teams[i].name, teamName) == 0){
            pos = i;
        }
    }
    return pos;
}

void showAllTeams(Team teams[], int numTeams){
    for (int i = 0; i < numTeams; i++){
        cout << "Name: " << teams[i].name << endl;
        cout << "Wins: " << teams[i].wins << endl;
        cout << "Losses: " << teams[i].losses << endl;
        cout << "Draws: " << teams[i].draws << endl;
        cout << "Points: " << teams[i].points << endl;
        for (int j = 0; j < kPLAYERS; j++){
            cout << teams[i].players[j].name 
            << ": " << teams[i].players[j].goals
            << " goals" << endl;
        }
    }
}

void showTeam(Team teams[], int numTeams, int pos){
    cout << "Name: " << teams[pos].name << endl;
    cout << "Wins: " << teams[pos].wins << endl;
    cout << "Losses: " << teams[pos].losses << endl;
    cout << "Draws: " << teams[pos].draws << endl;
    cout << "Points: " << teams[pos].points << endl;
    for (int j = 0; j < kPLAYERS; j++){
        cout << teams[pos].players[j].name 
        << ": " << teams[pos].players[j].goals
        << " goals" << endl;
    }
    
}

void showTeams(Team teams[], int numTeams){
    char teamName[kTEAMNAME];
    int pos;
    bool seguir;

    if(numTeams == 0){
        error(ERR_NO_TEAMS);
    }
    else{
        seguir = true;
        while(seguir){
            cout << "Enter team name: ";
            cin.getline(teamName, kTEAMNAME);
            if(teamName[0] != '\0'){
                pos = searchTeam(teams, numTeams, teamName);
                if(pos == -1){
                    error(ERR_NOT_EXIST);
                }
                else{
                    showTeam(teams, numTeams, pos);
                    seguir = false;
                }
            }
            else{
                seguir = false;
                showAllTeams(teams, numTeams);
            }
        }
    }
}

void deleteTeam(Team teams[], int &numTeams){
    int pos;
    bool seguir;
    char teamName[kTEAMNAME];

    if(numTeams == 0){
        error(ERR_NO_TEAMS);
    }
    else{
        seguir = true;
        while(seguir){
            cout << "Enter team name: ";
            cin.getline(teamName, kTEAMNAME);
            if(teamName[0] != '\0'){
                pos = searchTeam(teams, numTeams, teamName);
                if(pos == -1){
                    error(ERR_NOT_EXIST);
                }
                else{
                    for(int i = pos; i < numTeams - 1; i++){
                        teams[i] = teams[i+1];
                    }
                    numTeams--;
                    seguir = false;
                }
            }
            else{
                seguir = false;
                error(ERR_EMPTY);
            }
        }
    }
}

void showBestPlayers(Team teams[], int numTeams, bool played){
    if(!played){
        error(ERR_NO_LEAGUE);
    }
    else{
        for(int i = 0; i < numTeams; i++){
            cout << teams[i].name << "|";
            for(int j = 0; j < kPLAYERS; j++){
                if(teams[i].players[j].best == true){
                    cout << teams[i].players[j].name << "|"
                         << teams[i].players[j].goals << endl;
                }
            }
        }
    }
}

void playLeague(Team teams[], int numTeams, bool &played){
    int a, b;

    if(numTeams < kMINTEAMS){
        error(ERR_NUM_TEAMS);
    }
    else{

        //vaciamos las stats
        for(int i = 0; i < numTeams; i++){
            teams[i].wins = teams[i].losses = teams[i].draws = teams[i].points = 0;
            for(int j = 0; j < kPLAYERS; j++){
                teams[i].players[j].goals = 0;
                teams[i].players[j].best = false;
            }
        }

        played = true;
        //hacemos que jueguen
        for(int i = 0; i < numTeams - 1; i++){
            for(int j = i + 1; j < numTeams; j++){
                a = simulateGoals(teams[i]);
                b = simulateGoals(teams[j]);

                if(a == b){
                    teams[i].draws++;
                    teams[j].draws++;
                    teams[i].points += 1;
                    teams[j].points += 1;
                }
                else{
                    if(a > b){
                        teams[i].wins++;
                        teams[j].losses++;
                        teams[i].points += 3;
                    }
                    else{
                        teams[i].losses++;
                        teams[j].wins++;
                        teams[j].points += 3;
                    }
                }
            }
        }
        int posMejor;
        for(int i = 0; i < numTeams; i++){
            
            posMejor = 0;
            for(int j = 1; j < kPLAYERS; j++){
                if(teams[i].players[j].goals > teams[i].players[posMejor].goals){
                    posMejor = j;    
                }
            }
            teams[i].players[posMejor].best = true;
        }
    }
}

void showStandings(Team teams[], int numTeams, bool played){


  

    if(!played){
        error(ERR_NO_LEAGUE);
    }
    else{
    //copio la liga
    Team ordenado[kMAXTEAMS];
        for(int i = 0; i < numTeams; i++){
            ordenado[i] = teams[i]; // copio cada uno
        }
        //ordeno
        for(int i = 1; i < numTeams; i++){
            for(int j = numTeams - 1; j >= i; j--){ //arrastra el mayor hasta la posicion i - 1
                if(ordenado[j].points > ordenado[j-1].points){
                    Team aux = ordenado[j];
                    ordenado[j] = ordenado[j-1];
                    ordenado[j-1] = aux;
                }
            }
        }

        //imprimo
        for(int i = 0; i < numTeams; i++){
            cout << ordenado[i].name << "|"
                 << ordenado[i].wins << "|"
                 << ordenado[i].draws << "|"
                 << ordenado[i].losses << "|"
                 << ordenado[i].points << endl;
        }       
    }
}