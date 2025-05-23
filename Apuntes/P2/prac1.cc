#include <iostream>
#include <cstdlib> // Para rand() y srand()
#include <cstring> // Para strcpy(), strcat() y strcmp()
#include <vector>


using namespace std;

const int kTEAMNAME = 40;   // Máximo tamaño del nombre de un equipo
const int kPLAYERNAME = 50; // Máximo tamaño del nombre de un jugador
const int kPLAYERS = 5;     // Número de jugadores por equipo
const int kMAXTEAMS = 20;   // Número máximo de equipos
const int kMINTEAMS = 2;    // Número mínimo de equipos

// Registro para los jugadores
struct Player
{
    char name[kPLAYERNAME];
    unsigned int goals;
    bool best;
};

// Registro para los equipos
struct Team
{
    unsigned int id;
    char name[kTEAMNAME];
    unsigned int wins;
    unsigned int losses;
    unsigned int draws;
    unsigned int points;
    Player players[kPLAYERS];
};

// Tipos de error posibles
enum Error
{
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
void error(Error e)
{
    switch (e)
    {
    case ERR_EMPTY:
        cout << "ERROR: empty string" << endl;
        break;
    case ERR_MAX_TEAMS:
        cout << "ERROR: maximum number of teams reached" << endl;
        break;
    case ERR_NO_LEAGUE:
        cout << "ERROR: there are no standings" << endl;
        break;
    case ERR_NO_TEAMS:
        cout << "ERROR: there are no teams" << endl;
        break;
    case ERR_NOT_EXIST:
        cout << "ERROR: team does not exist" << endl;
        break;
    case ERR_NUM_TEAMS:
        cout << "ERROR: wrong number of teams" << endl;
        break;
    case ERR_OPTION:
        cout << "ERROR: wrong option" << endl;
    }
}

/* Función que simula los goles marcados por los jugadores
team: equipo para el que vamos a simular los goles marcados
return: número de goles totales del equipo
*/
int simulateGoals(Team &team)
{
    int goals = 0;

    for (int i = 0; i < kPLAYERS; i++)
    {
        int goal = rand() % 2; // Genera 0 o 1 (máximo un gol por robot en cada partido)
        team.players[i].goals += goal;
        goals += goal;
    }

    return goals;
}

/* Función que muestra el menú de opciones
return: nada
*/
void showMenu()
{
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
void addTeam(int &contadorActualEquipos, int &ID, vector<Team> &equipos)
{

    if (contadorActualEquipos > kMAXTEAMS)
    {
        error(ERR_MAX_TEAMS);
    }
    else
    {
        char nombreEquipo[kTEAMNAME];
        cout << "Enter team name: ";
        cin >> nombreEquipo;

        Team equipo;
        equipo.id = ID;
        ID++;

        cout << "Enter team name:";
        cin.getline(equipos[contadorActualEquipos].name, kTEAMNAME);
        if (strlen(equipos[contadorActualEquipos].name) == 0)
        {
            sprintf(equipos[contadorActualEquipos].name, "Team_%d", equipo.id);
        }

        /* Antes
                if ( (nombreEquipo.length() > kTEAMNAME) || (nombreEquipo.length() == 0))
                {
                    // Se pone el nombre por defecto

                    nombreEquipo = "Team_"+equipo.id;

                    for (int j = 0; j < nombreEquipo.length(); j++)
                    {
                        equipo.name[j] = nombreEquipo[j];
                    }

                }
                else
                {

                    for (int j = 0; j < nombreEquipo.length(); j++)
                    {
                        equipo.name[j] = nombreEquipo[j];
                    }

                }

        */

        equipo.wins = 0;
        equipo.losses = 0;
        equipo.draws = 0;
        equipo.points = 0;

        for (int i = 0; i < kPLAYERS; i++) // bucle añadir jugadores
        {
            sprintf(equipo.players[i].name, "%s-R%d", equipo.name, i + 1);
            equipo.players[i].goals = 0;
            equipo.players[i].best = false;
        }
        equipos.push_back(equipo);
        contadorActualEquipos++;
    }
}
void showTeams(int &contadorActualEquipos, vector<Team> &equipos)
{

    if (contadorActualEquipos == 0)
    {
        error(ERR_NO_TEAMS);
    }

    char teamName[kTEAMNAME];
    cout << "Enter team name: ";
    cin.getline(teamName, kTEAMNAME);
    if (strlen(teamName) == 0)
    { // Mostrar información de todos los equipos

        for (long unsigned int i = 0; i < equipos.size(); i++)
        {
            cout << "Name: " << equipos[i].name << endl;
            cout << "Wins: " << equipos[i].wins << endl;
            cout << "Losses: " << equipos[i].losses << endl;
            cout << "Draws: " << equipos[i].draws << endl;
            cout << "Points: " << equipos[i].points << endl;
            for (int j = 0; j < kPLAYERS; j++)
            {
                cout << equipos[i].players[j].name << ": " << equipos[i].players[j].goals << " goals" << endl;
            }
        }
    }
    else
    { // Buscar el equipo por nombre
        bool teamFound = false;
 
        for (long unsigned int i = 0; i < equipos.size(); i++)
        {
            if (strcmp(equipos[i].name, teamName) == 0)
            {
                teamFound=true;
                cout << "Name: " << equipos[i].name << endl;
                cout << "Wins: " << equipos[i].wins << endl;
                cout << "Losses: " << equipos[i].losses << endl;
                cout << "Draws: " << equipos[i].draws << endl;
                cout << "Points: " << equipos[i].points << endl;
                for (int j = 0; j < kPLAYERS; j++)
                {
                    cout << equipos[i].players[j].name << ": " << equipos[i].players[j].goals << " goals" << endl;
                }
            }
        }
        if (!teamFound)
        {
            error(ERR_NOT_EXIST);
        }
    }
}

void deleteTeam(int &contadorActualEquipos, vector<Team> &equipos)
{

    if (contadorActualEquipos == 0)
    {
        error(ERR_NO_TEAMS);
    }
    else
    {
        char teamName[kTEAMNAME];
        cout << "Enter team name: ";
        cin.getline(teamName, kTEAMNAME);

        if (strlen(teamName) == 0)
        {
            error(ERR_EMPTY);
        }
        else
        {
            bool teamFound = false;
            for (long unsigned int i = 0; i < equipos.size(); i++)
            {
                if (strcmp(equipos[i].name, teamName))
                {
                    equipos.erase(equipos.begin() + i);
                    teamFound = true;
                    contadorActualEquipos--;
                }
            }

            /*
                        for (Team team : equipos)
                        {
                            if (team.name == teamName)
                            {
                                teamFound = true;
                                contadorActualEquipos--;
                                for (int i = 0; i < kPLAYERS; i++)
                                {
                                    team.players[i] = NULL; ////HAY QUE MIRAR ESTO
                                }
                            }
                        }

                        */
            if (!teamFound)
            {
                error(ERR_NOT_EXIST);
            }
        }
    }
}
void addAllTeams(int &contadorActualEquipos, int &ID, vector<Team> &equipos)
{
    if (equipos.size() > 0)
    {
        char opcion;
        bool opcionCorrecta = false;
        while (!opcionCorrecta)
        {
            cout << "Do you want to delete existing teams (y/n)?";
            cin >> opcion;

            if (opcion == 'y' || opcion == 'Y')
            {
                opcionCorrecta = true;
                equipos.clear();
                int numeroEquipos;
                bool numeroCorrecto = false;
                while (!numeroCorrecto)
                {
                    cout << "Enter number of teams: ";
                    cin >> numeroEquipos;
                    if (numeroEquipos < 2 || numeroEquipos > 20)
                    {
                        error(ERR_NUM_TEAMS);
                    }
                    else
                    {
                        numeroCorrecto = true;
                    }
                }
                for (int i = 0; i < numeroEquipos; i++)
                {
                    Team equipo;
                    equipo.id = ID;
                    ID++;

                    equipo.wins = 0;
                    equipo.losses = 0;
                    equipo.draws = 0;
                    equipo.points = 0;
                    
                    sprintf(equipo.name, "Team_%d", equipo.id);

                    for (int i = 0; i < kPLAYERS; i++) // bucle añadir jugadores
                    {
                        sprintf(equipo.players[i].name, "%s-R%d", equipo.name, i + 1);
                        equipo.players[i].goals = 0;
                        equipo.players[i].best = false;
                    }
                    equipos.push_back(equipo);
                    contadorActualEquipos++;
                }
            }
            if (opcion == 'N' || opcion == 'n')
            {
                opcionCorrecta = true;
                // Volver al menu principal
            }
        }
        /*
        cout<< "Do you want to delete existing teams (y/n)?";
        cin>>opcion;

        if(opcion == "y" || opcion == "Y"){
            equipos.clear();
            int numeroEquipos;
            cout<<"Enter number of teams: ";
            cin>>numeroEquipos;
            if (numeroEquipos < 2 || numeroEquipos>20)
            {
                error(ERR_NUM_TEAMS);
            }


        }
        if(opcion == "N" || opcion == "n"){
            //Volver al menu principal
        }
    */
    }
    else
    {
        int numeroEquipos;
        bool numeroCorrecto = false;
        while (!numeroCorrecto)
        {
            cout << "Enter number of teams: ";
            cin >> numeroEquipos;
            if (numeroEquipos < 2 || numeroEquipos > 20)
            {
                error(ERR_NUM_TEAMS);
            }
            else
            {
                numeroCorrecto = true;
            }
        }
        for (int i = 0; i < numeroEquipos; i++)
        {
            Team equipo;
            equipo.id = ID;

            ID++;

            equipo.wins = 0;
            equipo.losses = 0;
            equipo.draws = 0;
            equipo.points = 0;

            
           
            sprintf(equipo.name, "Team_%d", equipo.id);

            for (int i = 0; i < kPLAYERS; i++) // bucle añadir jugadores
            {
                sprintf(equipo.players[i].name, "%s-R%d", equipo.name, i + 1);
                equipo.players[i].goals = 0;
                equipo.players[i].best = false;
            }
            equipos.push_back(equipo);
            contadorActualEquipos++;
        }
    }
}

void showStandings(vector<Team> &equipos, bool league)
{
    if (!league)
    {
        error(ERR_NO_LEAGUE);
    }
    else
    {
        vector<Team> equiposOrdenados = equipos;

        for (long unsigned int i = 1; i < equipos.size(); i++)
        {
            for (long unsigned int j = equipos.size()-1; j >=i; j--)
            {
                if (equiposOrdenados[j].points > equiposOrdenados[j-1].points)
                {
                    Team aux = equiposOrdenados[j];
                    equiposOrdenados[j] = equiposOrdenados[j-1];
                    equiposOrdenados[j-1] = aux;
                }
                
            }
            
        }

        for (long unsigned int i = 0; i < equiposOrdenados.size(); i++)
        {
            cout << equiposOrdenados[i].name << "|" << equiposOrdenados[i].wins << "|" << equiposOrdenados[i].draws << "|" << equiposOrdenados[i].losses << "|" << equiposOrdenados[i].points << endl;
        }
    }
}
void showBestPlayers(vector<Team> &equipos, bool league)
{

    if (!league)
    {
        error(ERR_NO_LEAGUE);
    }
    else
    {
        for (long unsigned int i = 0; i < equipos.size(); i++)
        {

            for (int j = 0; j < 5; j++)
            {
                if (equipos[i].players[j].best)
                {
                    cout << equipos[i].name << "|" << equipos[i].players[j].name << "|" << equipos[i].players[j].goals;
                }
            }
            
        }
    }
}
// Función principal. Tendrás que añadir más código tuyo
int main()
{

    int ID = 0;
    vector<Team> equipos;
    int contadorActualEquipos = 0;
    char option;
    bool league = true; /*Falta implementar este boleano*/
    srand(888);         // Fija la semilla del generador de números aleatorios. ¡NO TOCAR!

    do
    {
        showMenu();
        cin >> option;
        cin.ignore(); // Para evitar que el salto de línea se quede en el buffer de teclado y luego pueda dar problemas si usas "getline"

        switch (option)
        {
        case '1': // Llamar a la función "addTeam" para añadir un nuevo equipo
            addTeam(contadorActualEquipos, ID, equipos);
            break;
        case '2': // Llamar a la función "addAllTeams" para añadir todos los equipos de una vez
            addAllTeams(contadorActualEquipos, ID, equipos);
            break;
        case '3': // Llamar a la función "deleteTeam" para borrar un equipo
            deleteTeam(contadorActualEquipos, equipos);
            break;
        case '4': // Llamar a la función "showTeams" para mostrar los datos de los equipos
            showTeams(contadorActualEquipos, equipos);
            break;
        case '5': // Llamar a la función "playLeague" para simular los resultados de la competición
            break;
        case '6': // Llamar a la función "showStandings" para mostrar la clasificación final
            showStandings(equipos, league);
            break;
        case '7': // Llamar a la función "showBestPlayers" para mostrar los mejores jugadores de cada equipo
            showBestPlayers(equipos, league);
            break;
        case 'q':
            break;
        default:
            error(ERR_OPTION);
        }
    } while (option != 'q');
}
