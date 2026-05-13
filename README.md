# thingboard-boilerplate
A boiler plate for thingboard stuff, to be included in PlatformIO projects.

## HOWTO
* Add to your platformIO ini file
* In your main.cpp file, create a class extending the ITelemetry class.
  You are free to create your own constructor and add whatever you need for setup, etc. Just make sure to implement the `jsonDocument buildJson()` method and `bool readTelemetryData()` methods. `readTelemetryData()` should return true if succesful and false when not.
* In your loop() call `myClass.publishTelemetry(ACCESS_TOKEN)`, where ACCESS_TOKEN
  is the XXXXXXXXXXXXX in https://eu.thingsboard.cloud/api/v1/XXXXXXXXXXXXXXXXX/telemetry
  This will publish the telemetry to thing Board.


## TELEMETRY
The telemetry is organized as the STelemetry struct, and a protected member of the class for easy access. You are free to use any other thing, as the class does not actually expect you to use it. Just make sure that whatever readTelemetryData writes, buildJson can read.

### Currently available telemetry parameters:
* float temperature
* float humidity

Feel free to fork, add more and create a pull request.

## LICENSE
This software is licensed under the MIT License. See LICENSE for more details.

## AUTHOR
This software was written by Hartog C. de Mik in 2026, free of charge.

