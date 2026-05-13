#pragma once

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

/*
  The root CA certificate for the thingBoard API endpoint.

  It is used to setup a secure connection to the thingBoard API.
  Change this at your own risk.
*/
const char* root_ca = \
  "-----BEGIN CERTIFICATE-----\n" \
  "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\n" \
  "-----END CERTIFICATE-----\n";

/*
  The ITelemetry class handles the thingBoard communication.

  The implementing class is responsoble for building the JSON payload and reading
  the telemetry from the sensors.

  The ITelemetry class handles the WiFi connection and the actual communication with
  the thingBoard API.
*/

class ITelemetry {
  const char* ssid     = "the-mix-wifi-6";
  const char* password = "The new silly passw0rd 678";

  // as more telemetry is added, this struct can be extended to hold it
  struct STelemetry {
    float temperature;
    float humidity;
  };

  protected:
    // the shared telemetry struct that the user can read from and write to in their implementation of buildJson() and readTelemetry()
    STelemetry telemetry;
    bool verbose = false;

  public:
    // setup the network stuff
    WiFiClientSecure client;

    // abstract methods to be implemented by the user
    virtual JsonDocument buildJson() = 0;
    virtual bool readTelemetryData() = 0;

    virtual ~ITelemetry() = default;

    // connect to wifi. if not successful after 20 seconds, returns false
    virtual bool connectWifi() {
      WiFi.begin(ssid, password);

      log("Connecting to WiFi", false);
      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED){
        delay(500);
        log(".", false);
        if ( attempts++ >= 40 ) return false;
      }

      log(" Connected!");
      return true;
    }

    // set the verbose mode for logging.
    void setVerbose(bool verbose) {
      this->verbose = verbose;
    }
    // log the given message to the serial console if verbose mode is enabled. if newLine is true, a new line will be printed after the message
    void log(char* message, bool newLine = true) {
      if (verbose) {
        Serial.print(message);
        if (newLine) Serial.println();
      }
    }

    /*
      publish the telemetry to thingBoard. returns true if successful
      calls buildJson() to get the payload, so the user can control what is sent to thingBoard
    */
    bool publishTelemetry(const char* accessToken) {
      if (!readTelemetryData()) {
        log("Failed to read telemetry");
        return false;
      }

      char json[256];
      serializeJson(buildJson(), json);

      log("Sending an update");
      log(json);

      char url[100];
      sprintf(url, "https://eu.thingsboard.cloud/api/v1/%s/telemetry", accessToken);

      this->client.setCACert(root_ca);
      HTTPClient https;
      if (https.begin(this->client, url) ) {
        https.addHeader("Content-Type", "application/json");
        int httpResponseCode = https.POST(json);
        https.end();

        return httpResponseCode >= 200 && httpResponseCode < 300;
      }

      return false;
    }
};


