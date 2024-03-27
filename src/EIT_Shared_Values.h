#ifndef EIT_M5CORES3_TVANNIER_CONSTANTES_H
#define EIT_M5CORES3_TVANNIER_CONSTANTES_H

#include "AD5933.h"
#include "MCP23008.h"
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>

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
extern AD5933 ad5933;
extern MCP23008 MCP;

extern int page; // Variable to store the current page 0: Home, 1: Record, 2: Configuration, 3: Files
extern int gesture_repetition; // Variable to store the number of gesture repetitions
extern int sample_repetition; // Variable to store the number of sample repetitions
extern int gesture_index; // Variable to store the current gesture
extern boolean isGesture; // Variable to store the type of repetition (gesture or sample)
extern m5::touch_state_t prev_state; // Variable to store the previous touch state

extern double gain[]; // Array to store the gain values
extern int phase[]; // Array to store the phase values

// Gesture list
const String gestureList[] = {"CUSTOM", "FIST", "INDEX_PINCH", "LEFT", "RELAX", "RIGHT", "STRETCH", "THUMB_UP"};

extern struct tm timeInfo; // Declare timeInfo

//Wi-Fi information
extern const char* ssid;
extern const char* password;

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