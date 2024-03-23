//
// Created by tvannier1 on 04/02/2024.
//
#include "AD5933.h"
#include <M5CoreS3.h>
#include <MCP23008.h>
#include <random>
#include <SD.h>
#include <SPI.h>
#include "EIT_Interface_Display/EIT_Interface_Display.h"
#include "EIT_Shared_Values.h"

#ifndef IMPEDANCE_H
#define IMPEDANCE_H

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


double frequencySweep(int iRow, int iColumn, AD5933 ad5933, double gain[]); // Perform a frequency sweep
void computeTabImpedance(MCP23008 MCP, AD5933 ad5933, double gain[]); // Compute the impedance values for each electrode pair and update the tabImpedance matrix
void printTabImpedance(); // Print the tabImpedance matrix to the M5.Lcd
void initTabImpedance(); // Initialize the tabImpedance matrix with zeros
void sendTabImpedance(); // Send the tabImpedance matrix to the computer
void impedanceRandomCycle(int gesture_repetition,int sample_repetition, MCP23008 MCP, AD5933 ad5933, double gain[]);
void fileCreation(const String& gesture); // Create a file to store the impedance values

#endif
