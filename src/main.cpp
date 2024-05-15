#include <Arduino.h>
#include "EIT_Impedance_measurement/EIT_Impedance_measurement.h"
#include "EIT_Interface_Display/EIT_Interface_Display.h"
#include "EIT_Shared_Values.h"

// Declare and initialize the AD5933 and MCP23008 instances
AD5933 ad5933;
MCP23008 MCP(0x20);

// Declare and initialize the variables for the page, gesture repetition, sample repetition, gesture index, and isGesture
int page;
int gesture_repetition = 1;
int sample_repetition = 1;
int gesture_index;
int height_index = 0;
int rotation_index = 0;
boolean isGesture;

// Declare and initialize the variable for the previous touch state
m5::touch_state_t prev_state;

// Declare and initialize the arrays for the gain and phase values
double gain[NUM_INCR + 1]; // Array to store the gain values
int phase[NUM_INCR + 1]; // Array to store the phase values

// Declare and initialize the variable for the time information
struct tm timeInfo;

// Declare and initialize the RTC8563 instance
m5::RTC8563_Class rtc;

// Declare and initialize the variables for the SSID and password
const char* ssid = "SIMPLYY";
const char* password = "Tq6526-5";
 
// The setup function runs once when you press reset or power the board
void setup() {

    auto cfg = m5::M5Unified::config();
    // Initialize M5CoreS3
    M5.begin(cfg);

    drawPearLogo();
    delay(1000);
    M5.Lcd.clearDisplay();

    // Connect to Wi-Fi network with SSID and password (optional)
    WiFi.begin(ssid, password);
    switch (WiFiClass::status())
    {
    case WL_CONNECTED:
        Serial.println("Connected to the WiFi network");
        break;
    case WL_NO_SSID_AVAIL:
        Serial.println("No SSID available");
        break;
    case WL_CONNECT_FAILED:
        Serial.println("Connection failed");
        break;
    case WL_IDLE_STATUS:
        Serial.println("Idle status");
        break;
    case WL_DISCONNECTED:
        Serial.println("Disconnected");
        break;
    default:
        break;
    }

    // Set time and date on the RTC8563 module
    rtc.begin();
//    rtc.setTime({ 14, 21, 0 });
//    rtc.setDate({ 2024, 3, 27, 3 });
    auto month = rtc.getDate().month;
    auto day = rtc.getDate().date;
    auto year = rtc.getDate().year;
    auto hour = rtc.getTime().hours;
    auto minute = rtc.getTime().minutes;


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

    drawHomeScreen(month, day, year, hour, minute);
}

void loop() {
    // Update the M5CoreS3 to get the touch state
    interfaceGestion();
}