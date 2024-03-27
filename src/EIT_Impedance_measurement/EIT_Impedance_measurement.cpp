//
// Created by tvannier1 on 04/02/2024.
//

#include "EIT_Impedance_measurement/EIT_Impedance_measurement.h"
/**
 * @brief Initializes the tabImpedance matrix with zeros.
 */
void initTabImpedance() {
    for (int i = 0; i < 8; i++) // Loop through each row of the tabImpedance matrix
    {
        for (int j = 0; j < 8; j++) // Loop through each column of the tabImpedance matrix
        {
            tabImpedance[i][j] = 0;
        }
    }
}

/**
 * @brief Computes the impedance values for each electrode pair and updates the tabImpedance matrix.
 */
void computeTabImpedance(MCP23008 _MCP, AD5933 _ad5933, double _gain[]) {
    // Select electrodes and measure impedance
    for (int iRow = 0; iRow < 8; iRow++) {
        String rowStr = String(iRow) + ": ";
        for (int iColumn = iRow + 1; iColumn < 8; iColumn++) {
            // Skip the iteration if the calibration value is zero
            if (tabCalibration[iRow][iColumn] == 0) {
                continue;
            }
            // Configure GP0, GP1, GP2 pins to select iRow
            _MCP.digitalWrite(0, (iRow & 0x01) ? HIGH : LOW);
            _MCP.digitalWrite(1, (iRow & 0x02) ? HIGH : LOW);
            _MCP.digitalWrite(2, (iRow & 0x04) ? HIGH : LOW);

            // Configure GP3, GP4, GP5 pins to select iColumn
            _MCP.digitalWrite(3, (iColumn & 0x01) ? HIGH : LOW);
            _MCP.digitalWrite(4, (iColumn & 0x02) ? HIGH : LOW);
            _MCP.digitalWrite(5, (iColumn & 0x04) ? HIGH : LOW);

            frequencySweep(iRow, iColumn, _ad5933, _gain);
            rowStr += String(iColumn) + " ";
        }
//        M5.Lcd.println(rowStr);
    }
}
/**
 * @brief Performs a frequency sweep and computes the impedance for the given electrode pair.
 *
 * @param iRow Index of the row electrode.
 * @param iColumn Index of the column electrode.
 * @return The computed impedance value.
 */
double frequencySweep(int iRow, int iColumn, AD5933 _ad5933, double _gain[]) // Perform a frequency sweep and compute the impedance for the given electrode pair
{
    // Create arrays to hold the data
    int real[NUM_INCR + 1], imag[NUM_INCR + 1];

    // Perform the frequency sweep
    if (_ad5933.frequencySweep(real, imag, NUM_INCR + 1)) {

        // Compute impedance
        double realSquare = pow(real[0], 2);
        double imagSquare = pow(imag[0], 2);
        double magnitude = sqrt(realSquare + imagSquare);
        double impedance = 1 / (magnitude * _gain[0]);

        // Store impedance
        tabImpedance[iRow][iColumn] = impedance - tabCalibration[iRow][iColumn];

        return impedance;

    } else {
        // print a message that the frequency sweep failed
        Serial.println("Frequency sweep failed");
        return -1;
    }
}


/**
 * @brief Prints the tabImpedance matrix to the serial output.
 */
void printTabImpedance() {

    // Print the impedance value at the current row and column to the serial output with perfect column alignment by adding as many spaces as in the largest value
    for (int i = 0; i < 8; i++) // Loop through each row of the tabImpedance matrix
    {
        for (int j = 0; j < 8; j++) // Loop through each column of the tabImpedance matrix
        {
            Serial.print(tabImpedance[i][j]); // Print the impedance value at the current row and column to the serial output
            for (int k = 0; k < 10 - String(tabImpedance[i][j]).length(); k++) // Loop through each space required to align the columns
            {
                Serial.print(" "); // Print a space character to the serial output
            }
            Serial.print("\t"); // Print a tab character to the serial output
        }
        Serial.println(); // Print a newline character to the serial output
    }
    // if all value are 0, print a message to M5.Lcd that the matrix values are zero
    if (tabImpedance[0][1] == 0) {
        Serial.println("Impedance matrix values are zero");
        M5.Lcd.setTextColor(TFT_RED);
        M5.Lcd.println("Impedance matrix values are zero");
        M5.Lcd.setTextColor(TFT_WHITE);
    }
    else {
        M5.Lcd.setTextColor(TFT_GREEN);
        M5.Lcd.println("Impedance measurements done");
        M5.Lcd.setTextColor(TFT_WHITE);
    }

}
void sendTabImpedance(){
    Serial.println("Start of data sending...");

    for (int i = 0; i < 8; i++) // Loop through each row of the tabImpedance matrix
    {
        for (int j = 0; j < 8; j++) // Loop through each column of the tabImpedance matrix
        {
            Serial.print(tabImpedance[i][j]); // Print the impedance value at the current row and column to the serial output
            // Print a comma character to the serial output if the current column is not the last one
            if (j < 7) {
                Serial.print(";");
            }
            else {
                Serial.println();
            }
        }
    }
    Serial.println("End of data sending");
}

void impedanceRandomCycle(int _gesture_repetition, int _sample_repetition, MCP23008 _MCP, AD5933 _ad5933, double _gain[]) {

    String gestures[7];

    Serial.println("Number of repetitions for each gestures: " + String(_gesture_repetition));

    for (int i = 0; i < _gesture_repetition; i++) {
        Serial.println("Repetition " + String(i + 1));

        for (int j = 1; j < 8; j++) {
            gestures[j-1] = gestureList[j];
        }

        for (int j = 0; j < 7; j++) {
            int k = random(7);
            String temp = gestures[j];
            gestures[j] = gestures[k];
            gestures[k] = temp;
        }

        for (int j = 0; j < 7; j++) {
            Serial.println();
            Serial.println(gestures[j]);
            doGestureDisplay(gestures[j]);
            countDownDisplay(5);
            M5.Lcd.fillRect(110, 200, 60, 40, TFT_BLACK);
            M5.Lcd.setCursor(110, 200);
            M5.Lcd.setTextSize(4);
            M5.Lcd.print("Hold");
            for (int k = 0; k < _sample_repetition; k++){
                computeTabImpedance(_MCP, _ad5933, _gain);
                sendTabImpedance();
                fileCreation(gestures[j]);
                if (k % 3 == 0){
                    M5.Lcd.print(".");
                }
                delay(500);
            }
            Serial.println();
            M5.Lcd.fillRect(100, 200, 220, 40, TFT_BLACK);
            M5.Lcd.setCursor(120, 200);
            M5.Lcd.setTextColor(TFT_GREEN);
            M5.Lcd.print("Done");
            M5.Lcd.setTextColor(TFT_WHITE);
            delay(1000);
        }

    }
}
void fileCreation(const String& gesture){
    // Create a new file with the current date and time
    if(!getLocalTime(&timeInfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    // Create a filename with the current gesture and the current date and time
    String filename = "/" + gesture + "_" + String(timeInfo.tm_year + 1900) + "-" + String(timeInfo.tm_mon + 1) + "-" + String(timeInfo.tm_mday) + "_" + String(timeInfo.tm_hour) + "-" + String(timeInfo.tm_min) + "-" + String(timeInfo.tm_sec) + ".csv";

    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to create file");
        return;
    }
    Serial.println("File created on the SD card");
    // Write the impedance values to the file
    for (int i = 0; i < 8; i++) // Loop through each row of the tabImpedance matrix
    {
        for (int j = 0; j < 8; j++) // Loop through each column of the tabImpedance matrix
        {
            file.print(tabImpedance[i][j]); // Write the impedance value at the current row and column to the file
            // Write a comma character to the file if the current column is not the last one
            if (j < 7) {
                file.print(";");
            }
            else {
                file.println();
            }
        }
    }


    file.close();
}


