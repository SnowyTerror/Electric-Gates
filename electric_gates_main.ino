    #include <ESP8266WiFi.h>
    #include <BlynkSimpleEsp8266.h>

    // Shift Regsiter
    // 2 - Power Supply
    // 3 - Gate Left Power
    // 4 - Gate Right Power
    // 6 - Gate Switch
    // 7 - LEDs

    // Button stoppers
    int btnLeftStopperOpened = D5;
    int btnLeftStopperClosed = D6;
    int btnRightStopperOpened = D7;
    int btnRightStopperClosed = D3;

    int btnLeftStopperOpenedState = LOW;
    int btnLeftStopperClosedState = LOW;
    int btnRightStopperOpenedState = LOW;
    int btnRightStopperClosedState = LOW;

    // Shift register pins
    int latchPin = D0;
    int clockPin = D1;
    int dataPin = D2;
    byte data = 0b00000000;

    // WiFi details
    char auth[] = "0a5a18c1a9f2457eb842013ec8f5fef3";
    char ssid[] = "SKYCEDB2";
    char pass[] = "LVDLBSBQSC";

    // Blynk digital LED
    WidgetLED indicatorLED(V3);

    // Button booleans
    bool openGateBool = false;
    bool closeGateBool = false;
    bool emergencyStopBool = false;

    void setup() {
      Serial.begin(9600);
      Blynk.begin(auth, ssid, pass);

      // Inputs
      pinMode(btnLeftStopperOpened, INPUT);
      pinMode(btnLeftStopperClosed, INPUT);
      pinMode(btnRightStopperOpened, INPUT);
      pinMode(btnRightStopperClosed, INPUT);

      // Outputs
      pinMode(latchPin, OUTPUT);
      pinMode(clockPin, OUTPUT);
      pinMode(dataPin, OUTPUT);

      // Clear Shift Register
      bitClear(data, 2);
      bitClear(data, 3);
      bitClear(data, 4);
      bitClear(data, 6);
      bitClear(data, 7);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, data);
      digitalWrite(latchPin, HIGH);
    }

    // Blynk virtual buttons
    BLYNK_WRITE(V0) {
      openGateBool = !openGateBool;
    }

    BLYNK_WRITE(V1) {
      closeGateBool = !closeGateBool;
    }

    BLYNK_WRITE(V2) {
      emergencyStopBool = !emergencyStopBool;
    }

    void loop() {
      Blynk.run();

      // Detect Button States
      btnLeftStopperOpenedState = digitalRead(btnLeftStopperOpened);
      btnRightStopperOpenedState = digitalRead(btnRightStopperOpened);
      btnLeftStopperClosedState = digitalRead(btnLeftStopperClosed);
      btnRightStopperClosedState = digitalRead(btnRightStopperClosed);

      // Stability Delay
      delay(10);

      // Open gate function
      if((openGateBool == true) && (closeGateBool == false)) {

        // Initial Gate Check
        if((btnLeftStopperOpenedState == LOW) && (btnRightStopperOpenedState == LOW)) {
          bitSet(data, 2);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);
        }

        // If both gates are opened
        if((btnLeftStopperOpenedState == HIGH) && (btnRightStopperOpenedState == HIGH)) {
          bitClear(data, 2);
          bitClear(data, 3);
          bitClear(data, 4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);

          // Blynk LED indicator
          Blynk.setProperty(V3, "color", "#23C48E");
          indicatorLED.on();
        }

        // Left Gate Opening
        if(btnLeftStopperOpenedState == LOW) {
          bitSet(data, 3);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);
        } else {
          bitClear(data, 3);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);
        }

        // Right Gate Opening
        if(btnRightStopperOpenedState == LOW) {
          bitSet(data, 4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);
        } else {
          bitClear(data, 4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);
        }

        // Emergency Stop button
        if(emergencyStopBool == true) {
          bitClear(data, 2);
          bitClear(data, 3);
          bitClear(data, 4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);

          // Blynk LED indicator
          Blynk.setProperty(V3, "color", "#D3435C");
          indicatorLED.on();
          delay(900);
          indicatorLED.off();
          delay(900);
        }
      }

      // Close gate function
      if((openGateBool == false) && (closeGateBool == true)) {

        // Initial Gate Check
        if((btnLeftStopperClosedState == LOW) && (btnRightStopperClosedState == LOW)) {
          bitSet(data, 2);
          bitSet(data, 6);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);
        }

        // If both gates are closed
        if((btnLeftStopperClosedState == LOW) && (btnRightStopperClosedState == LOW)) {
          bitClear(data, 2);
          bitClear(data, 3);
          bitClear(data, 4);
          bitClear(data, 6);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);

          // Blynk LED indicator
          Blynk.setProperty(V3, "color", "#D3435C");
          indicatorLED.on();
        }

        // Left Gate Closing
        if(btnLeftStopperClosedState == LOW) {
          bitSet(data, 3);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);
        } else {
          bitClear(data, 3);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);
        }

        // Right Gate Closing
        if(btnRightStopperClosedState == LOW) {
          bitSet(data, 4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);
        } else {
          bitClear(data, 4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);
        }

        // Emergency Stop button
        if(emergencyStopBool == true) {
          bitClear(data, 2);
          bitClear(data, 3);
          bitClear(data, 4);
          bitClear(data, 6);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, data);
          digitalWrite(latchPin, HIGH);

          // Blynk LED indicator
          Blynk.setProperty(V3, "color", "#D3435C");
          indicatorLED.on();
          delay(900);
          indicatorLED.off();
          delay(900);
        }
      }

      // Checks gate status
      if((btnLeftStopperClosedState == LOW) && (btnRightStopperClosedState == LOW) && (btnLeftStopperOpenedState == LOW) && (btnRightStopperOpenedState == LOW)) {
        indicatorLED.off();
      }

      // Checks if both buttons are false
      if((openGateBool == false) && (closeGateBool == false)) {
        bitClear(data, 2);
        bitClear(data, 3);
        bitClear(data, 4);
        bitClear(data, 6);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, data);
        digitalWrite(latchPin, HIGH);
      }

      // Emergency Stop button
      if(emergencyStopBool == true) {
        bitClear(data, 2);
        bitClear(data, 3);
        bitClear(data, 4);
        bitClear(data, 6);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, data);
        digitalWrite(latchPin, HIGH);

        // Blynk LED indicator
        Blynk.setProperty(V3, "color", "#D3435C");
        indicatorLED.on();
        delay(900);
        indicatorLED.off();
        delay(900);
      }

    } // END //
