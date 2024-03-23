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

void gestureRepetitionDisplay(int gesture_repetition){
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
    M5.Lcd.print(gesture_repetition);
    M5.Lcd.setTextSize(1);
}
void sampleRepetitionDisplay(int sample_repetition){
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
    M5.Lcd.print(sample_repetition);
    M5.Lcd.setTextSize(1);
}
void drawRandomCycleScreen(int gesture_repetition, int sample_repetition, boolean isGesture){
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

    if (isGesture)
        gestureRepetitionDisplay(gesture_repetition);
    else
        sampleRepetitionDisplay(sample_repetition);

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

