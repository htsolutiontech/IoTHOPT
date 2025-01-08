/*
 * Creator: Ryan.
 * December, 26, 2024.
 */

#include <Buzzer_Lib.h>

uint32_t lastBuzzerStartTime = 0;
uint8_t lastBuzzerStartInterval = 50;

uint32_t lastBuzzerWarningTime = 0;
uint16_t lastBuzzerWarningInterval = 400;

// volatile bool buzzerState = false;

void buzzer_Setup()
{
    pinMode(BUZZER_PIN, OUTPUT);
    ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(BUZZER_PIN, PWM_CHANNEL);

    buzzer_Start();
}

void buzzer_ON()
{
    ledcWrite(PWM_CHANNEL, 255);
    tone(BUZZER_PIN, 2000);
}

void buzzer_OFF()
{
    noTone(BUZZER_PIN);
    ledcWrite(PWM_CHANNEL, 0);
}

void buzzer_Start()
{
    int melody[] = {500, 1000, 1500, 1300, 2000};
    int noteDurations[] = {200, 200, 200, 300, 400};

    for (int i = 0; i < 5; i++)
    {
        unsigned long currentMillis = millis();

        if (currentMillis - lastBuzzerStartTime >= lastBuzzerStartInterval)
        {
            lastBuzzerStartTime = currentMillis;
            tone(BUZZER_PIN, melody[i]);

            unsigned long noteEndTime = currentMillis + noteDurations[i];
            while (millis() < noteEndTime)
            {
                // delay
            }
            noTone(BUZZER_PIN);
        }
    }
}

void buzzer_Warning()
{
    static uint8_t buzzerState = 0;        // (0 = tắt, 1 = bật)
    static uint32_t buzzerLastChangeTime = 0; 

    uint32_t nowMillis = millis();      

    if (nowMillis - buzzerLastChangeTime >= (buzzerState == 0 ? 100 : 300)) // 100ms tắt, 300ms bật
    {
        buzzerLastChangeTime = nowMillis; 

        if (buzzerState == 0)
        {
            buzzer_ON();  
            // Serial.println("BUZZER ON");
            buzzerState = 1;
        }
        else
        {
            buzzer_OFF();  
            // Serial.println("BUZZER OFF");
            buzzerState = 0;
        }
    }
}
