/*
Escalado y clipping de la señal de los potenciometros
*/
#include <Arduino.h>

//////////////////////
//Range
int IR_entrada_min[6] = {200, 0, 0, 200, 0, 0};             //la posición mínima que queremos leer  z, y, x
int IR_entrada_max[6] = {900, 1023, 1023, 900, 1023, 1023}; //Posicion Máxima del sensor z,y,x

int IR_min_val[6] = {0, 0, 0, 0, 0, 0};            //el valor mínimo de salida
int IR_max_val[6] = {13, 127, 127, 127, 127, 127}; //valor máximo a la salida

byte IR_val[6] = {0};  //Valor actual
byte IR_Pval[6] = {0}; //Valor previo

float IR_curve = 1; //For the fscale function
///////////////////////////////////////////

///////////////////////////////////////////////////
//Check if it's greater than maximun value or less than then the minimum value
int clipValue(int in, int minVal, int maxVal)
{
    int out;

    if (in > maxVal)
    {
        out = maxVal;
    }
    else if (in < minVal)
    {
        out = minVal;
    }
    else
    {
        out = in;
    }
    return out;
}
////////////////// FIN CLIP ///////////////////////////

///////////////////////////
//Mapea y cambia (Escala) la curva del rango de valores
float fscale(float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve)
{

    float OriginalRange = 0;
    float NewRange = 0;
    float zeroRefCurVal = 0;
    float normalizedCurVal = 0;
    float rangedValue = 0;
    boolean invFlag = 0;

    // condition curve parameter
    // limit range

    if (curve > 10)
        curve = 10;
    if (curve < -10)
        curve = -10;

    curve = (curve * -.1);  // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
    curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

    /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println();
   */

    // Check for out of range inputValues
    if (inputValue < originalMin)
    {
        inputValue = originalMin;
    }
    if (inputValue > originalMax)
    {
        inputValue = originalMax;
    }

    // Zero Refference the values
    OriginalRange = originalMax - originalMin;

    if (newEnd > newBegin)
    {
        NewRange = newEnd - newBegin;
    }
    else
    {
        NewRange = newBegin - newEnd;
        invFlag = 1;
    }

    zeroRefCurVal = inputValue - originalMin;
    normalizedCurVal = zeroRefCurVal / OriginalRange; // normalize to 0 - 1 float

    /*
  Serial.print(OriginalRange, DEC);  
   Serial.print("   ");  
   Serial.print(NewRange, DEC);  
   Serial.print("   ");  
   Serial.println(zeroRefCurVal, DEC);  
   Serial.println();  
   */

    // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
    if (originalMin > originalMax)
    {
        return 0;
    }

    if (invFlag == 0)
    {
        rangedValue = (pow(normalizedCurVal, curve) * NewRange) + newBegin;
    }
    else // invert the ranges
    {
        rangedValue = newBegin - (pow(normalizedCurVal, curve) * NewRange);
    }

    return rangedValue;
}
//////////////////////// FIN ESCALADO Y CURVA //////////////////////////////