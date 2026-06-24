#include <Wire.h>
#include <Adafruit_BME280.h>

#define MQ135_PIN A1  
#define SOIL1_PIN A2  
#define SOIL2_PIN A5  
#define LDR_PIN   A4  

#define RELAY_PIN 24  

#define SOIL_MIN 40
#define SOIL_TARGET 65
#define TEMP_MAX 26
#define TEMP_MIN 15
#define HUM_MAX 70

Adafruit_BME280 bme;

float temperature;
float humidity;
float pressure;

int soil1;
int soil2;
int ldrValue;
int mq135Raw;

void setup() 
{
    Serial.begin(9600);

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH); 

    if (!bme.begin(0x76)) 
    {
        Serial.println("BME280 bulunamadi! Sistem durduruldu.");
        while (1);
    }

    Serial.println("Marsalda Sistem Basladi");
}

void loop() 
{
    readSensors();
    printData();
    controlSoil();
    controlClimate();
    controlLight();
    
    delay(5000); 
}

void readSensors() 
{
    temperature = bme.readTemperature();
    humidity    = bme.readHumidity();
    pressure    = bme.readPressure() / 100.0F;

    soil1    = analogRead(SOIL1_PIN);
    soil2    = analogRead(SOIL2_PIN);
    ldrValue = analogRead(LDR_PIN);
    mq135Raw = analogRead(MQ135_PIN);
}

int soilPercent(int raw) 
{
    int percent = map(raw, 850, 350, 0, 100);
    return constrain(percent, 0, 100); 
}

int getAvgSoil() 
{
    return (soilPercent(soil1) + soilPercent(soil2)) / 2;
}

void controlSoil() 
{
    int avgSoil = getAvgSoil();

    if (avgSoil < SOIL_MIN) 
    {
        Serial.println("TOPRAK KURU -> POMPA ACILDI");
        digitalWrite(RELAY_PIN, LOW); 
    } 
    else if (avgSoil >= SOIL_TARGET) 
    {
        if (digitalRead(RELAY_PIN) == LOW) 
        { 
            digitalWrite(RELAY_PIN, HIGH);
            Serial.println("HEDEF NEME ULASILDI -> POMPA KAPANDI");
        }
    }
}

void controlClimate() 
{
    if (temperature > TEMP_MAX) Serial.println("[UYARI] FAN CALISMALI");
    if (temperature < TEMP_MIN) Serial.println("[UYARI] ISITICI CALISMALI");
    if (humidity > HUM_MAX)     Serial.println("[UYARI] HAVALANDIRMA CALISMALI");
}

void controlLight() 
{
    if (ldrValue < 200) 
    {
        Serial.println("[TEHLIKE] LED ARIZASI OLABILIR VEYA ORTAM COK KARANLIK");
    }
}

void printData() 
{
    Serial.println("\n--- MARSALDA VERI RAPORU ---");
    Serial.print("Sicaklik: "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("Nem:      "); Serial.print(humidity);    Serial.println(" %");
    Serial.print("Basinc:   "); Serial.print(pressure);    Serial.println(" hPa");
    Serial.print("Toprak 1: "); Serial.print(soilPercent(soil1)); Serial.println(" %");
    Serial.print("Toprak 2: "); Serial.print(soilPercent(soil2)); Serial.println(" %");
    Serial.print("Ort. Nem: "); Serial.print(getAvgSoil());      Serial.println(" %");
    Serial.print("LDR:      "); Serial.println(ldrValue);
    Serial.print("MQ135:    "); Serial.println(mq135Raw);
}
