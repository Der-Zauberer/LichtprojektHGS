#include <C_8051F340.h>
#include <Einstellungen.h>



//--- Ports ---

sbit button0 = 0x80; //Vorheriger Raum
sbit button1 = 0x81; //Licht toggeln
sbit button2 = 0x82; //Automatikmodius toggeln
sbit button3 = 0x83; //Nächster Raum

sbit led0 = 0x90;
sbit led1 = 0x91;
sbit led2 = 0x92;
sbit led3 = 0x93;
sbit led4 = 0x94;
sbit led5 = 0x95;
sbit led6 = 0x96;
sbit led7 = 0x97;

sfr button = 0x80; //Port0
sfr led = 0x90; //Port1
sfr segments = 0xB0; //Port3



//--- Variablen ---

//Status des Lichtes für jeden Raum
//0=Aus; 1=An
char lightStatus[] = {0, 0, 0, 0, 0, 0, 0, 0};

//Status der Rolläden für jeden Raum
//0=Zu; 180=Auf (Angabe in Grad)
char shutterStatus[] = {0, 0, 0, 0, 0, 0, 0, 0};

//Status der automatischen Steuerung für jeden Raum
//0=Aus; 1=An
char autoStatus[] = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned char segmentDigit[] = {0b00111111, 0b00000111, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};

//Zeigt auf welchen Raum gerade Zugegriffen wird
//Es gibt Raum 0 bis 8
char room = 0;



//--- Funktionen ---

//Aktualisiert die LED Anzeige für die Lichter
void updateLights() {
    led0 = lightStatus[0];
    led1 = lightStatus[1];
    led2 = lightStatus[2];
    led3 = lightStatus[3];
    led4 = lightStatus[4];
    led5 = lightStatus[5];
    led6 = lightStatus[6];
    led7 = lightStatus[7];
}

//Aktualisiert die 7-Segment Anzeige
void updateDisplay() {
    segments = segmentDigit[room + 1];
}

//Initialisiert alle Timer und Interrupts und startet Timer0
void initialize() {
    Grundeinstellungen();
}

//Üperbrüft ob ein weiterer Raum zur Verfügung steht, wechselt auf den
//Raum und läd die Raumeigenschaften für die physische Ausgabe
//incement: -1=Decrement 1=Increment (Angabe von Differenz)
void nextRoom(char increment) {
    if(increment > 0 && room < 8) {
        room ++;
    } else if(increment < 0 && room > 0) {
        room--;
	}
    updateLights();
}

//Setzt das Licht auf den gewünschten Status
//status: 0=Zu; 1=Auf; 2=Invertieren
//room: Der Raum für den das Licht gesetzt werden soll
void setLight(char status, char room) {
    if(status == 0 || status == 1) {
        lightStatus[room] = status;
    } else if(status == 2) {
        lightStatus[room] = !lightStatus[room];
    }
    updateLights();
}

//Bewegt den Servomotor auf die gewünschte Position
//status: 0=Zu; 1=Auf; 2=Invertieren
//full: 0=StückweiseBewegen 1=KomplettBewegen
//room: Der Raum für den der Rolladen bewegt werden soll
void moveShutter(char status, char full, char room) {

}

//Main-Funktion für die Überprüfung der Taster
void main() {
    initialize();
    while(1) {
        if(!button3) {
            nextRoom(-1);
            while(!button3) continue;
        } else if(!button0) {
            nextRoom(1);
            while(!button0) continue;
        }
    }
}