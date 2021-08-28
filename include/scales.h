#include <Arduino.h>
/*
Manejo de la escalas y modos de configuración
Mensajes MIDI
*/

///////////////////////////
//ESCALAS
byte scaleIndex = 0;
int octaveIndex = 2;

const char *octavas[] = {"-24", "-12", "0", "+12", "+24"};
int octave[5] = {-24, -12, 0, 12, 24};

byte OctSel = 2;

const byte SCALE_NUM = 4; //Número de Escalas
const byte NOTE_NUM = 16; //Numero de notas por escala

int scaleNotes[SCALE_NUM][NOTE_NUM]{
    {

    }};

const char *scaleNames[] = {"Jonian", "MinHar", "MinPent", "WhlTon"};

byte escalaSelect = 0;

String noteNames[] = {
    "C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B"};

byte noteSelect = 0;

String mode[] = {"Scl", "Pb"};

byte ModeSel = 0;

////////////////////////////////////////////////////////

////////////////////////////////////
//MIDI
byte MIDI_CH = 1;
byte NOTE = 48;