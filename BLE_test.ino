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
    delay(100);
    Serial.println("hello there");
}

void loop()
{
    
    delay(100);
    String ble_msg = read_ble_msg();
    if(ble_msg != "")
        Serial.println(ble_msg.indexOf("color"));
    ble.write("test\n", 5);
}

String filter_payload(String msg)
{   
    String filtred_msg = "";
    for(uint16_t i = 0; i < msg.length(); i++)
    {
        if((msg.charAt(i) > 97) && (msg.charAt(i) <= 122))
            filtred_msg += msg.charAt(i); //leters
        else if((msg.charAt(i) > 48) && (msg.charAt(i) <= 57))
            filtred_msg += msg.charAt(i); //digits
        else if(msg.charAt(i) == '#')
            filtred_msg += msg.charAt(i);
    }
    return filtred_msg;
}

String read_ble_msg()
{
    static String payload_ble = "";
    if(ble.available() > 0)
    {
        while(ble.available() > 0)
        {
            char symbol = static_cast<char>(ble.read());
            payload_ble += symbol;
            if(symbol == '#')
            {
                String ble_msg = filter_payload(payload_ble);
                Serial.println(ble_msg);
                payload_ble = "";
                return ble_msg;
            }
        }    
    }
    return "";
}