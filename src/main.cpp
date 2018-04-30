#include <Arduino.h>

#include <PMS.h>

HardwareSerial Serial1(2); // 16,17

PMS pms(Serial1);
PMS::DATA data;
int setPin = 5;

void setup()
{
    Serial.begin(9600);  // 1,3 - connected via usb
    Serial1.begin(9600); // GPIO2 (D4 pin on ESP-12E Development Board)

    digitalWrite(setPin, HIGH);
    pinMode(setPin, OUTPUT);
    pms.passiveMode();
}

void loop()
{
    Serial.println("Wake up, wait 20 seconds for stable readings...");
    digitalWrite(setPin, HIGH);
    pms.wakeUp();
    delay(20000);

    Serial.println("Send request read...");
    pms.requestRead();

    bool status = pms.read(data, 10000);
    if (status)
    {
        Serial.print("PM 1.0 (ug/m3): ");
        Serial.println(data.PM_AE_UG_1_0);

        Serial.print("PM 2.5 (ug/m3): ");
        Serial.println(data.PM_AE_UG_2_5);

        Serial.print("PM 10.0 (ug/m3): ");
        Serial.println(data.PM_AE_UG_10_0);

        Serial.println();
    }
    else
    {
        Serial.println("No data.");
    }
    Serial.println("Going to sleep for 10 seconds.");
    pms.sleep();
    digitalWrite(setPin, LOW);
    delay(10000);
}
