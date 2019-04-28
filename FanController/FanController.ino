// Simple dew point controller for basement air conditioning
// strfry, 2019

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

#include "DHT.h"

#include <math.h>


#define FAN_PWM_PIN (3)

DHT dht_indoor(10, DHT22);
DHT dht_outdoor(14, DHT22);

// The sensors use logic pins for 5V supply...
void setup_dht_power() {
  digitalWrite(9, true);
  digitalWrite(13, true);
  
}

int deadTimeCounter = 0;

void setup() {
  setup_dht_power();
  
  Serial.begin(9600);
  Serial.println(F("Humidity logging disabled"));

  dht_indoor.begin();
  dht_outdoor.begin();
}

float absoluteHumidity(DHT& sensor) {
  
  float rh = sensor.readHumidity();
  
  float T = sensor.readTemperature();

   return 6.112 * expf((17.67 * T)/(T+243.5)) * rh * 2.1674 /  (273.15+T);
}

float fanSpeed(float indoorHumidity, float outdoorHumidity) {
  if (outdoorHumidity + 0.5 < indoorHumidity) {
      return 0.4;
  }

  return 0.0;
}

int sort_compare(const void* left, const void* right) {
    float fl = *reinterpret_cast<const float*>(left);
    float fr = *reinterpret_cast<const float*>(right);

    return fl > fr ? -1 : (fl < fr ? 1 : 0);
}

/// Do a repeated measurement, take a sample from the 50 percentile and compute the relative fan speed
float measureAndDecide() {
  const size_t num_samples = 10;
  
  float indoors[num_samples];
  float outdoors[num_samples];

  for (int i = 0; i < num_samples; i++) {
      indoors[i] = absoluteHumidity(dht_indoor);
      outdoors[i] = absoluteHumidity(dht_outdoor);
  }

  qsort(indoors, num_samples, sizeof(indoors[0]), sort_compare);
  qsort(outdoors, num_samples, sizeof(outdoors[0]), sort_compare);

  float indoor = indoors[num_samples / 2];
  float outdoor = outdoors[num_samples / 2];

  float result = fanSpeed(indoor, outdoor);

  return result;
}

void loop() {
  // Wait a few seconds between measurements.
  delay(6000);


  if (deadTimeCounter == 0) {
    float decision = measureAndDecide();
    analogWrite(FAN_PWM_PIN, static_cast<int>(decision * 255.0f));

    deadTimeCounter = 10 * 20; // 10 * 6 seconds * 20 = 20 minutes

    // for testing:
    deadTimeCounter = 0;
  } else {
    deadTimeCounter--;
  }


  //~ // Reading temperature or humidity takes about 250 milliseconds!
  //~ // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  //~ float h = dht_indoor.readHumidity();
  //~ // Read temperature as Celsius (the default)
  //~ float t = dht_indoor.readTemperature();

    //~ // Check if any reads failed and exit early (to try again).
  //~ if (isnan(h) || isnan(t)) {
    //~ Serial.println(F("Failed to read from DHT sensor!"));
    //~ return;
  //~ }

  //~ Serial.print(F("H: "));
  //~ Serial.print(h);
  //~ Serial.print(F(" %  T: "));
  //~ Serial.print(t);
  //~ Serial.println(F(" C"));
  
  //~ Serial.print(indoor);
  //~ Serial.println(F(" g/m^3"));

}
