//
// Created by tvannier1 on 04/02/2024.
//

// Include necessary libraries and header files
#include <random>
#include "EIT_Interface_Display/EIT_Interface_Display.h"
#include "EIT_Shared_Values.h"

#ifndef IMPEDANCE_H
#define IMPEDANCE_H

/**
 * @brief Performs a frequency sweep and returns the impedance value for a given row and column.
 * @param iRow The row index.
 * @param iColumn The column index.
 * @param _ad5933 The AD5933 instance.
 * @param _gain The gain array.
 * @return The impedance value.
 */
double frequencySweep(int iRow, int iColumn, AD5933 _ad5933, double _gain[]);

/**
 * @brief Computes the impedance matrix.
 * @param _MCP The MCP23008 instance.
 * @param _ad5933 The AD5933 instance.
 * @param _gain The gain array.
 */
void computeTabImpedance(MCP23008 _MCP, AD5933 _ad5933, double _gain[]);

/**
 * @brief Prints the impedance matrix.
 */
void printTabImpedance();

/**
 * @brief Initializes the impedance matrix with zeros.
 */
void initTabImpedance();

/**
 * @brief Sends the impedance matrix.
 */
void sendTabImpedance();

/**
 * @brief Performs a random cycle of impedance measurements.
 * @param _gesture_repetition The number of gesture repetitions.
 * @param _sample_repetition The number of sample repetitions.
 * @param _MCP The MCP23008 instance.
 * @param _ad5933 The AD5933 instance.
 * @param _gain The gain array.
 */
void impedanceRandomCycle(int _gesture_repetition, int _sample_repetition, MCP23008 _MCP, AD5933 _ad5933, double _gain[]);

/**
 * @brief Creates a file with the given gesture name.
 * @param gesture The gesture name.
 */
void fileCreation(const String& gesture);

#endif