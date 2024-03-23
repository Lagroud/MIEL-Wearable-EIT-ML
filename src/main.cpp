#include <Arduino.h>
#include "EIT_Impedance_measurement/EIT_Impedance_measurement.h"
#include "EIT_Interface_Display/EIT_Interface_Display.h"
#include "EIT_Shared_Values.h"

int page = 0; // Variable to store the current page 0: Home, 1: Record, 2: Configuration, 3: Files
int gesture_repetition = 1; // Variable to store the number of gesture repetitions
int sample_repetition = 1; // Variable to store the number of sample repetitions
int gesture_index = 0; // Variable to store the current gesture
boolean isGesture = true; // Variable to store the type of repetition (gesture or sample)
String command = ""; // Variable to store the command received over serial communication
static m5::touch_state_t prev_state; // Variable to store the previous touch state

// Wi-Fi information
const char* ssid = "";
const char* password = "";

AD5933 ad5933; // Create an instance of the AD5933 class
MCP23008 MCP(0x20); // Create an instance of the MCP23008 class

double gain[NUM_INCR + 1]; // Array to store the gain values
int phase[NUM_INCR + 1]; // Array to store the phase values


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
    // Set time and date on the RTC8563 module (optional)
//    m5::RTC8563_Class rtc;
//    rtc.begin();
//    rtc.setTime({ 15, 56, 0 });
//    rtc.setDate({ 2024, 3, 22, 5 });

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
    M5.update();

    auto touch = M5.Touch.getDetail();

    // Check if the touch state has changed since the last iteration
    if (prev_state != touch.state) {
        prev_state = touch.state;
        static constexpr const char *state_name[16] = {
                "none", "touch", "touch_end", "touch_begin",
                "___", "hold", "hold_end", "hold_begin",
                "___", "flick", "flick_end", "flick_begin",
                "___", "drag", "drag_end", "drag_begin"};
        M5_LOGI("%s", state_name[touch.state]);

        // Check if the user flicked the screen to go back to the home screen
        if (touch.state == m5::flick_begin && (page == 1 || page == 2 || page == 3)) {
            M5.Lcd.clearDisplay();
            page = 0;
            drawHomeScreen();
        }
        // Check if the user flicked the screen to go back to the record screen
        else if (touch.state == m5::flick_begin && (page == 4 || page == 5)) {
            M5.Lcd.clearDisplay();
            page = 2;
            drawRecordScreen();
        }
    }
    M5.Lcd.setCursor(0, 0);

    if (page == 0) {
        if(touch.wasClicked()){
            // Check if the user clicked on the file icon
            if (touch.x > 30 && touch.x < 90 && touch.y > 20 && touch.y < 80) {
                page = 1;
                drawFileScreen();

                // Check if an SD card is attached to the M5CoreS3 and print the card type and size
                uint8_t cardType = SD.cardType();

                if (cardType == CARD_NONE) {
                    Serial.println("No SD card attached");
                    M5.Lcd.println("No SD card attached");
                    return;
                }
                Serial.print("SD Card Type: ");
                M5.Lcd.print("SD Card Type: ");
                if (cardType == CARD_MMC) {
                    Serial.println("MMC");
                    M5.Lcd.println("MMC");
                } else if (cardType == CARD_SD) {
                    Serial.println("SDSC");
                    M5.Lcd.println("SDSC");
                } else if (cardType == CARD_SDHC) {
                    Serial.println("SDHC");
                    M5.Lcd.println("SDHC");
                } else {
                    Serial.println("UNKNOWN");
                    M5.Lcd.println("UNKNOWN");
                }

                uint64_t cardSize = SD.cardSize() / (1024 * 1024);
                Serial.printf("SD Card Size: %lluMB\n", cardSize);
                Serial.println();
                M5.Lcd.printf("SD Card Size: %lluMB\n", cardSize);

                // List all files in the root directory
                File root = SD.open("/");
                if (!root) {
                    Serial.println("Failed to open directory");
                    M5.Lcd.println("Failed to open directory");
                    return;
                }
                Serial.println("Files found in the root directory:");
                M5.Lcd.println("Files found in the root directory:");
                while (true) {
                    File entry = root.openNextFile();
                    if (!entry) {
                        break;
                    }
                    Serial.println(entry.name());
                    M5.Lcd.println(entry.name());
                    entry.close();
                }
                root.close();
            }
            // Check if the user clicked on the record icon
            if (touch.x > 130 && touch.x < 190 && touch.y > 20 && touch.y < 80) {
                page = 2;
                drawRecordScreen();
            }
            // Check if the user clicked on the configuration icon
            if (touch.x > 230 && touch.x < 290 && touch.y > 20 && touch.y < 80) {
                page = 3;
                M5.Lcd.clearDisplay();
                Serial.println();
                M5.Lcd.setCursor(0, 0);
                M5.Lcd.setTextSize(1);

                if (MCP.isConnected()) {
                    Serial.println("MCP23008 is connected");
                    M5.Lcd.println("MCP23008 is connected");
                    // Set all the GP0-GP7 pins values to LOW
                    for (int i = 0; i < 8; i++) {
                        MCP.pinMode(i, OUTPUT); // Set the GP0-GP7 pins as outputs
                        MCP.digitalWrite(i, LOW); // Set the state of the first 8 digital pins of the MCP23008 to LOW
                    }
                } else {
                    Serial.println("MCP23008 is not connected");
                    M5.Lcd.setTextColor(TFT_RED);
                    M5.Lcd.println("MCP23008 is not connected");
                    M5.Lcd.setTextColor(TFT_WHITE);
                }
                if (!(AD5933::reset() &&
                      AD5933::setInternalClock(true) &&
                      AD5933::setStartFrequency(START_FREQ) &&
                      AD5933::setIncrementFrequency(FREQ_INCR) &&
                      AD5933::setNumberIncrements(NUM_INCR) &&
                      AD5933::setPGAGain(PGA_GAIN_X1))) {
                    Serial.println("AD5933 initialization failed");
                    M5.Lcd.setTextColor(TFT_RED);
                    M5.Lcd.println("AD5933 initialization failed");
                    M5.Lcd.setTextColor(TFT_WHITE);
                }
                // Set the settling cycles to 1 for faster measurements (optional)
                if (!ad5933.setSettlingCycles(1)) {
                    Serial.println("AD5933 settling cycles failed");
                }

                // Perform calibration sweep
                if (AD5933::calibrate(gain, phase, REF_RESIST, NUM_INCR + 1)) {
                    Serial.println("AD5933 calibrated");
                    M5.Lcd.println("AD5933 calibrated");
                } else {
                    Serial.println("AD5933 calibration failed");
                    M5.Lcd.setTextColor(TFT_RED);
                    M5.Lcd.println("AD5933 calibration failed");
                    M5.Lcd.setTextColor(TFT_WHITE);
                }
                //check serial communication
                if (Serial) {
                    M5.Lcd.setTextColor(TFT_GREEN);
                    M5.Lcd.println("Serial communication established");
                    M5.Lcd.setTextColor(TFT_WHITE);
                } else {
                    M5.Lcd.setTextColor(TFT_RED);
                    M5.Lcd.println("Serial communication failed");
                    M5.Lcd.setTextColor(TFT_WHITE);
                }
                if (WiFiClass::status() == WL_CONNECTED) {
                    M5.Lcd.println("Connected to the WiFi network");
                } else {
                    M5.Lcd.println("Failed to connect to the WiFi network");
                }
                //print the date and time
                if(!getLocalTime(&timeinfo)){
                    Serial.println("Failed to obtain time");
                    M5.Lcd.println("Failed to obtain time");
                    return;
                }
                Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
                M5.Lcd.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

            }
        }

    }
    // On the record screen
    if (page == 2) {
        // Check if the user clicked on the Manual Record icon
        if (touch.wasClicked()){if (touch.x > 130 && touch.x < 190 && touch.y > 140 && touch.y < 200) {
                M5.Lcd.fillRect(0, 0, 320, 50, TFT_BLACK);
                unsigned long start = millis();
                initTabImpedance();
                unsigned long start1 = millis();
                computeTabImpedance(0x20, ad5933, gain);
                unsigned long end1 = millis();
                Serial.println();
                printTabImpedance();
                Serial.println();
                Serial.println(gestureList[gesture_index]);
                if (Serial) {
                    sendTabImpedance();
                    M5.Lcd.setTextColor(TFT_BLUE);
                    M5.Lcd.println("Data sent for CSV conversion");
                    M5.Lcd.setTextColor(TFT_WHITE);
                    Serial.println();
                } else {
                    M5.Lcd.setTextColor(TFT_RED);
                    M5.Lcd.println("Serial communication failed");
                    M5.Lcd.setTextColor(TFT_WHITE);
                }
                // if there is a sd card
                if (SD.cardType() != CARD_NONE) {
                    fileCreation(gestureList[gesture_index]);
                } else {
                    M5.Lcd.setTextColor(TFT_RED);
                    M5.Lcd.println("No SD card attached");
                    M5.Lcd.setTextColor(TFT_WHITE);
                }
                unsigned long end = millis();
                Serial.println("Time elapsed: " + String(end - start) + " ms");
                Serial.println("Time elapsed for computeTabImpedance: " + String(end1 - start1) + " ms");
            }
        }
        // Check if the user clicked on the Cycle icon
        if(touch.wasClicked() && touch.x > 30 && touch.x < 90 && touch.y > 140 && touch.y < 200){
            page = 4;
            drawRandomCycleScreen(gesture_repetition, sample_repetition, isGesture);
        }
        // Check if the user clicked on the gesture list icon
        if(touch.wasClicked() && touch.x > 230 && touch.x < 290 && touch.y > 140 && touch.y < 200){
            M5.Lcd.clearDisplay();
            page = 5;
            drawGestureListScreen(gestureList[gesture_index]);
        }
    }
    // On the cycle screen
    if (page == 4) {
        // Check if the user clicked on the gesture repetition icon
        if (touch.wasClicked() && touch.x > 5 && touch.x < 155 && touch.y > 0 && touch.y < 45) {
            isGesture = true;
            gestureRepetitionDisplay(gesture_repetition);
        }
        // Check if the user clicked on the sample repetition icon
        if (touch.wasClicked() && touch.x > 165 && touch.x < 315 && touch.y > 0 && touch.y < 45) {
            isGesture = false;
            sampleRepetitionDisplay(sample_repetition);
        }
        // Check if the user clicked on the plus icon for gesture repetition
        if(touch.wasClicked() && isGesture && touch.x > 200 && touch.x < 255 && touch.y > 80 && touch.y < 150){
            gesture_repetition++;
            if (gesture_repetition > 9) {
                gesture_repetition = 9;
            }
            Serial.println(gesture_repetition);
            gestureRepetitionDisplay(gesture_repetition);
        }
        // Check if the user clicked on the minus icon for gesture repetition
        if(touch.wasClicked() && isGesture && touch.x > 65 && touch.x < 120 && touch.y > 80 && touch.y < 150){
            gesture_repetition--;
            if (gesture_repetition < 1) {
                gesture_repetition = 1;
            }
            Serial.println(gesture_repetition);
            gestureRepetitionDisplay(gesture_repetition);
        }

        // Check if the user clicked on the plus icon for sample repetition
        if(touch.wasClicked() && !isGesture && touch.x > 200 && touch.x < 255 && touch.y > 70 && touch.y < 150){
            sample_repetition++;
            if (sample_repetition > 10) {
                sample_repetition = 10;
            }
            Serial.println(gesture_repetition);
            sampleRepetitionDisplay(sample_repetition);
        }
        // Check if the user clicked on the minus icon for sample repetition
        if(touch.wasClicked() && !isGesture && touch.x > 65 && touch.x < 120 && touch.y > 70 && touch.y < 150){
            sample_repetition--;
            if (sample_repetition < 1) {
                sample_repetition = 1;
            }
            Serial.println(sample_repetition);
            sampleRepetitionDisplay(sample_repetition);
        }

        // Check if the user clicked on the start record icon
        if(touch.wasClicked() && touch.x > 130 && touch.x < 190 && touch.y > 140 && touch.y < 200){
            M5.Lcd.clearDisplay();
            M5.Lcd.setCursor(0, 0);
            impedanceRandomCycle(gesture_repetition, sample_repetition, MCP, ad5933, gain);
            drawRandomCycleScreen(gesture_repetition, sample_repetition, isGesture);
        }

    }
    // On the gesture list screen
    if (page == 5) {
        // Check if the user clicked on the next gesture icon
        if(touch.wasClicked() && touch.x > 125 && touch.x < 195 && touch.y > 20 && touch.y < 60){
            gesture_index++;
            if (gesture_index > sizeof (gestureList) / sizeof (gestureList[0]) - 1) {
                gesture_index = 7;
            }
            M5.Lcd.fillRect(0, 82, 320, 65, TFT_BLACK);
            drawGestureListScreen(gestureList[gesture_index]);
        }
        // Check if the user clicked on the previous gesture icon
        else if (touch.wasClicked() && touch.x > 125 && touch.x < 195 && touch.y > 170 && touch.y < 210){
            gesture_index--;
            if (gesture_index < 0) {
                gesture_index = 0;
            }
            M5.Lcd.fillRect(0, 82, 320, 65, TFT_BLACK);
            drawGestureListScreen(gestureList[gesture_index]);
        }
    }
}
