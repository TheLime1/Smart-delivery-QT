#include "DHT.h"

#define DHTPIN 7
#define FAN_PIN 4
#define LED_PIN 5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
float idealTemperature = 25.0;
enum Mode
{
    START,
    IDLE,
    MONITOR
};
Mode currentMode = START;

void setup()
{
    pinMode(FAN_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);
    dht.begin();
}

void loop()
{
    if (Serial.available())
    {
        String command = Serial.readStringUntil('\n');
        command.trim(); // remove any trailing whitespace
        if (command == "idle" || command == "start")
        {
            currentMode = (command == "idle") ? IDLE : START;
            digitalWrite(FAN_PIN, LOW); // turn the fan off
            digitalWrite(LED_PIN, LOW); // turn the LED off
        }
        else
        {
            idealTemperature = command.toFloat();
            currentMode = MONITOR;
        }
    }

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    String dataString = "";

    if (currentMode == MONITOR)
    {
        dataString += String(t, 1);
        dataString += " C";

        if (t > idealTemperature)
        {
            // cooling
            digitalWrite(FAN_PIN, HIGH); // turn the fan on
            digitalWrite(LED_PIN, LOW);  // turn the LED off
        }
        else
        {
            // heating
            digitalWrite(FAN_PIN, LOW);  // turn the fan off
            digitalWrite(LED_PIN, HIGH); // turn the LED on
        }
        delay(1000);
    }
    else if (currentMode == IDLE)
    {
        dataString += String(t, 1);
        dataString += " C";
        delay(2000);
    }
    else // START mode
    {
        dataString += String(t, 1);
        dataString += " C ";
        dataString += String(h, 1);
        dataString += " %";
        delay(2000);
    }

    Serial.println(dataString);
}
