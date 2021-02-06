#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

#define PIN 5
#define BAUD_RATE 115200
bool state = true;

// Bluetooth TX -> D2
const int BLUETOOTH_TX = 4;
// Bluetooth RX -> D1
const int BLUETOOTH_RX = 5;

SoftwareSerial ble(BLUETOOTH_TX, BLUETOOTH_RX);

String payload_host = "";
String payload_ble = "";

void setup()
{
    Serial.begin(BAUD_RATE);
    ble.begin(BAUD_RATE);
    Serial.println("hello there");
}

void loop()
{
    if(ble.available() > 0)
    {
        payload_ble = "";
        while(ble.available() > 0)
        {
            payload_ble += static_cast<char>(ble.read());
        }
        Serial.println(payload_ble);
    }
    delay(100);
    ble.write("test\n", 5);
}