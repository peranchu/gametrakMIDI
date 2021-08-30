#include <Arduino.h>
/*
Manejo de la escalas y modos de configuración
Mensajes MIDI y MODO
*/

///////////////////////////
//ESCALAS
byte escalaSelect = 0; //Almacena escala seleccionada
byte noteSelect = 0;   //Almacena Tonalidad seleccionada
byte ModeSel = 0;      //Almacena modo seleccionado

//Octavas
const char *octavas[] = {"-24", "-12", "0", "+12", "+24"}; //Representacion pantalla
int octave[5] = {-24, -12, 0, 12, 24};                     //Octava

byte OctSel = 2; //Almacena la octava seleccionada

//Escalas
const byte SCALE_NUM = 4; //Número de Escalas
const byte NOTE_NUM = 16; //Numero de notas por escala

int scaleNotes[SCALE_NUM][NOTE_NUM]{

    {-1, 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24},   // 0: ionian
    {-1, 0, 2, 3, 5, 7, 8, 11, 12, 14, 15, 17, 19, 20, 23, 24},   // 4: min harm
    {-2, 0, 3, 5, 7, 10, 12, 15, 17, 19, 22, 24, 27, 29, 31, 34}, // 5: minor penta
    {-2, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28},  // 7: whltone
                                                                  //  { -2, 0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24}, // 4: aeolian
                                                                  //  { -4, 0, 2, 4, 7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33}, // 5: major pentatonic
                                                                  //  { -2, 0, 3, 5, 7, 10, 12, 15, 17, 19, 22, 24, 27, 29, 31, 34}, // 6: minor pentatonic
                                                                  //  { -2, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28}, // 7: wholetone

};

//Tipos de Escala
const char *scaleNames[] = {"Jonian", "MinHar", "MinPent", "WhlTon"}; //pantalla

//Tonalidad
const char *noteNames[] = {
    "C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B"}; //pantalla

//Modo
const char *mode[] = {"Scl", "Pb"}; //pantalla

bool modoScl = true;
bool modoPb = false;

////////////////////////////////////////////////////////

////////////////////////////////////
//MIDI
byte MIDI_CH = 1;
byte NOTE = 48;   //C3
byte noteOut = 0; //Almacena el mensaje de numero de nota a enviar

////////////////////////////////////////
