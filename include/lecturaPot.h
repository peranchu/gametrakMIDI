#include <Arduino.h>
#include <Average.h>

#include "Escalado.h"

//////  AVERAGE FILTRADO SEÑALES POTENCIOMETROS //////////////////
byte averageSize[6] = {10, 10, 10, 10, 10, 10};

Average<int> ave[6] = {
    Average<int>(averageSize[0]),
    Average<int>(averageSize[1]),
    Average<int>(averageSize[2]),
    Average<int>(averageSize[3]),
    Average<int>(averageSize[4]),
    Average<int>(averageSize[5])};

//////////////////////////////////////////

/////////////////////////////////////
//POTENCIOMETROS
const byte N_POTS = 6;

/*Asignacion pines Potenciometros
   A0,A1,A2 = mando derecha ejes z,x,y respectivamente
   A3,A6,A7 = mando izquierda ejes z,x,y respectivamente
*/
const byte POT_PIN[N_POTS] = {A0, A1, A2, A3, A6, A7};

//Cambios de control CC MIDI
int POT_CC_N[N_POTS] = {20, 21, 22, 23, 24, 25};

//Almacenan estado Actual, previo y variación de los potenciometros
int potCState[N_POTS] = {0}; //Estado Actual POT
int potPState[N_POTS] = {0}; //Estado previo POT
int potVar[N_POTS] = {0};    //Diferencia entre los dos estados

int reading[N_POTS] = {0};
int filteredVal[N_POTS] = {0};
int scaledVal[N_POTS] = {0};

int potMidiCState[N_POTS] = {0}; //Estado Actual Valor MIDI
int potMidiPState[N_POTS] = {0}; //Estado Previo valor MIDI

const int TIMEOUT = 300;                                          //Cantidad de tiempo que se leerá despues de superar el Threshold
const int varThreshold = 20;                                      //Umbral de variación
boolean potMoving[N_POTS] = {true, true, true, true, true, true}; //si el potenciometro se ha movido
unsigned long PTime[N_POTS] = {0};                                //Tiempo Previo almacenado
unsigned long timer[N_POTS] = {0};                                //Tiempo almacenado desde el reset

// DEBOUNCE
unsigned long lastDebounceTime[N_POTS] = {0};
unsigned long debounceDelay = 20;
//////////////////////////////////////////////////////////////////

bool note_is_playing = false;

////////////////////////
//POTENCIOMETROS
void readPot(int i)
{
    reading[i] = analogRead(POT_PIN[i]); //raw reading
    ave[i].push(reading[i]);             //Añadimos los valores al filtro

    filteredVal[i] = ave[i].mean();
    potCState[i] = filteredVal[i];

    //Escalado y curva
    //fscale( float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve) - linearize curve
    scaledVal[i] = fscale(IR_entrada_min[i], IR_entrada_max[i], IR_min_val[i], IR_max_val[i], filteredVal[i], IR_curve);

    //clips IR value to mix-max
    IR_val[i] = clipValue(scaledVal[i], IR_min_val[i], IR_max_val[i]);

    // /////////////////////////////////////
    // //Debug
    // for (int i = 0; i < 6; i++)
    // {
    //     if (i == 1)
    //     {

    //         Serial.print(i);
    //         Serial.print(": Reading: ");
    //         Serial.print(reading[0]);
    //         Serial.print(" | ");

    //         Serial.print(": filteredVal: ");
    //         Serial.print(filteredVal[0]);
    //         Serial.print(" | ");

    //         Serial.print(": Scaled val: ");
    //         Serial.print(scaledVal[0]);
    //         Serial.print(" | ");

    //         Serial.print(": IRval: ");
    //         Serial.print(IR_val[0]);
    //         Serial.print(" | ");
    //     }
    //     Serial.println();
    // }
    // /////////////////// DEBUG //////////////////////

    potVar[i] = abs(potCState[i] - potPState[i]); //Valor absoluto

    if (potVar[i] > varThreshold)
    {
        PTime[i] = millis();
    }
    timer[i] = millis() - PTime[i];

    if (timer[i] < TIMEOUT)
    {
        potMoving[i] = true;
    }
    else
    {
        potMoving[i] = false;
    }
}
///////////// FIN LECTURA POT //////////////////////////

///////////// FUNCIONES HILOS  /////////////
//IR0
void IR0()
{
    //Derecha Z
    int i = 0;
    readPot(i);

    if (potMoving[i] == true)
    {
        if ((millis() - lastDebounceTime[i]) > debounceDelay)
        {
            if (IR_val[i] != IR_Pval[i])
            {
                if (IR_val[i] >= 0)
                {
                    lastDebounceTime[i] = millis();

                    if (i == 0) //Envía NOTA  Eje Z canal derecho
                    {
                        // noteOut = NOTE + scaleNotes[escalaSelect][IR_val[i] + octave[OctSel]];
                        // usbMIDI.sendNoteOn(noteOut, 127, MIDI_CH);
                        // usbMIDI.send_now();

                        // noteOut = NOTE + scaleNotes[escalaSelect][IR_Pval[i] + octave[OctSel]]; //Off nota previa
                        // usbMIDI.sendNoteOff(noteOut, 0, MIDI_CH);
                        // usbMIDI.send_now();

                        //Debug
                        int noteOut = NOTE + scaleNotes[escalaSelect][IR_val[i]] + octave[OctSel];

                        Serial.print("IR");
                        Serial.print(i);
                        Serial.print(": ");
                        Serial.print(filteredVal[i]);
                        Serial.print(" | ");
                        Serial.print(noteOut);
                        Serial.print(" | ");
                        Serial.print(scaleNotes[escalaSelect][IR_val[i]]);
                        //Serial.print(a);
                        Serial.println("    ");
                        ////// DEBUG /////////////////////

                        note_is_playing = true;
                    }
                }
                //Lo que este por debajo de los limites del sensor
                if (filteredVal[i] > 800)
                {
                    if (note_is_playing == true)
                    {
                        for (byte i = 0; i < 127; i++)
                        {
                            // usbMIDI.sendNoteOff(i, 0, MIDI_CH);
                            // usbMIDI.send_now();
                            Serial.println("apagado");
                        }

                        note_is_playing = false;
                    }
                }
            }
        }
        IR_Pval[i] = IR_val[i]; //Almacena las lecturas anteriores
        potPState[i] = potCState[i];
    }
}
////////////////////////////////////////

//IR1
void IR1()
{
    //Derecha x
    int i = 1;
    readPot(i);

    if (potMoving[i] == true)
    {

        potPState[i] = potCState[i];
    }
}
/////////////////////////////

//IR2
void IR2()
{
    //Derecha y
    int i = 2;
    readPot(i);

    if (potMoving[i] == true)
    {

        potPState[i] = potCState[i];
    }
}
/////////////////////////////////////

//IR3
void IR3()
{
    //Izquierda z
    int i = 3;
    readPot(i);

    if (potMoving[i] == true)
    {

        potPState[i] = potCState[i];
    }
}
///////////////////////////////////

//IR4
void IR4()
{
    //Izquierda x
    int i = 4;
    readPot(i);

    if (potMoving[i] == true)
    {

        potPState[i] = potCState[i];
    }
}
/////////////////////////////////

//IR5
void IR5()
{
    //Izqueirda y
    int i = 5;
    readPot(i);

    if (potMoving[i] == true)
    {

        potPState[i] = potCState[i];
    }
}
///////////////////////////////////