//
// Created by tvannier1 on 04/02/2024.
//

// EIT_Shared_Values.h
// This header file contains the shared values and variables used in the EIT project.

#ifndef EIT_M5CORES3_TVANNIER_CONSTANTES_H
#define EIT_M5CORES3_TVANNIER_CONSTANTES_H

// Include necessary libraries and header files
#include "AD5933.h"
#include "MCP23008.h"
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <vector>

// SD card pin definition for M5CoreS3 (ESP32)
#define SD_SPI_SCK_PIN  36
#define SD_SPI_MISO_PIN 35
#define SD_SPI_MOSI_PIN 37
#define SD_SPI_CS_PIN   4

// Frequency sweep parameters for the AD5933 impedance analyzer
#define START_FREQ (40000) // Start frequency in Hz
#define FREQ_INCR (0) // Frequency increment in Hz
#define NUM_INCR (1) // Number of increments
#define REF_RESIST (1000) // Reference resistance in ohms

// Calibration values for the impedance measurements
extern AD5933 ad5933; // AD5933 instance
extern MCP23008 MCP; // MCP23008 instance

extern int page; // Current page (0: Home, 1: Record, 2: Configuration, 3: Files)
extern int gesture_repetition; // Number of gesture repetitions
extern int sample_repetition; // Number of sample repetitions
extern int gesture_index; // Current gesture
extern boolean isGesture;
extern boolean isReadyForGesture; // Type of repetition (gesture or sample)
extern m5::touch_state_t prev_state; // Previous touch state

extern double gain[]; // Array to store the gain values
extern int phase[]; // Array to store the phase values

const String gestureType = "Grip";
// vector Gesture list
extern std::vector<String> gestureList;

const String Params1[3] = {"Bas", "Milieu", "Haut"};
const String Params2[4] = {"0", "90", "180", "270"};

extern struct tm timeInfo; // Time information
extern m5::RTC8563_Class rtc; // RTC8563 instance

// Wi-Fi information
extern const char* ssid; // Wi-Fi SSID
extern const char* password; // Wi-Fi password

static double tabImpedance[8][8]; // Matrix to store the impedance values
static double tabCalibration[8][8] = {
        { 0, 0.24, 0.40, 0.40, 0.52, 0.19, 0.23, 0.19 },
        { 0, 0, 0.63, 0.35, 0.19, 0.19, 0.19, 0.19 },
        { 0, 0, 0, 0.31, 0.23, 0.24, 0.28, 0.28 },
        { 0, 0, 0, 0, 0.36, 0.28, 0.24, 0.24 },
        { 0, 0, 0, 0, 0, 0.28, 0.35, 0.24 },
        { 0, 0, 0, 0, 0, 0, 0.35, 0.47 },
        { 0, 0, 0, 0, 0, 0, 0, 0.47 },
        { 0, 0, 0, 0, 0, 0, 0, 0 }
}; // Matrix to store the calibration values for the impedance measurements

#endif //EIT_M5CORES3_TVANNIER_CONSTANTES_H