//
// Created by tvannier1 on 13/03/2024.
//
#include "EIT_Interface_Display/EIT_Interface_Display.h"
#include "EIT_Impedance_measurement/EIT_Impedance_measurement.h"

void drawPearLogo() {
    M5.Lcd.fillCircle(160, 100, 25, TFT_WHITE);
    M5.Lcd.fillCircle(160, 140, 40, TFT_WHITE);
    M5.Lcd.fillRect(155, 50, 6, 15, TFT_WHITE);
    M5.Lcd.fillCircle(200, 120, 25, TFT_BLACK);
}
void drawEcgLoadingBar() {
    int amplitude1 = 40; // Amplitude of the first ECG wave component
    int frequency1 = 1; // Frequency of the first ECG wave component
    int phase1 = 0; // Phase offset of the first ECG wave component

    int amplitude2 = 25; // Amplitude of the second ECG wave component
    int frequency2 = 3; // Frequency of the second ECG wave component
    int phase2 = PI/2; // Phase offset of the second ECG wave component

    int amplitude3 = 15; // Amplitude of the third ECG wave component
    int frequency3 = 5; // Frequency of the third ECG wave component
    int phase3 = PI; // Phase offset of the third ECG wave component

    int yOffset = 120; // Vertical offset of the ECG wave
    int trailLength = 120; // Length of the trail behind the head point

    // Draw the ECG wave
    for (int i = 0; i < 320; i++) {
        int y = amplitude1 * sin(2 * PI * frequency1 * i / 320 + phase1) + amplitude2 * sin(2 * PI * frequency2 * i / 320 + phase2) + amplitude3 * sin(2 * PI * frequency3 * i / 320 + phase3) + yOffset;
        // Draw the pixel at the position i and y
        M5.Lcd.drawPixel(i, y, TFT_GREEN);

        // Erase the pixel a certain distance behind the head point
        if (i >= trailLength) {
            int yOld = amplitude1 * sin(2 * PI * frequency1 * (i - trailLength) / 320 + phase1) + amplitude2 * sin(2 * PI * frequency2 * (i - trailLength) / 320 + phase2) + amplitude3 * sin(2 * PI * frequency3 * (i - trailLength) / 320 + phase3) + yOffset;
            M5.Lcd.drawPixel(i - trailLength, yOld, TFT_BLACK);
        }

        delay(3);
    }

    // erase the ECG wave
    for (int j = 0; j < 320; j++) {
        int y = amplitude1 * sin(2 * PI * frequency1 * j / 320 + phase1) + amplitude2 * sin(2 * PI * frequency2 * j / 320 + phase2) + amplitude3 * sin(2 * PI * frequency3 * j / 320 + phase3) + yOffset;
        M5.Lcd.drawPixel(j, y, TFT_BLACK);
    }
}

void drawFilesIcon(int x, int y) {
    M5.Lcd.drawRoundRect(baseX - iconSpacing - 30, baseY - 30, 60, 60, 15, TFT_WHITE);
    M5.Lcd.fillRoundRect(x - 20, y - 20, 40, 40, 5, TFT_BLUE);
    M5.Lcd.fillRect(x - 10, y - 15, 5, 10, TFT_WHITE);
    M5.Lcd.fillRect(x, y - 15, 5, 10, TFT_WHITE);
    M5.Lcd.fillRect(x + 10, y - 15, 5, 10, TFT_WHITE);
    M5.Lcd.setCursor(baseX - iconSpacing - 18, baseY + 35);
    M5.Lcd.print("Files");

}
void drawConfigIcon(int x, int y, int radius, int numTeeth, int toothSize) {

    M5.Lcd.fillCircle(x, y, radius, TFT_WHITE);
    M5.Lcd.drawRoundRect(baseX + iconSpacing - 30, baseY - 30, 60, 60, 15, TFT_WHITE);
    float angleStep = 2 * PI / numTeeth;
    for (int i = 0; i < numTeeth; ++i) {
        float angle = i * angleStep;
        int toothX = x + (radius + toothSize/4) * cos(angle);
        int toothY = y + (radius + toothSize/4) * sin(angle);
        M5.Lcd.fillRect(toothX - toothSize / 2, toothY - toothSize / 2, toothSize, toothSize, TFT_WHITE);
        M5.Lcd.fillCircle(x, y, 10, TFT_BLACK);
    }
    M5.Lcd.setCursor(baseX + iconSpacing - 18, baseY + 35);
    M5.Lcd.print("Config");
}

void drawRecordIcon(int x, int y) {

    M5.Lcd.drawRoundRect(baseX - 30, baseY - 30, 60, 60, 15, TFT_WHITE);
    M5.Lcd.fillCircle(baseX, baseY, 20, TFT_RED);
    M5.Lcd.setCursor(baseX - 18, baseY + 35);
    M5.Lcd.print("Record");
}

void drawHomeScreen() {
    M5.Lcd.clearDisplay();
    drawRecordIcon(baseX, baseY);
    drawFilesIcon(baseX - iconSpacing, baseY);
    drawConfigIcon(baseX + iconSpacing, baseY, 20, 8, 10);

    M5.Lcd.setCursor(0, 0);
}

void drawRecordScreen(){
    M5.Lcd.clearDisplay();

    //Manual record button
    M5.Lcd.drawRoundRect(baseX - 30, baseY + 90, 60, 60, 15, TFT_WHITE);
    M5.Lcd.fillCircle(baseX, baseY + 120, 20, TFT_RED);
    M5.Lcd.setCursor(baseX -45 , baseY + 155);
    M5.Lcd.print(" Manual Record");

    //Record cycle button
    M5.Lcd.drawRoundRect(baseX - 30 - iconSpacing, baseY + 90, 60, 60, 15, TFT_WHITE);
    M5.Lcd.drawCircle(60, 170, 20, TFT_RED);
    M5.Lcd.fillTriangle(75, 175, 85, 175, 80, 165, TFT_RED);
    M5.Lcd.fillTriangle(35, 165, 45, 165, 40, 175, TFT_RED);
    M5.Lcd.setCursor(25, 205);
    M5.Lcd.print("Record cycle");

    //Record list button
    M5.Lcd.drawRoundRect(baseX + iconSpacing - 30, baseY +90, 60, 60, 15, TFT_WHITE);
    M5.Lcd.fillRect(245, 150, 30, 40, TFT_WHITE);
    M5.Lcd.fillRect(250, 160, 20, 5, TFT_BLACK);
    M5.Lcd.fillRect(250, 170, 20, 5, TFT_BLACK);
    M5.Lcd.fillRect(250, 180, 20, 5, TFT_BLACK);
    M5.Lcd.setCursor(225, 205);
    M5.Lcd.print("Record list");

    M5.Lcd.setCursor(0, 0);
}

void drawFileScreen(){
    M5.Lcd.clearDisplay();
    M5.Lcd.setCursor(0, 0);
}

void gestureRepetitionDisplay(int _gesture_repetition){
    M5.Lcd.drawRoundRect(5, 0, 150, 45, 15, TFT_GREEN);
    M5.Lcd.setCursor(40, 15);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("Gesture");
    M5.Lcd.drawRoundRect(165, 0, 150, 45, 15, TFT_WHITE);
    M5.Lcd.setCursor(207, 15);
    M5.Lcd.print("Sample");
    M5.Lcd.fillRect(145, 80, 50, 60, TFT_BLACK);
    M5.Lcd.setCursor(145, 95);
    M5.Lcd.setTextSize(6);
    M5.Lcd.print(_gesture_repetition);
    M5.Lcd.setTextSize(1);
}
void sampleRepetitionDisplay(int _sample_repetition){
    M5.Lcd.drawRoundRect(165, 0, 150, 45, 15, TFT_GREEN);
    M5.Lcd.setCursor(207, 15);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("Sample");
    M5.Lcd.drawRoundRect(5, 0, 150, 45, 15, TFT_WHITE);
    M5.Lcd.setCursor(40, 15);
    M5.Lcd.print("Gesture");
    M5.Lcd.fillRect(145, 80, 50, 60, TFT_BLACK);
    M5.Lcd.setCursor(145, 95);
    M5.Lcd.setTextSize(6);
    M5.Lcd.print(_sample_repetition);
    M5.Lcd.setTextSize(1);
}
void drawRandomCycleScreen(int _gesture_repetition, int _sample_repetition, boolean _isGesture){
    M5.Lcd.clearDisplay();

    M5.Lcd.setCursor(108, 63);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("Repetition");
    M5.Lcd.setTextSize(1);

    //Start record button
    M5.Lcd.drawRoundRect(baseX - 30, baseY + 115, 60, 60, 15, TFT_WHITE);
    M5.Lcd.fillCircle(baseX, baseY + 145, 20, TFT_RED);
    M5.Lcd.setCursor(baseX - 15, baseY + 180);
    M5.Lcd.print("Start");

    if (_isGesture)
        gestureRepetitionDisplay(_gesture_repetition);
    else
        sampleRepetitionDisplay(_sample_repetition);

    M5.Lcd.fillTriangle(110, 140, 75, 115, 110 ,90, TFT_WHITE);
    M5.Lcd.fillTriangle(210, 140, 245, 115, 210 ,90, TFT_WHITE);
}

void doGestureDisplay(const String& gesture){
    M5.Lcd.clearDisplay();
    M5.Lcd.setTextSize(4);
    M5.Lcd.setCursor(110, 100);
    if (gesture == "INDEX_PINCH"){
        M5.Lcd.setCursor(25, 100);
        M5.Lcd.print("INDEX PINCH");
    }
    else if (gesture == "STRETCH"){
        M5.Lcd.setCursor(75, 100);
        M5.Lcd.print("STRETCH");
    }
    else if (gesture == "THUMB_UP"){
        M5.Lcd.setCursor(60, 100);
        M5.Lcd.print("THUMB UP");
    }else{
        M5.Lcd.print(gesture);
    }
    M5.Lcd.setTextSize(1);
}
void countDownDisplay(int countDown){
    for (int i = countDown; i > 0; i--) {
        M5.Lcd.fillRect(140, 200, 40, 40, TFT_BLACK);
        M5.Lcd.setCursor(150, 200);
        M5.Lcd.setTextSize(4);
        M5.Lcd.print(i);
        delay(1000);
    }
}

void drawGestureListScreen(const String& gesture){
    M5.Lcd.clearDisplay();

    if (gesture != gestureList[2])
        M5.Lcd.drawRoundRect(50, 85, 220, 60, 15, TFT_WHITE);
    else
        M5.Lcd.drawRoundRect(20, 85, 280, 60, 15, TFT_WHITE);

    //draw a triangle up the round rect in the middle x and  pointing to the top
    M5.Lcd.fillTriangle(160, 20, 125, 60, 195, 60, TFT_WHITE);
    //draw a triangle down the round rect in the middle x and  pointing to the bottom
    M5.Lcd.fillTriangle(160, 210, 125, 170, 195, 170, TFT_WHITE);

    if(gesture == gestureList[0]){
        M5.Lcd.setTextSize(4);
        M5.Lcd.setCursor(90, 100);
        M5.Lcd.print(gesture);
    }
    else if (gesture == gestureList[1] || gesture == gestureList[3] || gesture == gestureList[4] || gesture == gestureList[5]){
        M5.Lcd.setTextSize(4);
        M5.Lcd.setCursor(115, 100);
        M5.Lcd.print(gesture);
    }
    else if (gesture == gestureList[2]){
        M5.Lcd.setTextSize(4);
        M5.Lcd.setCursor(30, 100);
        M5.Lcd.print(gesture);
    }
    else if (gesture == gestureList[6] || gesture == gestureList[7]){
        M5.Lcd.setTextSize(4);
        M5.Lcd.setCursor(70, 100);
        M5.Lcd.print(gesture);
    }

    M5.Lcd.setTextSize(1);

    M5.Lcd.setCursor(0, 0);
}
void interfaceGestion(){
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
                if(!getLocalTime(&timeInfo)){
                    Serial.println("Failed to obtain time");
                    M5.Lcd.println("Failed to obtain time");
                    return;
                }
                Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");
                M5.Lcd.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");

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
