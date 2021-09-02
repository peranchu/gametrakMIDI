#include <arduino.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>

#include <PinButton.h>

#include "iconos.h"
#include "scales.h"

//Display
Adafruit_SH1106 display(-1);

//Encoder
volatile unsigned int posicion = 1;
int lectura;

#define DATA 3
#define CLK 4
#define BTN_ENCODER 2
////////////////////////////////////////////

//Botón Encoder
PinButton BtnEnc(BTN_ENCODER); //Instanciación del boton encoder en la librería
unsigned long tiempo1 = 0;
const int intervalo = 3000;
bool EstadoMenu = false;

bool submenu1 = false;
bool submenu2 = false;
bool submenu3 = false;
bool submenu4 = false;
bool submenu5 = false;

//Prototipo
void EntradaMenuConfig();
void MenuConfig();
void menuInicial();
//****************************************************************************

//Configuración Pantalla Inicio
void iniPantalla()
{
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);
    delay(1000);

    display.clearDisplay();
    display.setRotation(0);
    display.setTextWrap(true);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawBitmap(0, 0, artec, 128, 50, 1);
    display.setCursor(10, 54);
    display.println("Arte y Electronica");
    display.display();

    delay(3000);

    menuInicial();
}
////////////////////////////////

///////// MENU INICIAL ///////////////////
//Menu con los valores preestablecidos
void menuInicial()
{
    //Menu inicial
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.drawBitmap(2, 5, notas, 20, 20, 1);
    display.setCursor(32, 10);
    display.println(scaleNames[escalaSelect]);

    display.drawBitmap(2, 29, diapa, 20, 20, 1);
    display.setCursor(32, 34);
    display.println(noteNames[noteSelect]);

    display.drawBitmap(68, 28, midi, 20, 20, 1);
    display.setCursor(98, 34);
    display.println(MIDI_CH);

    display.setCursor(0, 55);
    display.print("OCT:");
    display.println(octavas[OctSel]);

    display.setCursor(62, 55);
    display.print("MODE:");
    display.println(mode[ModeSel]);

    display.display();
}

////////////////////////////////////////

////////////////////////////
//Menú Cofiguración
void MenuConfig()
{
    EstadoMenu = true;

    display.clearDisplay();
    display.setTextSize(1);
    //display.drawRoundRect(0, 4, 26, 22, 1, WHITE);
    display.drawBitmap(2, 5, notas, 20, 20, 1);
    display.setCursor(32, 10);

    //display.drawRoundRect(0, 35, 26, 22, 1, WHITE);
    display.drawBitmap(2, 36, diapa, 20, 20, 1);
    display.setCursor(32, 34);

    //display.drawRoundRect(45, 4, 30, 24, 1, WHITE);
    display.drawBitmap(50, 6, midi, 20, 20, 1);
    display.setCursor(98, 34);
    //display.println(MIDI_CH);

    //display.drawRoundRect(90, 4, 30, 24, 1, WHITE);
    display.drawBitmap(95, 5, flechas, 20, 20, 1);

    //display.drawRoundRect(45, 35, 26, 22, 1, WHITE);
    display.drawBitmap(47, 36, mano, 20, 20, 1);

    //display.drawRoundRect(90, 35, 26, 22, 1, WHITE);
    display.drawBitmap(92, 36, flecha, 20, 20, 1);

    display.display();

    while (EstadoMenu == true)
    {
        if (posicion >= 7)
        {
            posicion = 1;
        }
        if (posicion <= 0)
        {
            posicion = 6;
        }

        if (posicion == 1) //Escalas
        {
            display.drawRoundRect(0, 4, 26, 22, 1, WHITE);
            display.drawRoundRect(0, 35, 26, 22, 1, BLACK);
            display.drawRoundRect(45, 4, 30, 24, 1, BLACK);
            display.drawRoundRect(90, 4, 30, 24, 1, BLACK);
            display.drawRoundRect(45, 35, 26, 22, 1, BLACK);
            display.drawRoundRect(90, 35, 26, 22, 1, BLACK);

            display.display();

            //comprueba el botón
            BtnEnc.update();

            //Vuelve al menu Ajustes
            if (BtnEnc.isLongClick())
            {
                EstadoMenu = false;
                submenu1 = false;
                menuInicial();
                break;
            }

            //Entrada selector de Escala
            if (BtnEnc.isSingleClick())
            {
                display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(38, 0);
                display.println("Escalas");
                display.drawLine(2, 8, 124, 8, 1);
                display.display();

                submenu1 = true;

                //Contador de numero de escalas en el encoder
                while (submenu1)
                {
                    //Ajuste de los valores del cursor
                    if (posicion >= 10)
                    {
                        posicion = 1;
                    }
                    if (posicion <= 0)
                    {
                        posicion = 9;
                    }
                    ////////////////////////////

                    //Pintar Escalas en pantalla
                    if (posicion == 1)
                    {
                        display.fillRect(30, 22, 120, 40, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(BLACK, WHITE);
                        display.setCursor(30, 22);
                        display.print(scaleNames[0]);
                        display.display();

                        BtnEnc.update();
                        //Vuelve al menu principal
                        if (BtnEnc.isLongClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            menuInicial();
                            break;
                        }
                        //Escoje la escala
                        if (BtnEnc.isSingleClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            escalaSelect = posicion - 1;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 2)
                    {
                        display.fillRect(30, 22, 90, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(BLACK, WHITE);
                        display.setCursor(30, 22);
                        display.print(scaleNames[1]);
                        display.display();

                        BtnEnc.update();
                        //Vuelve al menu principal
                        if (BtnEnc.isLongClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            menuInicial();
                            break;
                        }
                        //Escoje la escala
                        if (BtnEnc.isSingleClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            escalaSelect = posicion - 1;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 3)
                    {
                        display.fillRect(30, 22, 90, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(BLACK, WHITE);
                        display.setCursor(30, 22);
                        display.print(scaleNames[2]);
                        display.display();

                        BtnEnc.update();
                        //Vuelve al menu principal
                        if (BtnEnc.isLongClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            menuInicial();
                            break;
                        }
                        //Escoje la escala
                        if (BtnEnc.isSingleClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            escalaSelect = posicion - 1;
                            menuInicial();
                        }
                    }
                    if (posicion == 4)
                    {
                        display.fillRect(30, 22, 120, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(BLACK, WHITE);
                        display.setCursor(30, 22);
                        display.print(scaleNames[3]);
                        display.display();

                        BtnEnc.update();
                        //Vuelve al menu principal
                        if (BtnEnc.isLongClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            menuInicial();
                            break;
                        }
                        //Escoje la escala
                        if (BtnEnc.isSingleClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            escalaSelect = posicion - 1;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 5)
                    {
                        display.fillRect(30, 22, 120, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(BLACK, WHITE);
                        display.setCursor(30, 22);
                        display.print(scaleNames[4]);
                        display.display();

                        BtnEnc.update();
                        //Vuelve al menu principal
                        if (BtnEnc.isLongClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            menuInicial();
                            break;
                        }
                        //Escoje la escala
                        if (BtnEnc.isSingleClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            escalaSelect = posicion - 1;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 6)
                    {
                        display.fillRect(30, 22, 120, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(BLACK, WHITE);
                        display.setCursor(30, 22);
                        display.print(scaleNames[5]);
                        display.display();

                        BtnEnc.update();
                        //Vuelve al menu principal
                        if (BtnEnc.isLongClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            menuInicial();
                            break;
                        }
                        //Escoje la escala
                        if (BtnEnc.isSingleClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            escalaSelect = posicion - 1;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 7)
                    {
                        display.fillRect(30, 22, 120, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(BLACK, WHITE);
                        display.setCursor(30, 22);
                        display.print(scaleNames[6]);
                        display.display();

                        BtnEnc.update();
                        //Vuelve al menu principal
                        if (BtnEnc.isLongClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            menuInicial();
                            break;
                        }
                        //Escoje la escala
                        if (BtnEnc.isSingleClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            escalaSelect = posicion - 1;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 8)
                    {
                        display.fillRect(30, 22, 120, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(BLACK, WHITE);
                        display.setCursor(30, 22);
                        display.print(scaleNames[7]);
                        display.display();

                        BtnEnc.update();
                        //Vuelve al menu principal
                        if (BtnEnc.isLongClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            menuInicial();
                            break;
                        }
                        //Escoje la escala
                        if (BtnEnc.isSingleClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            escalaSelect = posicion - 1;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 9)
                    {
                        display.fillRect(30, 22, 120, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(BLACK, WHITE);
                        display.setCursor(30, 22);
                        display.print(scaleNames[8]);
                        display.display();

                        BtnEnc.update();
                        //Vuelve al menu principal
                        if (BtnEnc.isLongClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            menuInicial();
                            break;
                        }
                        //Escoje la escala
                        if (BtnEnc.isSingleClick())
                        {
                            submenu1 = false;
                            EstadoMenu = false;
                            escalaSelect = posicion - 1;
                            menuInicial();
                            break;
                        }
                    }
                }
            }
        }
        //Sale del bucle principal del Menu config
        if (submenu1 == false && EstadoMenu == false)
        {
            posicion = 1; //Coloca el cursor en la primera posicion
            break;
        }
        /////////////  FIN ESCALAS ////////////

        /////////////////////////////////////////
        //Posicion 2 MIDI
        if (posicion == 2)
        {
            display.drawRoundRect(0, 4, 26, 22, 1, BLACK);
            display.drawRoundRect(0, 35, 26, 22, 1, BLACK);
            display.drawRoundRect(45, 4, 30, 24, 1, WHITE);
            display.drawRoundRect(90, 4, 30, 24, 1, BLACK);
            display.drawRoundRect(45, 35, 26, 22, 1, BLACK);
            display.drawRoundRect(90, 35, 26, 22, 1, BLACK);
            display.display();

            //comprueba el botón
            BtnEnc.update();

            //Vuelve al menu Ajustes
            if (BtnEnc.isLongClick())
            {
                EstadoMenu = false;
                submenu1 = false;
                menuInicial();
                break;
            }

            //Entrada selector canal MIDI
            if (BtnEnc.isSingleClick())
            {
                display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(38, 0);
                display.println("CANAL MIDI");
                display.drawLine(2, 8, 124, 8, 1);
                display.display();

                submenu2 = true;
                posicion = 1;

                while (submenu2)
                {
                    if (posicion >= 128)
                    {
                        posicion = 1;
                    }
                    if (posicion <= 0)
                    {
                        posicion = 127;
                    }
                    display.setTextSize(2);
                    display.fillRect(20, 20, 90, 40, BLACK);
                    display.setCursor(50, 30);
                    display.print(posicion);
                    display.display();

                    BtnEnc.update();

                    //Salir sin seleccionar
                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu2 = false;
                        menuInicial();
                        break;
                    }
                    //Seleccionar el canal MIDI
                    if (BtnEnc.isSingleClick())
                    {
                        submenu2 = false;
                        EstadoMenu = false;
                        MIDI_CH = posicion;
                        menuInicial();
                        break;
                    }
                }
            }
        }
        if (submenu2 == false && EstadoMenu == false)
        {
            posicion = 1;
            break;
        }
        ///////////////////FIN MIDI  //////////////////

        //posicion 3 OCTAVA
        if (posicion == 3)
        {
            display.drawRoundRect(0, 4, 26, 22, 1, BLACK);
            display.drawRoundRect(0, 35, 26, 22, 1, BLACK);
            display.drawRoundRect(45, 4, 30, 24, 1, BLACK);
            display.drawRoundRect(90, 4, 30, 24, 1, WHITE);
            display.drawRoundRect(45, 35, 26, 22, 1, BLACK);
            display.drawRoundRect(90, 35, 26, 22, 1, BLACK);
            display.display();

            BtnEnc.update();

            //Vuelve al menu Ajustes
            if (BtnEnc.isLongClick())
            {
                EstadoMenu = false;
                submenu3 = false;
                menuInicial();
                break;
            }

            //Entrada selector OCTAVA
            if (BtnEnc.isSingleClick())
            {
                display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(38, 0);
                display.println("OCTAVA");
                display.drawLine(2, 8, 124, 8, 1);
                display.display();

                submenu3 = true;
                posicion = 3;

                while (submenu3)
                {
                    if (posicion >= 6)
                    {
                        posicion = 1;
                    }
                    if (posicion <= 0)
                    {
                        posicion = 5;
                    }

                    if (posicion == 1)
                    {
                        display.fillRect(30, 30, 75, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(WHITE);
                        display.setCursor(50, 32);
                        display.print(octave[0]);
                        display.display();

                        BtnEnc.update();

                        if (BtnEnc.isLongClick())
                        {
                            EstadoMenu = false;
                            submenu3 = false;
                            menuInicial();
                            break;
                        }

                        if (BtnEnc.isSingleClick())
                        {
                            EstadoMenu = false;
                            submenu3 = false;
                            OctSel = 0;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 2)
                    {
                        display.fillRect(30, 30, 75, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(WHITE);
                        display.setCursor(50, 32);
                        display.print(octave[1]);
                        display.display();

                        BtnEnc.update();

                        if (BtnEnc.isLongClick())
                        {
                            EstadoMenu = false;
                            submenu3 = false;
                            menuInicial();
                            break;
                        }

                        if (BtnEnc.isSingleClick())
                        {
                            EstadoMenu = false;
                            submenu3 = false;
                            OctSel = 1;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 3)
                    {
                        display.fillRect(30, 30, 75, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(WHITE);
                        display.setCursor(50, 32);
                        display.print(octave[2]);
                        display.display();

                        BtnEnc.update();

                        if (BtnEnc.isLongClick())
                        {
                            EstadoMenu = false;
                            submenu3 = false;
                            menuInicial();
                            break;
                        }

                        if (BtnEnc.isSingleClick())
                        {
                            EstadoMenu = false;
                            submenu3 = false;
                            OctSel = 2;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 4)
                    {
                        display.fillRect(30, 30, 75, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(WHITE);
                        display.setCursor(50, 32);
                        display.print(octave[3]);
                        display.display();

                        BtnEnc.update();

                        if (BtnEnc.isLongClick())
                        {
                            EstadoMenu = false;
                            submenu3 = false;
                            menuInicial();
                            break;
                        }

                        if (BtnEnc.isSingleClick())
                        {
                            EstadoMenu = false;
                            submenu3 = false;
                            OctSel = 3;
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 5)
                    {
                        display.fillRect(30, 30, 75, 200, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(WHITE);
                        display.setCursor(50, 32);
                        display.print(octave[4]);
                        display.display();

                        BtnEnc.update();

                        if (BtnEnc.isLongClick())
                        {
                            EstadoMenu = false;
                            submenu3 = false;
                            menuInicial();
                            break;
                        }

                        if (BtnEnc.isSingleClick())
                        {
                            EstadoMenu = false;
                            submenu3 = false;
                            OctSel = 4;
                            menuInicial();
                            break;
                        }
                    }
                }
            }
        }
        if (submenu3 == false && EstadoMenu == false)
        {
            posicion = 1;
            break;
        }
        /////////////////// FIN OCTAVA ///////////////

        //posicion 4 Tonalidad
        if (posicion == 4)
        {
            display.drawRoundRect(0, 4, 26, 22, 1, BLACK);
            display.drawRoundRect(0, 35, 26, 22, 1, WHITE);
            display.drawRoundRect(45, 4, 30, 24, 1, BLACK);
            display.drawRoundRect(90, 4, 30, 24, 1, BLACK);
            display.drawRoundRect(45, 35, 26, 22, 1, BLACK);
            display.drawRoundRect(90, 35, 26, 22, 1, BLACK);
            display.display();

            //comprueba el botón
            BtnEnc.update();

            //Vuelve al menu Ajustes
            if (BtnEnc.isLongClick())
            {
                EstadoMenu = false;
                submenu4 = false;
                menuInicial();
                break;
            }

            //Entrada selector Tonalidad
            if (BtnEnc.isSingleClick())
            {
                display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(38, 0);
                display.println("TONALIDAD");
                display.drawLine(2, 8, 124, 8, 1);
                display.display();

                submenu4 = true;
                posicion = 1;
            }

            while (submenu4)
            {
                if (posicion >= 13)
                {
                    posicion = 1;
                }
                if (posicion <= 0)
                {
                    posicion = 12;
                }

                if (posicion == 1)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[0]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 2)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[1]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 3)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[2]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 4)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[3]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 5)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[4]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 6)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[5]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 7)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[6]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 8)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[7]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 9)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[8]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 10)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[9]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 11)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[10]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
                if (posicion == 12)
                {
                    display.fillRect(30, 30, 75, 20, BLACK);
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(50, 32);
                    display.print(noteNames[11]);
                    display.display();

                    BtnEnc.update();

                    if (BtnEnc.isLongClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        menuInicial();
                        break;
                    }

                    if (BtnEnc.isSingleClick())
                    {
                        EstadoMenu = false;
                        submenu4 = false;
                        noteSelect = posicion - 1;
                        menuInicial();
                        break;
                    }
                }
            }
        }
        if (submenu4 == false && EstadoMenu == false)
        {
            posicion = 1;
            break;
        }
        ////////////// FIN TONALIDAD /////////

        //posicion 5 MODO
        if (posicion == 5)
        {
            display.drawRoundRect(0, 4, 26, 22, 1, BLACK);
            display.drawRoundRect(0, 35, 26, 22, 1, BLACK);
            display.drawRoundRect(45, 4, 30, 24, 1, BLACK);
            display.drawRoundRect(90, 4, 30, 24, 1, BLACK);
            display.drawRoundRect(45, 35, 26, 22, 1, WHITE);
            display.drawRoundRect(90, 35, 26, 22, 1, BLACK);
            display.display();

            BtnEnc.update();

            //Vuelve al menu Ajustes
            if (BtnEnc.isLongClick())
            {
                EstadoMenu = false;
                submenu5 = false;
                menuInicial();
                break;
            }

            //Entrada selector MODO
            if (BtnEnc.isSingleClick())
            {
                display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(38, 0);
                display.println("MODO");
                display.drawLine(2, 8, 124, 8, 1);
                display.display();

                submenu5 = true;
                posicion = 1;

                while (submenu5)
                {
                    if (posicion >= 3)
                    {
                        posicion = 1;
                    }
                    if (posicion <= 0)
                    {
                        posicion = 2;
                    }

                    if (posicion == 1)
                    {
                        display.fillRect(30, 30, 75, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(WHITE);
                        display.setCursor(50, 32);
                        display.print(mode[0]);
                        display.display();

                        BtnEnc.update();

                        //Vuelve al menu Ajustes
                        if (BtnEnc.isLongClick())
                        {
                            EstadoMenu = false;
                            submenu5 = false;
                            menuInicial();
                            break;
                        }

                        if (BtnEnc.isSingleClick())
                        {
                            EstadoMenu = false;
                            submenu5 = false;
                            ModeSel = posicion - 1;
                            //Estado del modo
                            if (ModeSel == 0)
                            {
                                modoScl = true;
                                modoPb = false;
                            }
                            if (ModeSel == 1)
                            {
                                modoScl = false;
                                modoPb = true;
                            }
                            menuInicial();
                            break;
                        }
                    }
                    if (posicion == 2)
                    {
                        display.fillRect(30, 30, 75, 20, BLACK);
                        display.setTextSize(2);
                        display.setTextColor(WHITE);
                        display.setCursor(50, 32);
                        display.print(mode[1]);
                        display.display();

                        BtnEnc.update();

                        //Vuelve al menu Ajustes
                        if (BtnEnc.isLongClick())
                        {
                            EstadoMenu = false;
                            submenu5 = false;
                            menuInicial();
                            break;
                        }

                        if (BtnEnc.isSingleClick())
                        {
                            EstadoMenu = false;
                            submenu5 = false;
                            ModeSel = posicion - 1;
                            //Estado del modo
                            if (ModeSel == 0)
                            {
                                modoScl = true;
                                modoPb = false;
                            }
                            if (ModeSel == 1)
                            {
                                modoScl = false;
                                modoPb = true;
                            }
                            menuInicial();
                            break;
                        }
                    }
                }
            }
        }
        if (submenu5 == false && EstadoMenu == false)
        {
            posicion = 1;
            break;
        }
        //////////// FIN MODO //////////////////

        //posicion 6  Atrás
        if (posicion == 6)
        {
            display.drawRoundRect(0, 4, 26, 22, 1, BLACK);
            display.drawRoundRect(0, 35, 26, 22, 1, BLACK);
            display.drawRoundRect(45, 4, 30, 24, 1, BLACK);
            display.drawRoundRect(90, 4, 30, 24, 1, BLACK);
            display.drawRoundRect(45, 35, 26, 22, 1, BLACK);
            display.drawRoundRect(90, 35, 26, 22, 1, WHITE);
            display.display();

            BtnEnc.update();

            //Entrada selector MODO
            if (BtnEnc.isSingleClick())
            {
                posicion = 1;
                menuInicial();
                break;
            }
        }
    }
}
////////////// FIN MENU CONFIG ///////////////
