#include <Arduino.h>
#include <Average.h>

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
   A1,A2,A3 = mando derecha ejes z,y,x respectivamente
   A7,A8,A9 = mando izquierda ejes z,y,x respectivamente
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

////////////////////////
//POTENCIOMETROS
void readPot(int i)
{
    reading[i] = analogRead(POT_PIN[i]); //raw reading
    ave[i].push(reading[i]);             //Añadimos los valores al filtro

    filteredVal[i] = ave[i].mean();
    potCState[i] = filteredVal[i];

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

///////////////////////////////////////

///////////// FUNCIONES HILOS  /////////////
//IR0
void IR0()
{
    int i = 0;
    readPot(i);

    if (potMoving[i] == true)
    {
        Serial.print("Pot: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(potCState[i]);

        potPState[i] = potCState[i];
    }
}
////////////////////////////////////////

//IR1
void IR1()
{
    int i = 1;
    readPot(i);

    if (potMoving[i] == true)
    {
        Serial.print("Pot: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(potCState[i]);

        potPState[i] = potCState[i];
    }
}
/////////////////////////////

//IR2
void IR2()
{
    int i = 2;
    readPot(i);

    if (potMoving[i] == true)
    {
        Serial.print("Pot: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(potCState[i]);

        potPState[i] = potCState[i];
    }
}
/////////////////////////////////////

//IR3
void IR3()
{
    int i = 3;
    readPot(i);

    if (potMoving[i] == true)
    {
        Serial.print("Pot: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(potCState[i]);

        potPState[i] = potCState[i];
    }
}
///////////////////////////////////

//IR4
void IR4()
{
    int i = 4;
    readPot(i);

    if (potMoving[i] == true)
    {
        Serial.print("Pot: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(potCState[i]);

        potPState[i] = potCState[i];
    }
}
/////////////////////////////////

//IR5
void IR5()
{
    int i = 5;
    readPot(i);

    if (potMoving[i] == true)
    {
        Serial.print("Pot: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(potCState[i]);

        potPState[i] = potCState[i];
    }
}
///////////////////////////////////