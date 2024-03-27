//
// Created by tvannier1 on 04/02/2024.
//
#include <random>
#include "EIT_Interface_Display/EIT_Interface_Display.h"
#include "EIT_Shared_Values.h"
#ifndef IMPEDANCE_H
#define IMPEDANCE_H

double frequencySweep(int iRow, int iColumn, AD5933 _ad5933, double _gain[]); // Perform a frequency sweep
void computeTabImpedance(MCP23008 _MCP, AD5933 _ad5933, double _gain[]); // Compute the impedance values for each electrode pair and update the tabImpedance matrix
void printTabImpedance(); // Print the tabImpedance matrix to the M5.Lcd
void initTabImpedance(); // Initialize the tabImpedance matrix with zeros
void sendTabImpedance(); // Send the tabImpedance matrix to the computer
void impedanceRandomCycle(int _gesture_repetition, int _sample_repetition, MCP23008 _MCP, AD5933 _ad5933, double _gain[]);
void fileCreation(const String& gesture); // Create a file to store the impedance values


#endif
