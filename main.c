#include <C_8051F340.h>
#include <Einstellungen.h>

//-------------//
//--- Ports ---//
//-------------//

sbit PREROOM = 0x80; //Vorheriger Raum P0.0
sbit TOGGLELIGHT = 0x81; //Licht toggeln P0.1
sbit AUTOMODE = 0x82; //Automatikmodius toggeln P0.2
sbit NEXTROOM = 0x83; //Naechster Raum P0.3

sbit SHUTTERUP = 0xA0; //Rolladen hoch P2.0
sbit SHUTTERDOWN = 0xA1; //Rolladen runter P2.1

sbit LED0 = 0x90;
sbit LED1 = 0x91;
sbit LED2 = 0x92;
sbit LED3 = 0x93;
sbit LED4 = 0x94;
sbit LED5 = 0x95;
sbit LED6 = 0x96;
sbit LED7 = 0x97;

sfr BUTTON = 0x80; //Port0
sfr LED = 0x90; //Port1
sfr SEGMENTS = 0xB0; //Port3
sfr ENGINE = 0xC7; //Port4

sbit SEG0 = 0xA4; //port 2.4
sbit SEG1 = 0xA5; //Port 2.5

//-----------------//
//--- Variablen ---//
//-----------------//

//Status des Lichtes fuer jeden Raum
//0=Aus; 1=An
char lightStatus[] = {0, 0, 0, 0, 0, 0, 0, 0};

//Status des Rolladens
char shutterStatus = 0;

//Zählt von 0 bis 8 fuer den Index von ENGINE
char shutterIndex = 0;

//Enthält daten für den Schrittmotor
int engine[] = {0b00000001, 0b00000011, 0b00000010, 0b00000110 ,0b00000100, 0b00001100, 0b00001000, 0b00001001};

int SHUTTERSTATUSMIN = 0;
int SHUTTERSTATUSMAX = 1000;

//Status der automatischen Steuerung fuer jeden Raum
//0=Aus; 1=An
char autoStatus[] = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned char segmentDigit[] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};

//Zeigt auf welchen Raum gerade Zugegriffen wird
//Es gibt Raum 0 bis 8
char room = 0;

//------------------//
//--- Funktionen ---//
//------------------//

int i = 0;

//Aktualisiert die LED Anzeige fuer die Lichter
void updateLights() {
    LED0 = lightStatus[7];
    LED1 = lightStatus[6];
    LED2 = lightStatus[5];
    LED3 = lightStatus[4];
    LED4 = lightStatus[3];
    LED5 = lightStatus[2];
    LED6 = lightStatus[1];
    LED7 = lightStatus[0];
}

//Aktualisiert die 7-Segment Anzeige
void updateDisplay() {
    SEGMENTS = segmentDigit[room + 1];
}

//Initialisiert alle Timer und Interrupts und startet Timer0
void initialize() {
    Grundeinstellungen();
    SEG0 = 1;
    SEG1 = 0;
    TMOD = 0b00000001;
}

void wait50Millis() {
    TH0 = 0xFC;
    TL0 = 0x18;
    TR0 = 1;
    while (!TF0) continue;
    TR0 = 0;
    TF0 = 0;
}

void wait2Secs() {
    TH0 = 0x3C;
    TL0 = 0xB0;
    TR0 = 1;
    for(i = 0; i < 20000; i++) {
        TH0 = 0x3C;
        TL0 = 0xB0;
        while (!TF0) continue;
    }
    TR0 = 0;
    TF0 = 0;
}

//Ueperbrueft ob ein weiterer Raum zur Verfuegung steht, wechselt auf den
//Raum und laed die Raumeigenschaften fuer die physische Ausgabe
void incrementRoom() {
    if(room < 7) {
        room++;
    }
    updateLights();
    
}

//Ueperbrueft ob ein weiterer Raum zur Verfuegung steht, wechselt auf den
//Raum und laed die Raumeigenschaften fuer die physische Ausgabe
void decrementRoom() {
    if(room > 0) {
        room--;
    }
    updateLights();
}

//Setzt das Licht auf den gewuenschten Status
//status: 0=Zu; 1=Auf; 2=Invertieren
//room: Der Raum fuer den das Licht gesetzt werden soll
void setLight(char status, char room) {
    if(status == 0 || status == 1) {
        lightStatus[room] = status;
    } else if(status == 2) {
        lightStatus[room] = !lightStatus[room];
    }
    updateLights();
}

//Bewegt den Servomotor auf die gewuenschte Position
//room: Der Raum fuer den der Rolladen bewegt werden soll
void moveShutterUp() {
    wait2Secs();
    if(!SHUTTERUP) {
        while (!SHUTTERUP) {
            if (shutterIndex < 7) {
                shutterIndex++;
            } else {
                shutterIndex = 0;
            }
            ENGINE = engine[shutterIndex];
            wait50Millis();
        }
    } else {
        while(shutterStatus != SHUTTERSTATUSMAX) {
            if(!SHUTTERUP) {
                ENGINE = 0;
                while(!SHUTTERUP) continue;
                break;
            }
            if (shutterIndex < 7) {
                shutterIndex++;
            } else {
                shutterIndex = 0;
            }
            ENGINE = engine[shutterIndex];
            wait50Millis();
        }
    }
    ENGINE = 0;
}

//Bewegt den Servomotor auf die gewuenschte Position
//room: Der Raum fuer den der Rolladen bewegt werden soll
void moveShutterDown() {
    wait2Secs();
    if(!SHUTTERDOWN) {
        while (!SHUTTERDOWN) {
            if (shutterIndex > 0) {
                shutterIndex--;
            } else {
                shutterIndex = 7;
            }
            ENGINE = engine[shutterIndex];
            wait50Millis();
        }
    } else {
        while(shutterStatus != SHUTTERSTATUSMIN) {
            if(!SHUTTERDOWN) {
                ENGINE = 0;
                while(!SHUTTERUP) continue;
                break;
            }
            if (shutterIndex > 0) {
                shutterIndex--;
            } else {
                shutterIndex = 7;
            }
            ENGINE = engine[shutterIndex];
            wait50Millis();
        }
    }
    ENGINE = 0;
}

//Main-Funktion fuer die Ueberpruefung der Taster
void main() {
    initialize();
    while(1) {
    updateLights();
    updateDisplay();
        if(!NEXTROOM) {
            decrementRoom();
            while(!NEXTROOM) continue;
        } else if(!PREROOM) {
            incrementRoom();
            while(!PREROOM) continue;
        } else if(!AUTOMODE) {
            setLight(2, room);
            while(!AUTOMODE) continue;
        } else if(!SHUTTERUP) {
            moveShutterUp();
        } else if(!SHUTTERDOWN) {
            moveShutterDown();
        }
    }
}

