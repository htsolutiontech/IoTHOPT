/*
 * Creator: Ryan.
 * December, 26, 2024.
 */

#include "Oled_Lib.h"

TwoWire OLED_I2C = TwoWire(1);

Adafruit_SSD1306 HOPT_Oled(SCREEN_WIDTH, SCREEN_HEIGHT, &OLED_I2C, OLED_RESET);

void oled_Init()
{
    if (!HOPT_Oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    oled_Display_Logo();
}


void oled_Display_Logo()
{
    HOPT_Oled.clearDisplay();
    HOPT_Oled.drawBitmap(0, 0, HOPT_Custom_Logo, 128, 32, WHITE);
    HOPT_Oled.display();
}

void oled_Blink_Logo()
{
    bool blink = false;

    for (int i = 0; i < 5; i++)
    {
        HOPT_Oled.clearDisplay();
        HOPT_Oled.drawBitmap(0, 0, HOPT_Custom_Logo, 128, 32, WHITE);

        if (blink)
        {
            HOPT_Oled.invertDisplay(true);
        }
        else
        {
            HOPT_Oled.invertDisplay(false);
        }
        blink = !blink;

        HOPT_Oled.display();

        delay(200); 
    }

    oled_Display_Logo(); 
}


// void oled_Blink_Logo()
// {
//     bool blink = false;

//     uint16_t blinkStart = millis();

//     while (millis() - blinkStart < 1500)
//     {
//         HOPT_Oled.clearDisplay();
//         HOPT_Oled.drawBitmap(0, 0, HOPT_Custom_Logo, 128, 32, WHITE);

//         if (blink)
//         {
//             HOPT_Oled.invertDisplay(true);
//         }
//         else
//         {
//             HOPT_Oled.invertDisplay(false);
//         }
//         blink = !blink;

//         HOPT_Oled.display();
//         delay(200);
//     }

//     oled_Display_Logo();
// }

// void oled_Blink_Logo()
// {
//     bool blink = false;
//     uint16_t blinkStart = millis();
//     uint16_t blinkInterval = 300;
//     uint16_t blinkDuration = 2000;

//     while (millis() - blinkStart < blinkDuration)
//     {
//         static uint32_t lastBlinkTime = 0;

//         if (millis() - lastBlinkTime >= blinkInterval)
//         {
//             HOPT_Oled.clearDisplay();
//             HOPT_Oled.drawBitmap(0, 0, HOPT_Custom_Logo, 128, 32, WHITE);

//             if (blink)
//             {
//                 HOPT_Oled.invertDisplay(true);
//             }
//             else
//             {
//                 HOPT_Oled.invertDisplay(false);
//             }
//             blink = !blink;

//             HOPT_Oled.display();

//             lastBlinkTime = millis();
//         }
//     }

//     HOPT_Oled.invertDisplay(false);
// }


void oled_Scroll_Logo()
{
    int x = -128; // Vị trí ban đầu của hình (ngoài màn hình bên trái)

    while (x < SCREEN_WIDTH + 5)
    {
        HOPT_Oled.clearDisplay();
        HOPT_Oled.drawBitmap(x, 0, HOPT_Custom_Logo, 128, 32, WHITE);
        HOPT_Oled.display();

        x += 2;
        delay(12);
    }
}

// void oled_Scroll_Logo()
// {
//     uint16_t scrollIntervalTime = 20;

//     int x = -128; // Vị trí ban đầu của hình (ngoài màn hình bên trái)

//     static uint32_t lastScrollTime = 0;

//     while (x < SCREEN_WIDTH + 4)
//     {

//         if (millis() - lastScrollTime >= scrollIntervalTime)
//         {

//             HOPT_Oled.clearDisplay();
//             HOPT_Oled.drawBitmap(x, 0, HOPT_Custom_Logo, 128, 32, WHITE);
//             HOPT_Oled.display();

//             x += 2;

//             lastScrollTime = millis();
//         }
//     }
// }

void oled_Animation()
{
    oled_Scroll_Logo();
    delay(300);
    oled_Blink_Logo();
    delay(300);
    
}