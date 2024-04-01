//
// Created by tvannier1 on 13/03/2024.
//

// Include necessary libraries and header files
#include "EIT_Interface_Display/EIT_Interface_Display.h"
#include "EIT_Impedance_measurement/EIT_Impedance_measurement.h"
#include "EIT_Shared_Values.h"

/**
 * @brief Draws the Pear logo on the M5 LCD screen.
 */
void drawPearLogo() {
    M5.Lcd.fillCircle(160, 100, 25, TFT_WHITE);
    M5.Lcd.fillCircle(160, 140, 40, TFT_WHITE);
    M5.Lcd.fillRect(155, 50, 6, 15, TFT_WHITE);
    M5.Lcd.fillCircle(200, 120, 25, TFT_BLACK);
}

/**
 * @brief Draws an ECG loading bar on the M5 LCD screen.
 */
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

/**
 * @brief Draws the Files icon on the M5 LCD screen.
 * @param x The x-coordinate of the center of the icon.
 * @param y The y-coordinate of the center of the icon.
 */
void drawFilesIcon(int x, int y) {
    M5.Lcd.drawRoundRect(baseX - iconSpacing - 30, baseY - 30, 60, 60, 15, TFT_WHITE);
    M5.Lcd.fillRoundRect(x - 20, y - 20, 40, 40, 5, TFT_BLUE);
    M5.Lcd.fillRect(x - 10, y - 15, 5, 10, TFT_WHITE);
    M5.Lcd.fillRect(x, y - 15, 5, 10, TFT_WHITE);
    M5.Lcd.fillRect(x + 10, y - 15, 5, 10, TFT_WHITE);
    M5.Lcd.setCursor(baseX - iconSpacing - 18, baseY + 35);
    M5.Lcd.print("Files");
}

/**
 * @brief Draws the Config icon on the M5 LCD screen.
 * @param x The x-coordinate of the center of the icon.
 * @param y The y-coordinate of the center of the icon.
 * @param radius The radius of the icon.
 * @param numTeeth The number of teeth on the gear of the icon.
 * @param toothSize The size of each tooth on the gear of the icon.
 */
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

/**
 * @brief Draws the Record icon on the M5 LCD screen.
 * @param x The x-coordinate of the center of the icon.
 * @param y The y-coordinate of the center of the icon.
 */
void drawRecordIcon(int x, int y) {
    M5.Lcd.drawRoundRect(baseX - 30, baseY - 30, 60, 60, 15, TFT_WHITE);
    M5.Lcd.fillCircle(baseX, baseY, 20, TFT_RED);
    M5.Lcd.setCursor(baseX - 18, baseY + 35);
    M5.Lcd.print("Record");
}

/**
 * @brief Clears the display and draws the home screen.
 *
 * The home screen consists of three icons: Record, Files, and Config.
 * These icons are drawn by calling their respective functions.
 */
void drawHomeScreen() {
    M5.Lcd.clearDisplay();
    drawRecordIcon(baseX, baseY);
    drawFilesIcon(baseX - iconSpacing, baseY);
    drawConfigIcon(baseX + iconSpacing, baseY, 20, 8, 10);

    M5.Lcd.setCursor(0, 0);
}

/**
 * @brief Clears the display and draws the record screen.
 *
 * The record screen consists of three buttons: Manual Record, Record Cycle, and Record List.
 * Each button is drawn with a specific shape and label.
 */
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

/**
 * @brief Clears the display and draws the file screen.
 *
 * The file screen displays information about the SD card if it is attached.
 * It prints the card type, size, and lists all files in the root directory.
 */
void drawFileScreen(){
    M5.Lcd.clearDisplay();
    M5.Lcd.setCursor(0, 0);
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

/**
 * @brief Draws the gesture repetition display.
 *
 * The display consists of two round rectangles labeled "Gesture" and "Sample".
 * The number of gesture repetitions is displayed in a large font size.
 *
 * @param _gesture_repetition The number of gesture repetitions.
 */
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

/**
 * @brief Displays the sample repetition on the screen.
 *
 * This function draws a rectangle on the screen and displays the number of sample repetitions.
 * It also labels the display with "Sample" and "Gesture".
 *
 * @param _sample_repetition The number of sample repetitions.
 */
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

/**
 * @brief Draws the random cycle screen.
 *
 * This function clears the display and prints "Repetition". It also draws the start record button.
 * Depending on the value of _isGesture, it displays either the gesture repetition or the sample repetition.
 *
 * @param _gesture_repetition The number of gesture repetitions.
 * @param _sample_repetition The number of sample repetitions.
 * @param _isGesture A boolean value indicating whether the gesture repetition is to be displayed.
 */
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

/**
 * @brief Displays the given gesture on the screen.
 *
 * This function clears the display and prints the given gesture in a large font size.
 * The position of the text depends on the length of the gesture name.
 *
 * @param gesture The gesture to be displayed.
 */
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

/**
 * @brief Displays a countdown on the screen.
 *
 * This function displays a countdown from the given number to 0, with a delay of 1 second between each number.
 * The countdown is displayed in a large font size.
 *
 * @param countDown The starting number of the countdown.
 */
void countDownDisplay(int countDown){
    for (int i = countDown; i > 0; i--) {
        M5.Lcd.fillRect(140, 200, 40, 40, TFT_BLACK);
        M5.Lcd.setCursor(150, 200);
        M5.Lcd.setTextSize(4);
        M5.Lcd.print(i);
        delay(1000);
    }
}
/**
 * @brief Draws the gesture list screen.
 *
 * This function clears the display and draws a list of gestures on the screen.
 * The list is displayed in a round rectangle, with triangles pointing up and down to indicate scrolling.
 * The position and size of the text depend on the length of the gesture name.
 *
 * @param gesture The gesture to be displayed.
 */
void drawGestureListScreen(const String& gesture){
    // Clear the display
    M5.Lcd.clearDisplay();

    // Draw a round rectangle for the gesture list
    // The size and position of the rectangle depend on the length of the gesture name
    if (gesture != gestureList[2])
        M5.Lcd.drawRoundRect(50, 85, 220, 60, 15, TFT_WHITE);
    else
        M5.Lcd.drawRoundRect(20, 85, 280, 60, 15, TFT_WHITE);

    // Draw a triangle pointing up at the top of the round rectangle
    M5.Lcd.fillTriangle(160, 20, 125, 60, 195, 60, TFT_WHITE);
    // Draw a triangle pointing down at the bottom of the round rectangle
    M5.Lcd.fillTriangle(160, 210, 125, 170, 195, 170, TFT_WHITE);

    // Print the gesture name in the round rectangle
    // The position and size of the text depend on the length of the gesture name
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

    // Reset the text size
    M5.Lcd.setTextSize(1);

    // Reset the cursor position
    M5.Lcd.setCursor(0, 0);
}

/**
 * @brief Draws the configuration screen.
 *
 * This function clears the display and prints the status of various components such as MCP23008, AD5933, Serial, WiFi, and Time.
 * It also prints the current date and time.
 *
 * @param month The current month.
 * @param day The current day.
 * @param year The current year.
 * @param hour The current hour.
 * @param minute The current minute.
 */
void drawConfigScreen(uint16_t month, uint16_t day, uint16_t year, uint16_t hour, uint16_t minute){
    M5.Lcd.clearDisplay();
    M5.Lcd.clearDisplay();
    Serial.println();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(1);

    // Check if MCP23008 is connected
    if (MCP.isConnected()) {
        Serial.println("MCP23008 is connected");
        M5.Lcd.drawRoundRect(5, 0, 150, 45, 15, TFT_GREEN);
        M5.Lcd.setCursor(35, 15);
        M5.Lcd.setTextSize(2);
        M5.Lcd.print("MCP23008");
        // Set all the GP0-GP7 pins values to LOW
        for (int i = 0; i < 8; i++) {
            MCP.pinMode(i, OUTPUT); // Set the GP0-GP7 pins as outputs
            MCP.digitalWrite(i, LOW); // Set the state of the first 8 digital pins of the MCP23008 to LOW
        }
    } else {
        Serial.println("MCP23008 is not connected");
        M5.Lcd.drawRoundRect(5, 0, 150, 45, 15, TFT_RED);
        M5.Lcd.setCursor(35, 15);
        M5.Lcd.setTextSize(2);
        M5.Lcd.print("MCP23008");
    }

    // Initialize and calibrate AD5933
    if (!(AD5933::reset() &&
          AD5933::setInternalClock(true) &&
          AD5933::setStartFrequency(START_FREQ) &&
          AD5933::setIncrementFrequency(FREQ_INCR) &&
          AD5933::setNumberIncrements(NUM_INCR) &&
          AD5933::setPGAGain(PGA_GAIN_X1))) {
        Serial.println("AD5933 initialization failed");
    }
    // Set the settling cycles to 1 for faster measurements (optional)
    if (!ad5933.setSettlingCycles(1)) {
        Serial.println("AD5933 settling cycles failed");
    }

    // Perform calibration sweep
    if (AD5933::calibrate(gain, phase, REF_RESIST, NUM_INCR + 1)) {
        Serial.println("AD5933 calibrated");
        M5.Lcd.drawRoundRect(165, 0, 150, 45, 15, TFT_GREEN);
        M5.Lcd.setCursor(207, 15);
        M5.Lcd.print("AD5933");
    } else {
        Serial.println("AD5933 calibration failed");
        M5.Lcd.drawRoundRect(165, 0, 150, 45, 15, TFT_RED);
        M5.Lcd.setCursor(207, 15);
        M5.Lcd.print("AD5933");
    }

    // Check serial communication
    if (Serial) {
        M5.Lcd.drawRoundRect(5, 50, 150, 45, 15, TFT_GREEN);
        M5.Lcd.setCursor(45, 65);
        M5.Lcd.print("Serial");
    } else {
        M5.Lcd.drawRoundRect(5, 50, 150, 45, 15, TFT_RED);
        M5.Lcd.setCursor(45, 65);
        M5.Lcd.print("Serial");
    }

    // Check WiFi status
    if (WiFiClass::status() == WL_CONNECTED) {
        M5.Lcd.drawRoundRect(165, 50, 150, 45, 15, TFT_GREEN);
        M5.Lcd.setCursor(217, 65);
        M5.Lcd.print("WiFi");
    } else {
        M5.Lcd.drawRoundRect(165, 50, 150, 45, 15, TFT_RED);
        M5.Lcd.setCursor(217, 65);
        M5.Lcd.print("WiFi");
    }

    // Print the date and time
    if(!getLocalTime(&timeInfo)){
        Serial.println("Failed to obtain time");
        M5.Lcd.drawRoundRect(5, 100, 150, 45, 15, TFT_RED);
        M5.Lcd.setCursor(55, 115);
        M5.Lcd.print("Time");
        return;
    }
    Serial.printf("%02d %02d %04d %02d:%02d", month, day, year, hour, minute);
    M5.Lcd.drawRoundRect(5, 100, 150, 45, 15, TFT_WHITE);
    M5.Lcd.setCursor(55, 115);
    M5.Lcd.print("Time");
    M5.Lcd.setCursor(70, 175);
    M5.Lcd.printf("%02d/%02d/%04d %02d:%02d", month, day, year, hour, minute);

    M5.Lcd.setTextSize(1);
}

/**
 * @brief Draws the time screen.
 *
 * This function clears the display and prints the current date and time.
 * If the time cannot be obtained, it prints an error message.
 * Otherwise, it prints the date and time, and draws arrows for adjusting each component of the date and time.
 * It also prints a message instructing the user to press the restart button to apply changes.
 *
 * @param month The current month.
 * @param day The current day.
 * @param year The current year.
 * @param hour The current hour.
 * @param minute The current minute.
 */
void drawTimeScreen(uint16_t month, uint16_t day, uint16_t year, uint16_t hour, uint16_t minute){
    M5.Lcd.clearDisplay();
    // print the date and time
    if(!getLocalTime(&timeInfo)){
        Serial.println("Failed to obtain time");
        M5.Lcd.println("Failed to obtain time");
        return;
    }else {

        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(15, 100);
        M5.Lcd.printf("%02d   %02d   %04d  %02d : %02d", month, day, year, hour, minute);
        //Month arrows
        M5.Lcd.fillTriangle(25, 30, 5, 60, 45, 60, TFT_WHITE);
        M5.Lcd.fillTriangle(25, 180, 5, 150, 45, 150, TFT_WHITE);
        //Day arrows
        M5.Lcd.fillTriangle(90, 30, 70, 60, 110, 60, TFT_WHITE);
        M5.Lcd.fillTriangle(90, 180, 70, 150, 110, 150, TFT_WHITE);
        //Year arrows
        M5.Lcd.fillTriangle(155, 30, 135, 60, 175, 60, TFT_WHITE);
        M5.Lcd.fillTriangle(155, 180, 135, 150, 175, 150, TFT_WHITE);
        //Hour arrows
        M5.Lcd.fillTriangle(220, 30, 200, 60, 240, 60, TFT_WHITE);
        M5.Lcd.fillTriangle(220, 180, 200, 150, 240, 150, TFT_WHITE);
        //Minute arrows
        M5.Lcd.fillTriangle(285, 30, 265, 60, 305, 60, TFT_WHITE);
        M5.Lcd.fillTriangle(285, 180, 265, 150, 305, 150, TFT_WHITE);

        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(5, 220);
        M5.Lcd.print("Press the m5's restart button to apply the changes");
    }
}

/**
 * @brief Manages the interface of the application.
 *
 * This function updates the M5 stack, gets the touch details and checks if the touch state has changed since the last iteration.
 * It also manages the navigation between different screens (home, record, configuration, etc.) based on the user's touch input.
 * It handles the functionality of different screens like recording, configuration, etc.
 */
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
        } else if (touch.state == m5::flick_begin && (page == 31 || page == 32 || page == 33 || page == 34 || page == 35)) {
            M5.Lcd.clearDisplay();
            page = 3;
            auto month = rtc.getDate().month;
            auto day = rtc.getDate().date;
            auto year = rtc.getDate().year;
            auto hour = rtc.getTime().hours;
            auto minute = rtc.getTime().minutes;
            drawConfigScreen(month, day, year, hour, minute);
        }
    }
    M5.Lcd.setCursor(0, 0);

    if (page == 0) {
        if(touch.wasClicked()){
            // Check if the user clicked on the file icon
            if (touch.x > 30 && touch.x < 90 && touch.y > 20 && touch.y < 80) {
                page = 1;
                drawFileScreen();
            }
            // Check if the user clicked on the record icon
            if (touch.x > 130 && touch.x < 190 && touch.y > 20 && touch.y < 80) {
                page = 2;
                drawRecordScreen();
            }
            // Check if the user clicked on the configuration icon
            if (touch.x > 230 && touch.x < 290 && touch.y > 20 && touch.y < 80) {
                page = 3;
                auto month = rtc.getDate().month;
                auto day = rtc.getDate().date;
                auto year = rtc.getDate().year;
                auto hour = rtc.getTime().hours;
                auto minute = rtc.getTime().minutes;
                drawConfigScreen(month, day, year, hour, minute);
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

                if (Serial) {
                    Serial.println(gestureList[gesture_index]);
                    sendTabImpedance();
                    M5.Lcd.println("Data sent for CSV conversion");
                    Serial.println();
                } else {
                    Serial.println("No serial communication");
                    M5.Lcd.setTextColor(TFT_RED);
                    M5.Lcd.println("No serial communication");
                    M5.Lcd.setTextColor(TFT_WHITE);
                }
                // if there is a sd card
                if (SD.cardType() != CARD_NONE) {
                    fileCreation(gestureList[gesture_index]);
                    M5.Lcd.println("File created on the SD card");
                }
                unsigned long end = millis();
                Serial.println("Time elapsed: " + String(end - start) + " ms");
                Serial.println("Time elapsed for computeTabImpedance: " + String(end1 - start1) + " ms");
            }
        }

        if(touch.wasClicked() && touch.x > 30 && touch.x < 90 && touch.y > 140 && touch.y < 200){
            page = 4;
            drawRandomCycleScreen(gesture_repetition, sample_repetition, isGesture);
        }

        if(touch.wasClicked() && touch.x > 230 && touch.x < 290 && touch.y > 140 && touch.y < 200){
            M5.Lcd.clearDisplay();
            page = 5;
            drawGestureListScreen(gestureList[gesture_index]);
        }
    }
    if (page == 3) {
        // Check if the user clicked on the MCP23008 round rect
        if(touch.wasClicked() && touch.x > 5 && touch.x < 155 && touch.y > 0 && touch.y < 45){
            page = 31;
            M5.Lcd.fillRoundRect(0, 30, 320, 160, 15, TFT_WHITE);
            M5.Lcd.setCursor(5, 80);
            M5.Lcd.setTextSize(1);
            M5.Lcd.setTextColor(TFT_BLACK);
            if (MCP.isConnected()) {
                Serial.println("MCP23008 is connected");
                M5.Lcd.println("MCP23008 is connected and configured");
            } else {
                Serial.println("MCP23008 is not connected");
                M5.Lcd.println("MCP23008 is not connected");
                M5.Lcd.println("Please check the MCP23008 connection and restart the M5CoreS3\n");
                M5.Lcd.println("If problems persist, check MCP23008 configurations");
                M5.Lcd.println("in the setup() function of the main.cpp file");
            }
            M5.Lcd.setTextColor(TFT_WHITE);
        }
        // Check if the user clicked on the AD5933 round rect
        if(touch.wasClicked() && touch.x > 165 && touch.x < 315 && touch.y > 0 && touch.y < 45){
            page = 32;
            M5.Lcd.fillRoundRect(0, 30, 320, 160, 15, TFT_WHITE);
            M5.Lcd.setCursor(5, 60);
            M5.Lcd.setTextSize(1);
            M5.Lcd.setTextColor(TFT_BLACK);
            if (!(AD5933::reset() &&
                  AD5933::setInternalClock(true) &&
                  AD5933::setStartFrequency(START_FREQ) &&
                  AD5933::setIncrementFrequency(FREQ_INCR) &&
                  AD5933::setNumberIncrements(NUM_INCR) &&
                  AD5933::setPGAGain(PGA_GAIN_X1))) {
                Serial.println("AD5933 initialization failed");
                M5.Lcd.println("AD5933 initialization failed");
                M5.Lcd.println("Please check the AD5933 connection and restart the M5CoreS3\n");
                M5.Lcd.println("If problems persist, check AD5933 configurations");
                M5.Lcd.println("like the start frequency, increment frequency,");
                M5.Lcd.println("number of increments, PGA gain and settling cycles");
                M5.Lcd.println("in the setup() function of the main.cpp file\n");
            } else {
                M5.Lcd.println("AD5933 is connected and configured");
            }
            // Set the settling cycles to 1 for faster measurements (optional)
            if (!ad5933.setSettlingCycles(1)) {
                Serial.println("AD5933 settling cycles failed");
                M5.Lcd.println("AD5933 settling cycles failed\n");
            }else{
                M5.Lcd.println("AD5933 settling cycles set to 1\n");
            }

            // Perform calibration sweep
            if (AD5933::calibrate(gain, phase, REF_RESIST, NUM_INCR + 1)) {
                Serial.println("AD5933 calibrated");
                M5.Lcd.println("AD5933 calibrated");
            } else {
                Serial.println("AD5933 calibration failed");
                M5.Lcd.println("AD5933 calibration failed\n");
            }
            M5.Lcd.setTextColor(TFT_WHITE);
        }
        // Check if the user clicked on the Serial round rect
        if(touch.wasClicked() && touch.x > 5 && touch.x < 155 && touch.y > 50 && touch.y < 95){
            page = 33;
            M5.Lcd.fillRoundRect(0, 30, 320, 160, 15, TFT_WHITE);
            M5.Lcd.setCursor(5, 60);
            M5.Lcd.setTextSize(1);
            M5.Lcd.setTextColor(TFT_BLACK);
            if (Serial) {
                Serial.println("Serial communication established");
                M5.Lcd.println("Serial communication established");
            } else {
                Serial.println("No serial communication established");
                M5.Lcd.println("No serial communication established");
                M5.Lcd.println("Please check the serial connection and restart the M5CoreS3\n");
                M5.Lcd.println("If problems persist, check the serial COM port and baud rate");
                M5.Lcd.println("in the setup() function of the main.cpp file\n");
                M5.Lcd.println("Also check the USB cable connection and be sure to use admin terminal");
            }
            M5.Lcd.setTextColor(TFT_WHITE);
        }
        // Check if the user clicked on the Wi-Fi round rect
        if(touch.wasClicked() && touch.x > 165 && touch.x < 315 && touch.y > 70 && touch.y < 95){
            page = 34;
            M5.Lcd.fillRoundRect(0, 30, 320, 160, 15, TFT_WHITE);
            M5.Lcd.setCursor(5, 60);
            M5.Lcd.setTextSize(1);
            M5.Lcd.setTextColor(TFT_BLACK);
            if (WiFiClass::status() == WL_CONNECTED) {
                Serial.println("WiFi connected");
                M5.Lcd.println("WiFi connected");
            } else {
                Serial.println("WiFi not connected");
                M5.Lcd.println("WiFi not connected\n");
                M5.Lcd.println("Set WiFi SSID and password in the main.cpp file\n");
            }
            M5.Lcd.setTextColor(TFT_WHITE);
        }
        // Check if the user clicked on the Time round rect
        if(touch.wasClicked() && touch.x > 5 && touch.x < 155 && touch.y > 100 && touch.y < 145){
            page = 35;
            auto month = rtc.getDate().month;
            auto day = rtc.getDate().date;
            auto year = rtc.getDate().year;
            auto hour = rtc.getTime().hours;
            auto minute = rtc.getTime().minutes;
            drawTimeScreen(month, day, year, hour, minute);
        }
    }
    if (page == 35) {
        auto month = rtc.getDate().month;
        auto day = rtc.getDate().date;
        auto year = rtc.getDate().year;
        auto hour = rtc.getTime().hours;
        auto minute = rtc.getTime().minutes;

        // Check if the user clicked on the month up arrow
        if(touch.wasClicked() && touch.x > 5 && touch.x < 45 && touch.y > 30 && touch.y < 60){
            // Set the month to the next month
            month++;
            if (month > 12) {
                month = 1;
            }
            rtc.setDate({year, month, day });
            drawTimeScreen(month, day, year, hour, minute);
        }
        // Check if the user clicked on the month down arrow
        if(touch.wasClicked() && touch.x > 5 && touch.x < 45 && touch.y > 150 && touch.y < 180){
            // Set the month to the previous month
            month--;
            if (month < 1) {
                month = 12;
            }
            rtc.setDate({year, month, day });
            drawTimeScreen(month, day, year, hour, minute);
        }
        // Check if the user clicked on the day up arrow
        if(touch.wasClicked() && touch.x > 70 && touch.x < 110 && touch.y > 30 && touch.y < 60){
            // Set the day to the next day
            day++;
            if (month == 2 && day > 29){
                day = 1;
            }
            if (day > 31) {
                day = 1;
            }
            rtc.setDate({year, month, day });
            drawTimeScreen(month, day, year, hour, minute);
        }
        // Check if the user clicked on the day down arrow
        if(touch.wasClicked() && touch.x > 70 && touch.x < 110 && touch.y > 150 && touch.y < 180){
            // Set the day to the previous day
            day--;
            if (month == 2 && day < 1){
                day = 29;
            }
            if (day < 1) {
                day = 31;
            }
            rtc.setDate({year, month, day });
            drawTimeScreen(month, day, year, hour, minute);
        }
        // Check if the user clicked on the year up arrow
        if(touch.wasClicked() && touch.x > 135 && touch.x < 175 && touch.y > 30 && touch.y < 60){
            // Set the year to the next year
            year++;
            if (year > 2099) {
                year = 2024;
            }
            rtc.setDate({year, month, day });
            drawTimeScreen(month, day, year, hour, minute);
        }
        // Check if the user clicked on the year down arrow
        if(touch.wasClicked() && touch.x > 135 && touch.x < 175 && touch.y > 150 && touch.y < 180){
            // Set the year to the previous year
            year--;
            if (year < 2024) {
                year = 2099;
            }
            rtc.setDate({year, month, day });
            drawTimeScreen(month, day, year, hour, minute);
        }
        // Check if the user clicked on the hour up arrow
        if(touch.wasClicked() && touch.x > 190 && touch.x < 250 && touch.y > 30 && touch.y < 60){
            // Set the hour to the next hour
            hour++;
            if (hour > 23) {
                hour = 0;
            }
            rtc.setTime({hour, minute, 0 });
            drawTimeScreen(month, day, year, hour, minute);
        }
        // Check if the user clicked on the hour down arrow
        if(touch.wasClicked() && touch.x > 190 && touch.x < 250 && touch.y > 150 && touch.y < 180){
            // Set the hour to the previous hour
            hour--;
            if (hour < 0) {
                hour = 23;
            }
            rtc.setTime({hour, minute, 0 });
            drawTimeScreen(month, day, year, hour, minute);
        }
        // Check if the user clicked on the minute up arrow
        if(touch.wasClicked() && touch.x > 265 && touch.x < 325 && touch.y > 30 && touch.y < 60){
            // Set the minute to the next minute
            minute++;
            if (minute > 59) {
                minute = 0;
            }
            rtc.setTime({hour, minute, 0 });
            drawTimeScreen(month, day, year, hour, minute);
        }
        // Check if the user clicked on the minute down arrow
        if(touch.wasClicked() && touch.x > 265 && touch.x < 325 && touch.y > 150 && touch.y < 180){
            // Set the minute to the previous minute
            minute--;
            if (minute < 0) {
                minute = 59;
            }
            rtc.setTime({hour, minute, 0 });
            drawTimeScreen(month, day, year, hour, minute);
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
