#include <C8051F340.h>
#include <Einstellungen.h>


void init (void) 
{
Grundeinstellungen();
TMOD = 0b00000001; //Timer0 16-Bit (In Formelsammlung nachschlagen)
TH0 = 0xFC; //65536�s � 15536�s = 50000�s = 3CB0 in hex (Ein Durchgang ist 50ms lang)
TL0 = 0x18; //65536�s � 15536�s = 50000�s = 3CB0 in hex (Ein Durchgang ist 50ms lang)


}

int counter=0;



void main() {
    int motorschritte[] = {0b00000001, 0b00000011, 0b00000010, 0b00000110 ,0b00000100, 0b00001100, 0b00001000, 0b00001001};
    init();
    TR0=1;
    while(1){
        while(!TF0) continue; //Warten, bis ein Durchgang fertig ist
        TF0 = 0; //�berlaufbit f�r erneute �berpr�fung auf 0 setzen
        TH0 = 0xFC; //F�r n�chsten Durchgang nachladen
        TL0 = 0x18; //F�r n�chsten Durchgang nachladen
        P0= motorschritte[counter];
        if(counter < 8){
            counter++;
        }else{
            counter=0;
        }   
    }
}


    



