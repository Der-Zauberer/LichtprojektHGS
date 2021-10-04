#include <C_8051F340.h>
#include <Einstellungen.h>

//--- Variablen ---

//Status des Lichtes f�r jeden Raum
//0=Aus; 1=An
char lightStatus[] = {0, 0, 0, 0, 0, 0, 0, 0};

//Status der Roll�den f�r jeden Raum
//0=Zu; 180=Auf (Angabe in Grad)
char shutterStatus[] = {0, 0, 0, 0, 0, 0, 0, 0};

//Status der automatischen Steuerung f�r jeden Raum
//0=Aus; 1=An
char autoStatus[] = {0, 0, 0, 0, 0, 0, 0, 0};

//Zeigt auf welchen Raum gerade Zugegriffen wird
//Es gibt Raum 0 bis 8
char room = 0;

//--- Funktionen ---

//Initialisiert alle Timer und Interrupts und startet Timer0
void initialize() {

}

//�perbr�ft ob ein weiterer Raum zur verf�gung steht, wechselt auf den
//Raum und l�d die Raumeigenschaften f�r die physische Ausgabe
//incement: -1=Decrement 1=Increment (Angabe von Differenz)
void nextRoom(char increment) {
    if(increment < 0 && room > 0) {
        room ++;
    } else if(increment > 0 && room < 8 ) {
        room--;
    }
    P1 = room;
}

//Setzt das Licht auf den gew�nschten Status
//status: 0=Zu; 1=Auf; 2=Invertieren
//room: Der Raum f�r den das Licht gesetzt werden soll
void setLights(char status, char room) {

}

void main()
{
    Grundeinstellungen();

    while (1) {
    
        if(!P0_3) {
            nextRoom(-1);
            while (!P0_3) continue;
        } else if(!P0_0) {
            nextRoom(1);
            while (!P0_0) continue;
        }
    }
}