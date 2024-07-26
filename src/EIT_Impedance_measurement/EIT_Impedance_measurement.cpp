//
// Created by tvannier1 on 04/02/2024.
//

// Include necessary libraries and header files
#include "EIT_Impedance_measurement.h"
#include "EIT_Interface_Display.h"

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
 * @param _MCP The MCP23008 instance.
 * @param _ad5933 The AD5933 instance.
 * @param _gain The gain array.
 */
void computeTabImpedance(MCP23008 _MCP, AD5933 _ad5933, double _gain[]) {
    // Select electrodes and measure impedance
    for (int iRow = 0; iRow < 8; iRow++) {
        for (int iColumn = iRow + 1; iColumn < 8; iColumn++) {
            // Skip the iteration if the calibration value is zero
            if (tabCalibration[iRow][iColumn] == 0) {
                continue;
            }
            // Configure MCP23008 to select iRow and iColumn
            _MCP.digitalWrite(0, (iRow & 0x01) ? HIGH : LOW);
            _MCP.digitalWrite(1, (iRow & 0x02) ? HIGH : LOW);
            _MCP.digitalWrite(2, (iRow & 0x04) ? HIGH : LOW);
            _MCP.digitalWrite(3, (iColumn & 0x01) ? HIGH : LOW);
            _MCP.digitalWrite(4, (iColumn & 0x02) ? HIGH : LOW);
            _MCP.digitalWrite(5, (iColumn & 0x04) ? HIGH : LOW);

            // Perform frequency sweep and compute impedance
            frequencySweep(iRow, iColumn, _ad5933, _gain);
        }
    }
}

/**
 * @brief Performs a frequency sweep and computes the impedance for the given electrode pair.
 * @param iRow Index of the row electrode.
 * @param iColumn Index of the column electrode.
 * @param _ad5933 The AD5933 instance.
 * @param _gain The gain array.
 * @return The computed impedance value.
 */
double frequencySweep(int iRow, int iColumn, AD5933 _ad5933, double _gain[]) {
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
        // Print a message that the frequency sweep failed
        Serial.println("Frequency sweep failed");
        return -1;
    }
}

/**
 * @brief Prints the tabImpedance matrix to the serial output.
 */
void printTabImpedance() {
    // Print the impedance value at the current row and column to the serial output
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Serial.print(tabImpedance[i][j]);
            Serial.print("\t");
        }
        Serial.println();
    }
}

/**
 * @brief Sends the tabImpedance matrix to the serial output.
 */
void sendTabImpedance() {
    Serial.println("Start of data sending...");

    // Print the impedance value at the current row and column to the serial output
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Serial.print(tabImpedance[i][j]);
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

/**
 * @brief Performs a cycle of impedance measurements for a given number of gesture repetitions and sample repetitions with gestures in random order.
 * @param _gesture_repetition The number of gesture repetitions.
 * @param _sample_repetition The number of sample repetitions.
 * @param _MCP The MCP23008 instance.
 * @param _ad5933 The AD5933 instance.
 * @param _gain The gain array.
 */
void impedanceRandomCycle(int _gesture_repetition, int _sample_repetition, MCP23008 _MCP, AD5933 _ad5933, double _gain[]) {
    // Perform impedance measurements for each gesture
    int size = gestureList.size();
    for (int i = 0; i < _gesture_repetition; i++) {
        for (int j = 0; j < size; j++) {
            drawCycleInfo(gestureList[j], 3);
            drawDataRecordScreen();
            for (int k = 0; k < _sample_repetition; k++) {        
                computeTabImpedance(_MCP, _ad5933, _gain);
                AddData(gestureList.at(j));
                delay(100);
            }
        }
    }
    BLECommunication::getInstance()->sendCsvEndMarker();
    
}

void InitGestureList(){
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            gestureList.push_back(gestureType + "_" + Params1[i] + "_" + Params2[j]);
        }
    }
}

void InitData() {
    File file;
    String FileName = "/SensorData_" + gestureType + ".csv";

    if(!SD.exists(FileName)) {
        file = SD.open(FileName, FILE_WRITE);
        for (int i = 0; i < 8; i++) {
            for (int j = i + 1; j < 8; j++) {
                file.print("Electrode " + String(i) + "-" + String(j));
                file.print(";");
            }
        }
        file.println("Gesture");
        file.close();
    }
}

/**
 * @brief Creates a file with the given gesture name and writes the tabImpedance matrix to it.
 * @param gesture The gesture name.
 */
void AddData(String& gesture) {
    String FileName = "/SensorData_" + gesture + ".csv";
    File file = SD.open(FileName, FILE_APPEND);

    if (!file) {
        Serial.println("Failed to access file, attempting Bluetooth communication only");

        // Attempt Bluetooth communication
        if(BLECommunication::getInstance()->isDeviceConnected()){
            BLECommunication::getInstance()->sendCsvDataToClient(gesture, tabImpedance);
        }
        return;
    }
    // If the file is successfully opened, proceed with SD card operations
    Serial.println("Data add on the SD card");

    // Write the impedance values to the file
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            file.print(tabImpedance[i][j]);
            file.print(";");
        }
    }
    file.println(gesture);
    file.close();

    // After SD card operation, proceed with Bluetooth communication
    if(BLECommunication::getInstance()->isDeviceConnected()){
        BLECommunication::getInstance()->sendCsvDataToClient(gesture, tabImpedance);
    }
}






 