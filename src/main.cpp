#include <Arduino.h>

#include <Thread.h>
#include <ThreadController.h>
#include <wire.h>

#include "graficos.h"
#include "lecturaPot.h"

//////////////////////////////////////////////

//THREADS
ThreadController cpu; //Hilo principal, donde se añaden otros hilos
Thread threadIR0;     // Hilo control POT
Thread threadIR1;
Thread threadIR2;
Thread threadIR3;
Thread threadIR4;
Thread threadIR5;
////////////////////////////////////////

////////////////////////////////
//Encoder
void encoder();

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(DATA, INPUT);
  pinMode(CLK, INPUT);

  attachInterrupt(digitalPinToInterrupt(DATA), encoder, LOW);
  delay(50);

  usbMIDI.begin();

  //Hilos
  //Control POT
  threadIR0.setInterval(20); //Cada cuantos ms
  threadIR0.onRun(IR0);      // Función que será llamada
  cpu.add(&threadIR0);       // Añadir el hilo

  threadIR1.setInterval(20); //Cada cuantos ms
  threadIR1.onRun(IR1);      // Función que será llamada
  cpu.add(&threadIR1);       // Añadir el hilo

  threadIR2.setInterval(20); //Cada cuantos ms
  threadIR2.onRun(IR2);      // Función que será llamada
  cpu.add(&threadIR2);       // Añadir el hilo

  threadIR3.setInterval(20); //Cada cuantos ms
  threadIR3.onRun(IR3);      // Función que será llamada
  cpu.add(&threadIR3);       // Añadir el hilo

  threadIR4.setInterval(20); //Cada cuantos ms
  threadIR4.onRun(IR4);      // Función que será llamada
  cpu.add(&threadIR4);       // Añadir el hilo

  threadIR5.setInterval(20); //Cada cuantos ms
  threadIR5.onRun(IR5);      // Función que será llamada
  cpu.add(&threadIR5);       // Añadir el hilo
  //////////////////////////////////////////////////

  iniPantalla();
}

void loop()
{
  BtnEnc.update();
  if (BtnEnc.isDoubleClick())
  {
    MenuConfig();
  }

  if (modoPb == true)
  {
    threadIR0.setInterval(2);
  }
  else
  {
    threadIR0.setInterval(20);
  }

  cpu.run();
}

//////////////////////////////
//Manejo Encoder
void encoder()
{
  static unsigned long ultimaInterrupcion = 0;
  unsigned long tiempoInterrupcion = millis();
  if (tiempoInterrupcion - ultimaInterrupcion > 125)
  {
    if (digitalRead(CLK) == HIGH)
    {
      posicion--;
    }
    else
    {
      posicion++;
    }
    ultimaInterrupcion = tiempoInterrupcion;
  }
}