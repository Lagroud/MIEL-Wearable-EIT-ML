//
// Created by tvannier1 on 22/03/2024.
//

#ifndef EIT_M5CORES3_TVANNIER_CONSTANTES_H
#define EIT_M5CORES3_TVANNIER_CONSTANTES_H

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


// Gesture list
const String gestureList[] = {"CUSTOM", "FIST", "INDEX_PINCH", "LEFT", "RELAX", "RIGHT", "STRETCH", "THUMB_UP"};
const String gestureListWithoutCustom[] = {"FIST", "INDEX_PINCH", "LEFT", "RELAX", "RIGHT", "STRETCH", "THUMB_UP"};

extern struct tm timeinfo; // Declare timeinfo

#endif //EIT_M5CORES3_TVANNIER_CONSTANTES_H
