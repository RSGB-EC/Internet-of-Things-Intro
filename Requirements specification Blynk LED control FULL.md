# Requirements Specification

# Arduino Nano 33 IoT Blynk LED Control and Voltage Monitor

## 1. Project title

**Arduino Nano 33 IoT Internet of Things Demonstrator with Blynk LED Control and Voltage Monitoring**

## 2. Purpose

The purpose of this project is to create a simple Internet of Things demonstrator using an Arduino Nano 33 IoT and the Blynk platform.

The project shall allow a user to:

1. Switch an external LED on and off from a smartphone dashboard.
2. Measure an external DC voltage between 0 and 5 V.
3. Display the measured voltage on the same Blynk dashboard using a gauge widget.

The project is intended as an introductory IoT exercise for novice experimenters and radio amateurs. It demonstrates the basic principles of remote control, remote monitoring, Wi-Fi connectivity, cloud-based dashboards, and safe interfacing between external equipment and a microcontroller.

## 3. Scope

The system shall consist of:

- An Arduino Nano 33 IoT development board.
- A Blynk IoT device and dashboard.
- One external LED connected to a digital output.
- One external DC voltage input connected through a resistor divider to an analogue input.
- Firmware written using the Arduino IDE.
- Smartphone control and display using the Blynk mobile app.

The project shall demonstrate low-voltage monitoring and control only.

The project shall not directly control mains-powered equipment, transmitters, relays, motors, lamps, power amplifiers, or other significant loads without additional interface circuitry.

## 4. System overview

The system shall use the following data paths:

For LED control:

```text
Blynk mobile switch
→ Blynk cloud
→ Wi-Fi
→ Arduino Nano 33 IoT
→ Digital pin D2
→ External LED
```

For voltage monitoring:

```text
External 0 to 5 V DC signal
→ Resistor divider
→ Arduino analogue input A0
→ Arduino firmware calculation
→ Wi-Fi
→ Blynk cloud
→ Blynk mobile gauge
```

The smartphone shall not communicate directly with the Arduino. Both the phone and the Arduino shall communicate through the Blynk cloud service.

## 5. Hardware requirements

### 5.1 Controller board

| ID    | Requirement                                                  |
| ----- | ------------------------------------------------------------ |
| HW-01 | The controller shall be an Arduino Nano 33 IoT.              |
| HW-02 | The board shall be powered from USB during development and testing. |
| HW-03 | The board shall connect to a 2.4 GHz Wi-Fi network.          |
| HW-04 | The project shall use 3.3 V-compatible signals only on the Arduino input and output pins. |
| HW-05 | No Arduino input pin shall be exposed directly to 5 V.       |

### 5.2 LED output

| ID        | Requirement                                                  |
| --------- | ------------------------------------------------------------ |
| HW-LED-01 | One external LED shall be connected to digital pin D2.       |
| HW-LED-02 | The LED shall include a current-limiting resistor.           |
| HW-LED-03 | The recommended LED resistor value shall be 330 Ω.           |
| HW-LED-04 | The LED shall be wired so that setting D2 HIGH turns the LED on. |
| HW-LED-05 | The LED current shall remain within the safe output capability of the Arduino Nano 33 IoT. |

Recommended wiring:

```text
D2 → 330 Ω resistor → LED anode
LED cathode → GND
```

### 5.3 Voltage input

| ID        | Requirement                                                  |
| --------- | ------------------------------------------------------------ |
| HW-VIN-01 | The project shall measure one external DC voltage input.     |
| HW-VIN-02 | The external voltage range shall be 0 to 5 V DC.             |
| HW-VIN-03 | The voltage input shall be connected to analogue input A0 through a resistor divider. |
| HW-VIN-04 | The external voltage source negative connection shall be connected to Arduino GND. |
| HW-VIN-05 | The voltage at A0 shall not exceed 3.3 V under normal operation. |
| HW-VIN-06 | The recommended resistor divider shall use 18 kΩ as the upper resistor and 22 kΩ as the lower resistor. |

Recommended voltage divider:

```text
External voltage + ---- 18 kΩ ----+---- A0
                                  |
                                 22 kΩ
                                  |
External voltage - ---------------+---- Arduino GND
```

The divider ratio shall be:

```text
A0 voltage = input voltage × 22 kΩ / (18 kΩ + 22 kΩ)
```

At 5.0 V input, the voltage at A0 shall be approximately:

```text
5.0 V × 22 / 41 = 2.7 V
```

This keeps the analogue input below 3.3 V.

## 6. Software requirements

### 6.1 Development environment

| ID    | Requirement                                                  |
| ----- | ------------------------------------------------------------ |
| SW-01 | The firmware shall be written as an Arduino sketch.          |
| SW-02 | The firmware shall be developed using the Arduino IDE or Arduino-compatible toolchain. |
| SW-03 | The Arduino Nano 33 IoT board package shall be installed.    |
| SW-04 | The WiFiNINA library shall be installed.                     |
| SW-05 | The Blynk library shall be installed.                        |

### 6.2 Firmware configuration

| ID         | Requirement                                                  |
| ---------- | ------------------------------------------------------------ |
| SW-CONF-01 | The firmware shall include the Blynk Template ID.            |
| SW-CONF-02 | The firmware shall include the Blynk Template Name.          |
| SW-CONF-03 | The firmware shall include the Blynk Auth Token for the device. |
| SW-CONF-04 | The firmware shall include the Wi-Fi SSID.                   |
| SW-CONF-05 | The firmware shall include the Wi-Fi password.               |
| SW-CONF-06 | Credentials should be stored separately from the main sketch if the project is to be shared or published. |

### 6.3 LED control firmware

| ID        | Requirement                                                  |
| --------- | ------------------------------------------------------------ |
| SW-LED-01 | The firmware shall configure digital pin D2 as an output.    |
| SW-LED-02 | The firmware shall set the LED output off during startup.    |
| SW-LED-03 | The firmware shall receive LED commands from Blynk Virtual Pin V0. |
| SW-LED-04 | A value of 0 received on V0 shall switch the LED off.        |
| SW-LED-05 | A value of 1 received on V0 shall switch the LED on.         |
| SW-LED-06 | The firmware shall use a `BLYNK_WRITE(V0)` callback or equivalent Blynk mechanism to process LED commands. |

### 6.4 Voltage measurement firmware

| ID        | Requirement                                                  |
| --------- | ------------------------------------------------------------ |
| SW-VIN-01 | The firmware shall configure A0 as the voltage measurement input. |
| SW-VIN-02 | The firmware shall use analogue-to-digital conversion to read the voltage at A0. |
| SW-VIN-03 | The firmware shall calculate the voltage present at A0.      |
| SW-VIN-04 | The firmware shall calculate the original external voltage by correcting for the resistor divider ratio. |
| SW-VIN-05 | The firmware shall send the calculated external voltage to Blynk Virtual Pin V1. |
| SW-VIN-06 | The firmware shall update the voltage value approximately once per second. |
| SW-VIN-07 | The firmware should average multiple ADC readings to improve display stability. |
| SW-VIN-08 | The displayed voltage shall be expressed in volts.           |

The calculation shall be based on the following principle:

```text
A0 voltage = ADC reading × ADC reference voltage / ADC maximum count
```

Then:

```text
External voltage = A0 voltage × (R1 + R2) / R2
```

For the recommended resistor divider:

```text
External voltage = A0 voltage × (18 kΩ + 33 kΩ) / 33 kΩ
External voltage = A0 voltage × 1.545
```

### 6.5 Timing and program structure

| ID         | Requirement                                                  |
| ---------- | ------------------------------------------------------------ |
| SW-TIME-01 | The main loop shall call `Blynk.run()` frequently.           |
| SW-TIME-02 | The main loop shall not contain long blocking delays.        |
| SW-TIME-03 | Voltage updates shall be controlled using a software timer, such as `BlynkTimer`. |
| SW-TIME-04 | Voltage readings shall not be sent continuously on every pass through the main loop. |
| SW-TIME-05 | The recommended voltage update interval shall be 1000 ms.    |

## 7. Blynk requirements

### 7.1 Template

| ID         | Requirement                                                  |
| ---------- | ------------------------------------------------------------ |
| BL-TEMP-01 | A Blynk template shall be created for the project.           |
| BL-TEMP-02 | The suggested template name shall be `Nano33IoT LED Voltage Demo`. |
| BL-TEMP-03 | The hardware type shall be set to Arduino or equivalent.     |
| BL-TEMP-04 | The connection type shall be set to Wi-Fi.                   |

### 7.2 Device

| ID        | Requirement                                                  |
| --------- | ------------------------------------------------------------ |
| BL-DEV-01 | A Blynk device shall be created from the template.           |
| BL-DEV-02 | The device shall have its own Blynk Auth Token.              |
| BL-DEV-03 | The Auth Token shall be copied into the Arduino sketch.      |
| BL-DEV-04 | The device shall appear online in Blynk when the Arduino is powered and connected. |

### 7.3 LED datastream

| ID        | Requirement                                               |
| --------- | --------------------------------------------------------- |
| BL-LED-01 | A datastream shall be created for LED control.            |
| BL-LED-02 | The LED datastream shall use Virtual Pin V0.              |
| BL-LED-03 | The LED datastream name shall be `LED Switch` or similar. |
| BL-LED-04 | The LED datastream data type shall be Integer.            |
| BL-LED-05 | The minimum value shall be 0.                             |
| BL-LED-06 | The maximum value shall be 1.                             |
| BL-LED-07 | The value 0 shall represent LED off.                      |
| BL-LED-08 | The value 1 shall represent LED on.                       |

### 7.4 Voltage datastream

| ID        | Requirement                                                  |
| --------- | ------------------------------------------------------------ |
| BL-VIN-01 | A datastream shall be created for voltage monitoring.        |
| BL-VIN-02 | The voltage datastream shall use Virtual Pin V1.             |
| BL-VIN-03 | The voltage datastream name shall be `Measured Voltage` or similar. |
| BL-VIN-04 | The voltage datastream data type shall be Double or equivalent numeric type. |
| BL-VIN-05 | The minimum value shall be 0.                                |
| BL-VIN-06 | The maximum value shall be 5.                                |
| BL-VIN-07 | The unit shall be volts, shown as `V`.                       |
| BL-VIN-08 | The displayed value should use one or two decimal places.    |

### 7.5 Mobile dashboard

| ID         | Requirement                                                  |
| ---------- | ------------------------------------------------------------ |
| BL-DASH-01 | The Blynk mobile dashboard shall include a switch widget for LED control. |
| BL-DASH-02 | The switch widget shall be connected to the V0 LED datastream. |
| BL-DASH-03 | The switch widget shall operate as a maintained switch, not a momentary push button. |
| BL-DASH-04 | The Blynk mobile dashboard shall include a gauge widget for voltage monitoring. |
| BL-DASH-05 | The gauge widget shall be connected to the V1 voltage datastream. |
| BL-DASH-06 | The gauge minimum shall be 0 V.                              |
| BL-DASH-07 | The gauge maximum shall be 5 V.                              |
| BL-DASH-08 | The gauge shall display the measured voltage in volts.       |

## 8. Functional requirements

| ID    | Requirement                                                  |
| ----- | ------------------------------------------------------------ |
| FR-01 | The system shall connect to the configured Wi-Fi network after power-up. |
| FR-02 | The system shall connect to the Blynk cloud service after Wi-Fi connection. |
| FR-03 | The Blynk dashboard shall indicate that the device is online when connected. |
| FR-04 | The user shall be able to switch the LED on from the Blynk mobile dashboard. |
| FR-05 | The user shall be able to switch the LED off from the Blynk mobile dashboard. |
| FR-06 | The system shall measure the external voltage connected to the divider input. |
| FR-07 | The system shall calculate and display the external voltage, not merely the reduced voltage at A0. |
| FR-08 | The voltage gauge shall update approximately once per second while the device is online. |
| FR-09 | The LED control and voltage monitoring functions shall operate at the same time. |
| FR-10 | The system shall continue to call the Blynk service while taking voltage readings. |

## 9. Non-functional requirements

### 9.1 Usability

| ID         | Requirement                                                  |
| ---------- | ------------------------------------------------------------ |
| NFR-USE-01 | The dashboard shall be simple enough for a novice user to understand. |
| NFR-USE-02 | The LED control shall be labelled clearly.                   |
| NFR-USE-03 | The voltage gauge shall clearly show the measured value and unit. |
| NFR-USE-04 | The project should be suitable for explanation in a beginner-level technical article. |

### 9.2 Reliability

| ID         | Requirement                                                  |
| ---------- | ------------------------------------------------------------ |
| NFR-REL-01 | The device shall reconnect automatically after a power cycle, provided Wi-Fi and internet access are available. |
| NFR-REL-02 | The device shall not require manual intervention after a normal restart. |
| NFR-REL-03 | The LED shall default to off at startup unless state restoration is deliberately enabled. |
| NFR-REL-04 | The voltage monitor shall continue updating while the device remains online. |

### 9.3 Security

| ID         | Requirement                                                  |
| ---------- | ------------------------------------------------------------ |
| NFR-SEC-01 | The Blynk Auth Token shall not be published.                 |
| NFR-SEC-02 | The Wi-Fi password shall not be published.                   |
| NFR-SEC-03 | Credentials should be moved to a separate secrets file if the sketch is shared. |
| NFR-SEC-04 | The system shall not be used for safety-critical remote control without further design work. |

### 9.4 Safety

| ID          | Requirement                                                  |
| ----------- | ------------------------------------------------------------ |
| NFR-SAFE-01 | The project shall only be used with low-voltage DC signals during the demonstration. |
| NFR-SAFE-02 | No mains voltage shall be connected to the Arduino, breadboard or Blynk-controlled circuit. |
| NFR-SAFE-03 | External voltages above 5 V shall not be connected to the voltage input without redesigning the divider and protection circuit. |
| NFR-SAFE-04 | Loads requiring more current than an Arduino GPIO pin can safely supply shall require a suitable driver circuit. |
| NFR-SAFE-05 | Any future shack equipment control shall include appropriate isolation, protection and fail-safe behaviour. |

## 10. Interface requirements

### 10.1 Physical interfaces

| Signal        | Arduino pin | Direction         | Purpose                                    |
| ------------- | ----------- | ----------------- | ------------------------------------------ |
| LED output    | D2          | Output            | Drives external indicator LED              |
| Voltage input | A0          | Input             | Measures scaled external voltage           |
| Ground        | GND         | Common            | Shared reference for LED and voltage input |
| USB           | USB port    | Power/programming | Provides power and programming interface   |

### 10.2 Blynk virtual interfaces

| Function        | Blynk virtual pin | Direction     | Data type | Range    |
| --------------- | ----------------- | ------------- | --------- | -------- |
| LED control     | V0                | App to device | Integer   | 0 to 1   |
| Voltage display | V1                | Device to app | Double    | 0 to 5 V |

## 11. Calibration requirements

| ID     | Requirement                                                  |
| ------ | ------------------------------------------------------------ |
| CAL-01 | The system shall provide an approximate voltage reading suitable for demonstration. |
| CAL-02 | For improved accuracy, the measured value should be compared with a digital multimeter. |
| CAL-03 | If required, a calibration factor may be added to the firmware. |
| CAL-04 | Calibration should be performed using one or more known voltages within the 0 to 5 V range. |
| CAL-05 | The calibration process shall not require changes to the Blynk dashboard. |

Example calibration principle:

```text
Calibration factor = actual voltage / displayed voltage
```

If a multimeter reads 4.00 V and Blynk displays 3.92 V:

```text
Calibration factor = 4.00 / 3.92 = 1.0204
```

The calculated voltage can then be multiplied by this factor in the firmware.

## 12. Test and acceptance criteria

### 12.1 LED control tests

| Test ID  | Test                                  | Expected result                            |
| -------- | ------------------------------------- | ------------------------------------------ |
| T-LED-01 | Upload firmware to Arduino            | Firmware compiles and uploads successfully |
| T-LED-02 | Power the Arduino                     | LED remains off at startup                 |
| T-LED-03 | Turn Blynk switch on                  | LED turns on                               |
| T-LED-04 | Turn Blynk switch off                 | LED turns off                              |
| T-LED-05 | Repeat switch operation several times | LED follows the dashboard switch reliably  |

### 12.2 Voltage measurement tests

| Test ID  | Test                                                 | Expected result                                         |
| -------- | ---------------------------------------------------- | ------------------------------------------------------- |
| T-VIN-01 | Connect 0 V to the voltage input                     | Gauge displays approximately 0 V                        |
| T-VIN-02 | Connect a known voltage of approximately 1 V         | Gauge displays approximately 1 V                        |
| T-VIN-03 | Connect a known voltage of approximately 2.5 V       | Gauge displays approximately 2.5 V                      |
| T-VIN-04 | Connect a known voltage of approximately 5 V         | Gauge displays approximately 5 V                        |
| T-VIN-05 | Measure A0 voltage with 5 V applied to divider input | A0 voltage is approximately 3.24 V                      |
| T-VIN-06 | Leave the system running for 10 minutes              | Gauge continues to update approximately once per second |

### 12.3 Combined system tests

| Test ID  | Test                                               | Expected result                                              |
| -------- | -------------------------------------------------- | ------------------------------------------------------------ |
| T-SYS-01 | Operate LED switch while voltage gauge is updating | LED control remains responsive                               |
| T-SYS-02 | Power-cycle the Arduino                            | Device reconnects to Wi-Fi and Blynk                         |
| T-SYS-03 | Reopen the Blynk app                               | Dashboard shows the device online and voltage updates resume |
| T-SYS-04 | Disconnect Wi-Fi temporarily                       | Device loses connection but does not create unsafe output behaviour |
| T-SYS-05 | Restore Wi-Fi                                      | Device reconnects automatically where supported by the firmware and network |

## 13. Assumptions

The project assumes that:

- The user has a working Blynk account.
- The Arduino Nano 33 IoT can connect to the local 2.4 GHz Wi-Fi network.
- The Wi-Fi network has internet access.
- The external voltage being measured is DC.
- The external voltage remains between 0 and 5 V.
- The external voltage source can share a common ground with the Arduino.
- The project is being used for experimentation and demonstration, not safety-critical control.

## 14. Constraints

| ID     | Constraint                                                   |
| ------ | ------------------------------------------------------------ |
| CON-01 | The Arduino Nano 33 IoT uses 3.3 V logic.                    |
| CON-02 | The analogue input must not be driven above 3.3 V.           |
| CON-03 | The project depends on the Blynk cloud service.              |
| CON-04 | The project depends on Wi-Fi and internet availability.      |
| CON-05 | The LED output is only suitable for a small indicator LED unless additional driver circuitry is added. |
| CON-06 | The voltage measurement is approximate unless calibrated.    |

## 15. Out of scope

The following are outside the scope of this version:

- Mains voltage measurement.
- Direct control of mains-powered equipment.
- Direct control of a transmitter.
- Direct relay coil drive from an Arduino pin.
- Battery operation.
- Enclosure design.
- PCB design.
- Over-the-air firmware updates.
- Local web server control.
- MQTT integration.
- Multiple analogue channels.
- Data logging beyond the Blynk dashboard.
- Alarm notifications.
- Fail-safe interlock design for remote transmitting equipment.

## 16. Possible future enhancements

Future versions could include:

- Multiple voltage inputs.
- Temperature monitoring.
- Battery voltage monitoring.
- Relay control through a transistor or MOSFET driver.
- Opto-isolated inputs and outputs.
- Local manual override switch.
- Status LED showing Wi-Fi or Blynk connection state.
- Calibration menu or stored calibration factor.
- Alert notifications when voltage is too high or too low.
- Enclosure and front-panel design.
- Integration with shack power distribution.
- Monitoring of forward and reflected power using suitable RF detector circuitry.
- Local fail-safe logic for station equipment control.

## 17. Completion definition

The project shall be considered complete when:

1. The Arduino Nano 33 IoT connects successfully to Wi-Fi and Blynk.
2. The Blynk mobile dashboard shows the device online.
3. The dashboard switch controls the external LED connected to D2.
4. The external voltage input can measure 0 to 5 V through the resistor divider.
5. The Blynk gauge displays the calculated external voltage on V1.
6. LED control and voltage monitoring operate together without interfering with each other.
7. The system can be power-cycled and resumes operation without needing the sketch to be reloaded.
8. The wiring, Blynk datastreams, dashboard configuration and Arduino firmware are documented clearly enough for a novice experimenter to reproduce the project.