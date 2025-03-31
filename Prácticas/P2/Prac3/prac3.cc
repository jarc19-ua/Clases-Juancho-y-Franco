#include <iostream>
#include "AircraftCarrier.h"
#include "Board.h"

using namespace std;

int main(){
    srand(888);

    Board board(5);  // Tablero de tamaño 5x5

    AircraftCarrier one("USS Acme");
    one.addFighters("3/F-35B:2/F-18:3/A6");

    AircraftCarrier two("Spectra One");
    two.addFighters("4/SF-1:3/SB-3:2/SI-6B");

     // Posicionamos algunos cazas sobre el tabler
    Coordinate c1(2,3);
    Fighter *f1=one.getFirstAvailableFighter("F-18");
    board.launch(c1,f1);

    Coordinate c2(3,2);
    Fighter *f2=two.getFirstAvailableFighter("");
    board.launch(c2,f2);

    cout << "After launching one fighter of each aircraft:" << endl;
    cout << one << endl;
    one.showFleet();
    cout << endl;
    cout << two << endl;
    two.showFleet();
    cout << endl;

    Fighter *f3=one.getFirstAvailableFighter("");
    int result=board.launch(c2,f3);
    if(result!=0){
        one.updateResults(result);
        two.updateResults(-result);
    }
    
    cout << "After a fight between two fighters:" << endl;
    cout << one << endl;
    one.showFleet();
    cout << endl;
    cout << two << endl;
    two.showFleet();
    cout << endl;

    return 0;
}