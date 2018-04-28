#include <Arduino.h>

#include <PMS.h>

HardwareSerial Serial1(2); // 16,17

PMS pms(Serial1);
PMS::DATA data;

void setup()
{
  Serial.begin(9600);   // 1,3 - connected via usb
  delay(100);
  Serial.println("Hellloooooo");
  Serial.println(RX);
  Serial.println(TX);
  Serial.println(SDA);
  Serial.println(SCL);
  Serial1.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
}

void loop()
{
  if (pms.read(data))
  {
    Serial.println("Data:");

    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    Serial.println();
  }
}
