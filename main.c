#include <C_8051F340.h>
#include <Einstellungen.h>

//-------------//
//--- Ports ---//
//-------------//

sbit PREV_ROOM = 0x80; //Vorheriger Raum P0.0
sbit TOGGLE_LIGHT = 0x81; //Licht toggeln P0.1
sbit CRISTMASLIGHT = 0x82; // Weihnachtsbeleuchtung toggeln P0.2
sbit NEXT_ROOM = 0x83; //Naechster Raum P0.3

sbit SHUTTER_UP = 0xA0; //Rolladen hoch P2.0
sbit SHUTTER_DOWN = 0xA1; //Rolladen runter P2.1

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
int shutterStatus = 0;

//Zaehlt von 0 bis 7 fuer den Index von engine[]
char shutterIndex = 0;

//Enthaelt die Zustände für den Schrittmotor
unsigned char engine[] = {0b00000001, 0b00000011, 0b00000010, 0b00000110 ,0b00000100, 0b00001100, 0b00001000, 0b00001001};

char SHUTTER_STATUS_MIN = 0;
int SHUTTER_STATUS_MAX = 2048;

//Enthält die Zustaende für die Sieben-Segment-Anzeige
unsigned char segmentDigit[] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};

//Enthaelt die Zustaende für die Weihnachtsbeleuchtung
unsigned char christmasLightState[] = {0b10101010, 0b00000000, 0b01010101, 0b00000000};

//Zaehlt die Interrupt-Service-Routinen
unsigned int interruptCounter = 0;

//Zeigt auf welchen Raum gerade Zugegriffen wird
//Es gibt Raum 0 bis 8
char room = 0;

//Gibt an ob die Weihnachtsbeleuchtung eingeschaltet ist
//0=Aus, 1=An
char christmasLight = 0;

//Zaehlt den Animationsfortschritt fuer die Weihnachtsbeleuchtung an
unsigned int christmasLightCounter = 0;

//------------------//
//--- Funktionen ---//
//------------------//

int i = 0;

//Timer-Interrupt für die Weihnachtsbeleuchtung
void timerInterrupt() interrupt 3 {
    if (interruptCounter < 1) {
        interruptCounter++;
    } else {
        interruptCounter = 0;
        if (christmasLightCounter < 4) {
            christmasLightCounter++;
        } else {
            christmasLightCounter = 0;
        }
    }
    TF0 = 0; //Ueberlaufbit für erneute Ueberprüfung auf 0 setzen
    TH0 = 0x3C; //65536µs – 15536µs = 50000µs = 3CB0 in hex (Ein Durchgang ist 50ms lang)
    TL0 = 0xB0; //65536µs – 15536µs = 50000µs = 3CB0 in hex (Ein Durchgang ist 50ms lang)
}

//Aktualisiert die LED Anzeige fuer die Lichter
void updateLights() {
    if (!christmasLight) {
        LED0 = lightStatus[7];
        LED1 = lightStatus[6];
        LED2 = lightStatus[5];
        LED3 = lightStatus[4];
        LED4 = lightStatus[3];
        LED5 = lightStatus[2];
        LED6 = lightStatus[1];
        LED7 = lightStatus[0];
    } else {
        LED = christmasLightState[christmasLightCounter];
    }
}

//Aktualisiert die 7-Segment Anzeige
void updateDisplay() {
    SEGMENTS = segmentDigit[room + 1];
}

//Initialisiert alle Timer und Interrupts und startet Timer0
void initialize() {
    Grundeinstellungen();
    SEG0 = 1; //Setzt die Anzeige aus Segment0
    SEG1 = 0; //Setzt die Anzeige aus Segment0
    TMOD = 0b00010001; //Timer0 16-Bit und Timer1 16-Bit
    IE = 0b10001000; //Interrupt Timer1
    ENGINE = 0; //Schaltet Motor aus
}

//Wartet 500 Millisekunden
void wait500Mycros() {
    TH0 = 0xFC; //65536µs – 65026µs = 500µs = FC18 in hex (Ein Durchgang ist 500µs lang)
    TL0 = 0x18; //65536µs – 65026µs = 500µs = FC18 in hex (Ein Durchgang ist 500µs lang)
    TR0 = 1; //Timer starten
    while (!TF0) continue;
    TF0 = 0; //Ueberlaufbit für erneute Ueberprüfung auf 0 setzen
    TR0 = 0; //Timer stoppen
}

//Wartet 500 Millisekunden
void wait500Millis() {
    TH0 = 0x3C; //65536µs – 15536µs = 50000µs = 3CB0 in hex (Ein Durchgang ist 50ms lang)
    TL0 = 0xB0; //65536µs – 15536µs = 50000µs = 3CB0 in hex (Ein Durchgang ist 50ms lang)
    TR0 = 1; //Timer starten
    for (i = 0; i < 10; i++) { //40 Durchgänge, da 40 * 50ms = 2000ms = 2s
        while (!TF0) continue; //Warten, bis ein Durchgang fertig ist
        TF0 = 0; //Ueberlaufbit für erneute Ueberprüfung auf 0 setzen
        TH0 = 0x3C; //65536µs – 15536µs = 50000µs = 3CB0 in hex (Ein Durchgang ist 50ms lang)
        TL0 = 0xB0; //65536µs – 15536µs = 50000µs = 3CB0 in hex (Ein Durchgang ist 50ms lang)
    }
    TR0 = 0; //Timer stoppen
}

//Ueperbrueft ob ein weiterer Raum zur Verfuegung steht, wechselt auf den
//Raum und laed die Raumeigenschaften fuer die physische Ausgabe
void incrementRoom() {
    if (room < 7) {
        room++;
    }
    updateLights();
}

//Ueperbrueft ob ein weiterer Raum zur Verfuegung steht, wechselt auf den
//Raum und laed die Raumeigenschaften fuer die physische Ausgabe
void decrementRoom() {
    if (room > 0) {
        room--;
    }
    updateLights();
}

//Setzt das Licht auf den gewuenschten Status
//status: 0=Zu; 1=Auf; 2=Invertieren
//room: Der Raum fuer den das Licht gesetzt werden soll
void setLight(char status, char room) {
    if (status == 0 || status == 1) {
        lightStatus[room] = status;
    } else if (status == 2) {
        lightStatus[room] = !lightStatus[room];
    }
    updateLights();
}

//Bewegt den Schrittmotor um einen Schritt
//up: 0=Herunter; 1=Hoch
//return: Gibt an, ob sich der Motor weiter bewegen lässt 0=Nein; 1=Ja
int stepShutter(char up) {
    if (up == 1) {
        if (shutterStatus < (SHUTTER_STATUS_MAX * 4)) {
            if (shutterIndex < 8) {
                shutterIndex++;
            } else {
                shutterIndex = 0;
            }
            ENGINE = engine[shutterIndex];
            shutterStatus++;
        } else {
            return 0;
        }
    } else {
         if (shutterStatus > SHUTTER_STATUS_MIN) {
            if (shutterIndex > 0) {
                shutterIndex--;
            } else {
                shutterIndex = 8;
            }
            ENGINE = engine[shutterIndex];
            shutterStatus--;
        } else {
            return 0;
        }
    }
    return 1;
}

//Bewegt den Schrittmotor auf die gewuenschte Position
//up: 0=Herunter; 1=Hoch
void moveShutter(char up) {
    wait500Millis();
    if (SHUTTER_UP && SHUTTER_DOWN) {
        while(SHUTTER_DOWN && SHUTTER_UP && stepShutter(up) == 1) {
            wait500Mycros();
        }
        ENGINE = 0;
        return;
    }
    if (up == 1) {
        if (!SHUTTER_UP) {
            while (!SHUTTER_UP && stepShutter(up) == 1) {
                wait500Mycros();
            }
            ENGINE = 0;
            return;
        }
    } else {
        if (!SHUTTER_DOWN) {
            while (!SHUTTER_DOWN && stepShutter(up) == 1) {
                wait500Mycros();
            }
            ENGINE = 0;
            return;
        }
    }
}

//Main-Funktion fuer die Ueberpruefung der Taster
void main() {
    initialize();
    while (1) {
        updateLights();
        updateDisplay();
        //Naechster Raum
        if (!NEXT_ROOM) {
            decrementRoom();
            while (!NEXT_ROOM) continue;
        //Vorheriger Raum
        } else if (!PREV_ROOM) {
            incrementRoom();
            while (!PREV_ROOM) continue;
	//Licht Anschalten oder Ausschalten
	} else if (!TOGGLE_LIGHT) {
	    setLight(2, room);
	    while (!TOGGLE_LIGHT) continue;
        //Weihnachstbeleuchtung Anschanlten oder Ausschalten
        } else if (!CRISTMASLIGHT) {
            if (christmasLight) {
                TR1 = 0;
            } else {
                TR1 = 1;
            }
            christmasLight = !christmasLight;
            while (!CRISTMASLIGHT) continue;
        //Rolladen Hoch
        } else if (!SHUTTER_UP) {
            moveShutter(1);
            while (!SHUTTER_UP) continue;
        //Rolladen Herunter
        } else if (!SHUTTER_DOWN) {
            moveShutter(0);
            while (!SHUTTER_DOWN) continue;
        }
    }
}
