#include <Arduino.h>
#include <thingBoard.h>
#include <DHT.h>

// define your own Telemetery class
class DhtTelemetry : public ITelemetry {
  // setup peripherals
  DHT dht;
  static const int DHT_PIN = 13;

  public:
    void setup() {
      dht.setup(DHT_PIN, DHT::DHT11);
      delay(dht.getMinimumSamplingPeriod() + 100);
    }

    JsonDocument buildJson() override {
      JsonDocument doc;
      doc["temperature"] = telemetry.temperature;
      doc["humidity"]    = telemetry.humidity;
      doc["dewPoint"]    = telemetry.temperature - (100 - telemetry.humidity) / 5;

      return doc;
    }

    bool readTelemetryData() override {
      // read the sensor
      const char *dhtStatus = dht.getStatusString();
      if ( strcmp(dhtStatus, "OK") != 0 ) {
        Serial.printf("DHT read error: %s\n", dhtStatus);
        return false;
      }

      float temp = dht.getTemperature();
      float  hum = dht.getHumidity();
      if ( isnan(temp) || isnan(hum) ) {
        Serial.println("DHT read error: NaN value");
        return false;
      }

      telemetry.temperature = temp;
      telemetry.humidity = hum;

      return true;
    }
};

DhtTelemetry telemetry;
static constexpr uint64_t SLEEP_TIME = 5 * 60 * 1000000ULL;  // 5 mins
const char ACCESS_TOKEN[] = "XXXXXXXXXXXXXXXXXXX"; // the random bit from the test URL

// helper method for entering deep sleep
inline void enterDeepSleep(uint64_t sleep_time) {
  Serial.println("Entering deep sleep...");
  esp_sleep_enable_timer_wakeup(sleep_time);
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  delay(200); // allow the serial time to spin up
  Serial.println("The board awakens");

  telemetry.setVerbose(true);
  const char* ssid     = "MY WIFI NETWORK'S NAME";
  const char* password = "MY WIFI PASSWORD";
  if ( !telemetry.connectWifi(ssid, password) ) {
    Serial.println("Could not connect to WiFi");
    enterDeepSleep(SLEEP_TIME);
  }

  telemetry.setup();
}

void loop() {
  if ( telemetry.publishTelemetry(ACCESS_TOKEN) ) {
    enterDeepSleep(SLEEP_TIME);
  } else {
    Serial.println("Waiting 10 seconds for retry...");
    delay(10000);
  }
}
