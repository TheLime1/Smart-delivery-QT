#include "DHT.h"

#define DHTPIN 7
#define FAN_PIN 4
#define LED_PIN 5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  // t is the ideal temperature
  if (t > 25)
  {
    digitalWrite(FAN_PIN, HIGH); // turn the fan on
    digitalWrite(LED_PIN, LOW);  // turn the LED off
    Serial.println("Fan is ON");
  }
  else
  {
    digitalWrite(FAN_PIN, LOW);  // turn the fan off
    digitalWrite(LED_PIN, HIGH); // turn the LED on
    Serial.println("LED is ON");
  }

  delay(1000);
}
