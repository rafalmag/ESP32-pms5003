#include <Arduino.h>

#include <PMS.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HttpClient.h>
#include "secret.h"

//const char* ssid     = "your-ssid";
//const char* password = "your-password";
//const char* iftttKey = "secret";

// https://maker.ifttt.com/trigger/test/with/key/secret
const char *host = "maker.ifttt.com";
const char *eventId = "test";

String uri = String("/trigger/") + eventId + "/with/key/" + iftttKey;

// last cert from
// openssl s_client -showcerts -connect maker.ifttt.com:443
const char *rootCa = "-----BEGIN CERTIFICATE-----\n"
                     "MIIEADCCAuigAwIBAgIBADANBgkqhkiG9w0BAQUFADBjMQswCQYDVQQGEwJVUzEh\n"
                     "MB8GA1UEChMYVGhlIEdvIERhZGR5IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBE\n"
                     "YWRkeSBDbGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTA0MDYyOTE3\n"
                     "MDYyMFoXDTM0MDYyOTE3MDYyMFowYzELMAkGA1UEBhMCVVMxITAfBgNVBAoTGFRo\n"
                     "ZSBHbyBEYWRkeSBHcm91cCwgSW5jLjExMC8GA1UECxMoR28gRGFkZHkgQ2xhc3Mg\n"
                     "MiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTCCASAwDQYJKoZIhvcNAQEBBQADggEN\n"
                     "ADCCAQgCggEBAN6d1+pXGEmhW+vXX0iG6r7d/+TvZxz0ZWizV3GgXne77ZtJ6XCA\n"
                     "PVYYYwhv2vLM0D9/AlQiVBDYsoHUwHU9S3/Hd8M+eKsaA7Ugay9qK7HFiH7Eux6w\n"
                     "wdhFJ2+qN1j3hybX2C32qRe3H3I2TqYXP2WYktsqbl2i/ojgC95/5Y0V4evLOtXi\n"
                     "EqITLdiOr18SPaAIBQi2XKVlOARFmR6jYGB0xUGlcmIbYsUfb18aQr4CUWWoriMY\n"
                     "avx4A6lNf4DD+qta/KFApMoZFv6yyO9ecw3ud72a9nmYvLEHZ6IVDd2gWMZEewo+\n"
                     "YihfukEHU1jPEX44dMX4/7VpkI+EdOqXG68CAQOjgcAwgb0wHQYDVR0OBBYEFNLE\n"
                     "sNKR1EwRcbNhyz2h/t2oatTjMIGNBgNVHSMEgYUwgYKAFNLEsNKR1EwRcbNhyz2h\n"
                     "/t2oatTjoWekZTBjMQswCQYDVQQGEwJVUzEhMB8GA1UEChMYVGhlIEdvIERhZGR5\n"
                     "IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBEYWRkeSBDbGFzcyAyIENlcnRpZmlj\n"
                     "YXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQEFBQAD\n"
                     "ggEBADJL87LKPpH8EsahB4yOd6AzBhRckB4Y9wimPQoZ+YeAEW5p5JYXMP80kWNy\n"
                     "OO7MHAGjHZQopDH2esRU1/blMVgDoszOYtuURXO1v0XJJLXVggKtI3lpjbi2Tc7P\n"
                     "TMozI+gciKqdi0FuFskg5YmezTvacPd+mSYgFFQlq25zheabIZ0KbIIOqPjCDPoQ\n"
                     "HmyW74cNxA9hi63ugyuV+I6ShHI56yDqg+2DzZduCLzrTia2cyvk0/ZM/iZx4mER\n"
                     "dEr/VxqHD3VILs9RaRegAhJhldXRQLIQTO7ErBBDpqWeCtWVYpoNz4iCxTIM5Cuf\n"
                     "ReYNnyicsbkqWletNw+vHX/bvZ8=\n"
                     "-----END CERTIFICATE-----\n";

HardwareSerial Serial1(2); // 16,17

PMS pms(Serial1);
PMS::DATA data;
int setPin = 5;

void setup()
{
    Serial.begin(9600); // 1,3 - connected via usb

    //pms5003
    Serial1.begin(9600);
    digitalWrite(setPin, HIGH);
    pinMode(setPin, OUTPUT);
    pms.passiveMode();

    //wifi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    Serial.println("Wake up, wait 30 seconds for stable readings...");
    digitalWrite(setPin, HIGH);
    pms.wakeUp();
    delay(30000);

    HTTPClient https;
    https.begin(host, 443, uri, rootCa); //HTTPS
    https.addHeader("Content-Type", "application/json; charset=utf-8");

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

        String req = String("{\"value1\":") + data.PM_AE_UG_1_0 + ",\"value2\":" + data.PM_AE_UG_2_5 + ",\"value3\":" + data.PM_AE_UG_10_0 + "}";
        int httpCode = https.POST(req);

        // httpCode will be negative on error
        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
        }
        else
        {
            Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
    }
    else
    {
        Serial.println("No data.");
    }
    https.end();
    Serial.println("Going to sleep for 30 seconds.");
    pms.sleep();
    digitalWrite(setPin, LOW);
    delay(30000);
}
