#include <Arduino.h>
#include "EIT_Impedance_measurement/EIT_Impedance_measurement.h"
#include "EIT_Interface_Display/EIT_Interface_Display.h"
#include "EIT_Shared_Values.h"

AD5933 ad5933;
MCP23008 MCP(0x20);
int page;
int gesture_repetition = 1;
int sample_repetition = 1;
int gesture_index;
boolean isGesture;
m5::touch_state_t prev_state;
double gain[NUM_INCR + 1]; // Array to store the gain values
int phase[NUM_INCR + 1];
struct tm timeInfo;
const char* ssid = "";
const char* password = "";

void setup() {

    auto cfg = m5::M5Unified::config();
    // Initialize M5CoreS3
    M5.begin(cfg);

    drawPearLogo();
    delay(1000);
    M5.Lcd.clearDisplay();

    // Connect to Wi-Fi network with SSID and password (optional)
//    WiFiClass::begin(ssid, password);
//    if (WiFiClass::status() == WL_CONNECTED) {
//        Serial.println("Connected to the WiFi network");
//    } else {
//        Serial.println("Failed to connect to the Wi-Fi network");
//
//    }

    // Set time and date on the RTC8563 module
    m5::RTC8563_Class rtc;
    rtc.begin();
//    rtc.setTime({ 14, 21, 0 });
//    rtc.setDate({ 2024, 3, 27, 3 });

    //Initialize communication with the MCP23008
    MCP.begin();
    if (MCP.isConnected()) {
        Serial.println("MCP23008 is connected");
        // Set all the GP0-GP7 pins values to LOW to avoid any unwanted signal
        for (int i = 0; i < 8; i++) {
            MCP.pinMode(i, OUTPUT); // Set the GP0-GP7 pins as outputs
            MCP.digitalWrite(i, LOW); // Set the state of the first 8 digital pins of the MCP23008 to LOW
        }
    } else {
        Serial.println("MCP23008 is not connected");
    }

    // Initialize the AD5933
    if (!(AD5933::reset() &&
    AD5933::setInternalClock(true) &&
    AD5933::setStartFrequency(START_FREQ) &&
    AD5933::setIncrementFrequency(FREQ_INCR) &&
    AD5933::setNumberIncrements(NUM_INCR) &&
    AD5933::setPGAGain(PGA_GAIN_X1)))
    {
        Serial.println("AD5933 initialization failed");
    }
    // Set the settling cycles to 1 for faster measurements (optional)
    if(!ad5933.setSettlingCycles(1)){
        Serial.println("AD5933 settling cycles failed");
    }
    // Perform calibration sweep
    if (AD5933::calibrate(gain, phase, REF_RESIST, NUM_INCR+1)) {
        Serial.println("AD5933 calibrated");
    }
    else {
        Serial.println("AD5933 calibration failed");
    }

    // Initialize the SD card and SPI communication
    SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
    if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
        Serial.println("Card failed, or not present");
        Serial.println();
    }

    // Initialize the tabImpedance matrix with zeros
    initTabImpedance();
    printTabImpedance();

    drawHomeScreen();
}

void loop() {
    // Update the M5CoreS3 to get the touch state
    interfaceGestion();
}
