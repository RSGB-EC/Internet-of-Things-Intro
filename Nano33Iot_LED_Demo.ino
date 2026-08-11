/*************************************************************
  Arduino Nano 33 IoT + Blynk LED and Voltage Monitor Demo

  Functions:
    - Controls an external LED connected to D2 from Blynk V0.
    - Reads an external 0 to 5 V signal through a resistor divider.
    - Sends the calculated voltage to Blynk V1 for display on a Gauge.

  Hardware:
    LED:
      D2 -> 330 ohm resistor -> LED anode
      LED cathode -> GND

    Voltage input:
      External voltage + -> R1 22k -> A0 -> R2 33k -> GND
      External voltage - ---------------------------> GND

  Board:
    Arduino Nano 33 IoT

  Libraries:
    - WiFiNINA
    - Blynk
 *************************************************************/

// ------------------------------------------------------------
// Blynk device details
// ------------------------------------------------------------

#define BLYNK_TEMPLATE_ID   """
#define BLYNK_TEMPLATE_NAME "Nano33IoT LED Demo"
#define BLYNK_AUTH_TOKEN    ""

#define BLYNK_PRINT Serial

// ------------------------------------------------------------
// Libraries
// ------------------------------------------------------------

#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>

// ------------------------------------------------------------
// Wi-Fi credentials
// ------------------------------------------------------------

char ssid[] = "SSID";
char pass[] = "PASSWORD";

// ------------------------------------------------------------
// Hardware configuration
// ------------------------------------------------------------

const uint8_t LED_PIN = 2;
const uint8_t VOLTAGE_PIN = A0;

const uint8_t LED_ON  = HIGH;
const uint8_t LED_OFF = LOW;

// ------------------------------------------------------------
// ADC and voltage divider configuration
// ------------------------------------------------------------

const float ADC_REFERENCE_VOLTAGE = 3.3;   // Nano 33 IoT logic/reference voltage
const int ADC_RESOLUTION_BITS = 12;
const int ADC_MAX_COUNT = 4095;            // 12-bit ADC gives 0 to 4095

// Voltage divider:
// External voltage + -> R1 -> A0 -> R2 -> GND
const float R1_OHMS = 22000.0;
const float R2_OHMS = 33000.0;

const float VOLTAGE_DIVIDER_FACTOR = (R1_OHMS + R2_OHMS) / R2_OHMS;

// Send voltage to Blynk once per second
const unsigned long VOLTAGE_SEND_INTERVAL_MS = 1000;

// Number of ADC samples to average
const uint8_t ADC_SAMPLES = 16;

bool ledState = false;

BlynkTimer timer;

// ------------------------------------------------------------
// Read external voltage
// ------------------------------------------------------------

float readExternalVoltage()
{
  unsigned long adcTotal = 0;

  for (uint8_t i = 0; i < ADC_SAMPLES; i++)
  {
    adcTotal += analogRead(VOLTAGE_PIN);
    delayMicroseconds(250);
  }

  float adcAverage = adcTotal / (float)ADC_SAMPLES;

  float pinVoltage = (adcAverage * ADC_REFERENCE_VOLTAGE) / ADC_MAX_COUNT;

  float externalVoltage = pinVoltage * VOLTAGE_DIVIDER_FACTOR;

  return externalVoltage;
}

// ------------------------------------------------------------
// Send voltage to Blynk
// ------------------------------------------------------------

void sendVoltageToBlynk()
{
  float voltage = readExternalVoltage();

  Blynk.virtualWrite(V1, voltage);

  Serial.print("Voltage: ");
  Serial.print(voltage, 3);
  Serial.println(" V");
}

// ------------------------------------------------------------
// Blynk callback: connected
// ------------------------------------------------------------

BLYNK_CONNECTED()
{
  Serial.println("Connected to Blynk Cloud");

  /*
    LED state restore is optional.

    If you want the board to restore the last Blynk switch state
    after reconnecting, uncomment this:

    Blynk.syncVirtual(V0);
  */
}

// ------------------------------------------------------------
// Blynk callback: LED switch on V0
// ------------------------------------------------------------

BLYNK_WRITE(V0)
{
  int value = param.asInt();

  ledState = (value != 0);

  digitalWrite(LED_PIN, ledState ? LED_ON : LED_OFF);

  Serial.print("Blynk V0 command received: LED ");
  Serial.println(ledState ? "ON" : "OFF");
}

// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  pinMode(VOLTAGE_PIN, INPUT);

  /*
    The Nano 33 IoT supports configurable ADC resolution.
    Arduino notes that analogReadResolution() applies to Nano 33 IoT,
    and that the default is 10 bits for compatibility.
  */
  analogReadResolution(ADC_RESOLUTION_BITS);

  Serial.println();
  Serial.println("Arduino Nano 33 IoT Blynk LED + Voltage Monitor starting...");
  Serial.print("LED pin: D");
  Serial.println(LED_PIN);
  Serial.print("Voltage input pin: A0");
  Serial.println();

  Serial.print("Voltage divider factor: ");
  Serial.println(VOLTAGE_DIVIDER_FACTOR, 4);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(VOLTAGE_SEND_INTERVAL_MS, sendVoltageToBlynk);

  Serial.println("Setup complete");
}

// ------------------------------------------------------------
// Main loop
// ------------------------------------------------------------

void loop()
{
  Blynk.run();
  timer.run();
}
