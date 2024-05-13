// How to use the DHT-22 sensor with Arduino
// Temperature and humidity sensor
// + is 3.3v
// - is gnd
// out is 7

// Libraries
#include "DHT.h"

// Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

// Variables
int h;  // Stores humidity value
int t; // Stores temperature value

void setup()
{
    Serial.begin(9600);
    Serial.println("Temperature and Humidity Sensor Test");
    dht.begin();
}

void loop()
{
    // Read data and store it to variables h (humidity) and t (temperature)
    // Reading temperature or humidity takes about 250 milliseconds!
    h = dht.readHumidity();
    t = dht.readTemperature();
    
    // Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %, Temp: ");
    Serial.print(t);
    Serial.println(" ° Celsius");
    
    delay(1000); // Delay 1 sec.
}
