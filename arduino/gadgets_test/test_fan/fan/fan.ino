#define FAN_PIN 4
String command;

void setup()
{
    pinMode(FAN_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    while (Serial.available())
    {
        command = Serial.readString();
        if (command == "ON")
        {
            digitalWrite(FAN_PIN, HIGH); // turn the fan on
            Serial.println("Fan is ON");
        }
        else if (command == "OFF")
        {
            digitalWrite(FAN_PIN, LOW); // turn the fan off
            Serial.println("Fan is OFF");
        }
    }
}
