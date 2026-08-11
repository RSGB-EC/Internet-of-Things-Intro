## Requirements specification: Blynk LED control demo

### 1. Purpose

Create a simple demonstration sketch for an **Arduino Nano 33 IoT** that allows a user to switch an external LED on and off from a smartphone using the **Blynk IoT** app.

### 2. Scope

The demo shall:

| ID   | Requirement                                                  |
| ---- | ------------------------------------------------------------ |
| S-01 | Control one external LED connected to one Nano 33 IoT digital GPIO pin. |
| S-02 | Use Wi-Fi to connect the Nano 33 IoT to Blynk Cloud.         |
| S-03 | Use a Blynk mobile app button as the user interface.         |
| S-04 | Use a Blynk Virtual Pin, proposed as `V0`, to carry the app command. |
| S-05 | Switch the LED fully on or fully off, with no dimming required. |

Out of scope for the first demo: OTA updates, provisioning, multiple LEDs, physical push-button override, battery operation, relays, mains switching, and access control beyond the normal Blynk account/device token model.

### 3. Hardware requirements

| ID    | Requirement                                                  |
| ----- | ------------------------------------------------------------ |
| HW-01 | Board shall be an Arduino Nano 33 IoT or Nano 33 IoT with headers. |
| HW-02 | LED shall be connected to a digital GPIO pin, proposed pin: `D2`. |
| HW-03 | LED shall be wired with a current-limiting resistor. Recommended starting value: **330 Ω**. |
| HW-04 | GPIO output current shall remain below the Nano 33 IoT per-pin limit. The Arduino pinout states a **maximum current per pin of 7 mA**. |
| HW-05 | The circuit shall use 3.3 V-compatible signalling only. The Nano 33 IoT I/O is **3.3 V and not 5 V tolerant**. |
| HW-06 | The board shall be powered over USB during development.      |

Recommended LED wiring:

```
D2  →  330 Ω resistor  →  LED anode
LED cathode  →  GND
```

With a typical red LED, this keeps the LED current modest, roughly 3 to 5 mA depending on LED forward voltage, which is appropriate for the Nano 33 IoT.

### 4. Software requirements

| ID    | Requirement                                                  |
| ----- | ------------------------------------------------------------ |
| SW-01 | Firmware shall be written as an Arduino sketch.              |
| SW-02 | Firmware shall use the Blynk Arduino library. Blynk’s current setup flow requires the Arduino IDE, Blynk library, Template ID, device name, and device authentication details in the firmware. |
| SW-03 | Firmware shall use Wi-Fi credentials for the local 2.4 GHz Wi-Fi network. |
| SW-04 | Firmware shall call `Blynk.begin()` using the Blynk Auth Token and Wi-Fi credentials. Blynk documents this as the mechanism used to get the device online and authenticated with Blynk Cloud. |
| SW-05 | Firmware shall call `Blynk.run()` continuously inside `loop()` to maintain the cloud connection. |
| SW-06 | Firmware shall avoid blocking delays in the main loop. Blynk specifically recommends keeping `loop()` clean and avoiding delays. |
| SW-07 | Firmware shall implement a `BLYNK_WRITE(V0)` handler. Blynk documents `BLYNK_WRITE(vPIN)` as the mechanism for receiving data from the app through a Virtual Pin. |
| SW-08 | Firmware shall interpret `V0 = 1` as LED on and `V0 = 0` as LED off. |

### 5. Blynk application requirements

| ID    | Requirement                                                  |
| ----- | ------------------------------------------------------------ |
| BL-01 | A Blynk Template shall be created for the Nano 33 IoT demo.  |
| BL-02 | The template shall include one datastream: `V0`, type integer, allowed values `0` and `1`. |
| BL-03 | The mobile dashboard shall include one Button widget.        |
| BL-04 | The Button widget shall be attached to datastream `V0`.      |
| BL-05 | The Button widget shall operate in **Switch** mode rather than momentary Push mode. |
| BL-06 | Button display labels should be `Off` and `On`.              |

### 6. Behavioural requirements

| ID    | Requirement                                                  |
| ----- | ------------------------------------------------------------ |
| BR-01 | When the mobile button is set to `On`, the LED shall turn on within approximately 1 second under normal network conditions. |
| BR-02 | When the mobile button is set to `Off`, the LED shall turn off within approximately 1 second under normal network conditions. |
| BR-03 | On power-up, the sketch shall configure the LED pin as an output. |
| BR-04 | On power-up, the LED shall default to off until a command is received, unless Blynk state synchronisation is deliberately added later. |
| BR-05 | The serial monitor should report basic connection status for troubleshooting. |

### 7. Security and configuration requirements

| ID     | Requirement                                                  |
| ------ | ------------------------------------------------------------ |
| SEC-01 | Wi-Fi SSID, Wi-Fi password, Blynk Template ID, and Auth Token shall be kept out of public repositories. |
| SEC-02 | Demo credentials may be hard-coded for a local test sketch, but should later be moved to a separate ignored header file such as `arduino_secrets.h`. |
| SEC-03 | The Blynk Auth Token shall be treated as a device credential. |

### 8. Acceptance criteria

The demo is successful when:

| Test                         | Expected result                                              |
| ---------------------------- | ------------------------------------------------------------ |
| Upload sketch to Nano 33 IoT | Sketch compiles and uploads without errors.                  |
| Open serial monitor          | Device reports Wi-Fi/Blynk connection progress.              |
| Press Blynk button `On`      | External LED turns on.                                       |
| Press Blynk button `Off`     | External LED turns off.                                      |
| Power-cycle board            | Board reconnects to Wi-Fi and Blynk without manual intervention. |
| Leave running for 10 minutes | LED remains controllable from the phone.                     |