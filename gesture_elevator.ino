#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFun_APDS9960.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Gesture sensor object
SparkFun_APDS9960 apds;

// Variables
int currentFloor = 0;

void setup() {
  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Gesture Elevator");
  display.display();

  // Initialize gesture sensor
  if (apds.init()) {
    Serial.println("APDS9960 initialized");
  } else {
    Serial.println("APDS9960 failed");
  }

  if (apds.enableGestureSensor(true)) {
    Serial.println("Gesture sensor enabled");
  }

  delay(2000);
}

void loop() {
  if (apds.isGestureAvailable()) {
    int gesture = apds.readGesture();

    switch (gesture) {

      case DIR_UP:
        currentFloor++;
        showStatus("UP Gesture", currentFloor);
        break;

      case DIR_DOWN:
        if (currentFloor > 0) {
          currentFloor--;
        }
        showStatus("DOWN Gesture", currentFloor);
        break;

      case DIR_LEFT:
        showStatus("Door OPEN", currentFloor);
        break;

      case DIR_RIGHT:
        showStatus("Door CLOSE", currentFloor);
        break;

      default:
        break;
    }

    delay(1000); // avoid false triggers
  }
}

void showStatus(String message, int floor) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Gesture Detected:");
  display.println(message);
  display.print("Floor: ");
  display.println(floor);
  display.display();

  Serial.println(message);
}
