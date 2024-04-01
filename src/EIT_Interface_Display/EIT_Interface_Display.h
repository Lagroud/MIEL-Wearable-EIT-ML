//
// Created by tvannier1 on 13/03/2024.
//

#ifndef EIT_PROJECT_DRAWS_H
#define EIT_PROJECT_DRAWS_H

#include <M5CoreS3.h>
#include "EIT_Shared_Values.h"

// Base coordinates for drawing on the screen
const int baseX = 160;
const int baseY = 50;
// Spacing between icons on the screen
const int iconSpacing = 100;

/**
 * @brief Draws the Pear logo on the screen.
 */
void drawPearLogo();

/**
 * @brief Draws the home screen of the interface.
 */
void drawHomeScreen();

/**
 * @brief Draws the record screen of the interface.
 */
void drawRecordScreen();

/**
 * @brief Draws the file screen of the interface.
 */
void drawFileScreen();

/**
 * @brief Draws the random cycle screen of the interface.
 *
 * @param _gesture_repetition The number of gesture repetitions.
 * @param _sample_repetition The number of sample repetitions.
 * @param _isGesture A boolean value indicating whether the gesture repetition is to be displayed.
 */
void drawRandomCycleScreen(int _gesture_repetition, int _sample_repetition, boolean _isGesture);

/**
 * @brief Displays the given gesture repetition on the screen.
 *
 * @param _gesture_repetition The number of gesture repetitions.
 */
void gestureRepetitionDisplay(int _gesture_repetition);

/**
 * @brief Displays the sample repetition on the screen.
 *
 * @param _sample_repetition The number of sample repetitions.
 */
void sampleRepetitionDisplay(int _sample_repetition);

/**
 * @brief Displays the given gesture on the screen.
 *
 * @param gesture The gesture to be displayed.
 */
void doGestureDisplay(const String& gesture);

/**
 * @brief Displays a countdown on the screen.
 *
 * @param countDown The starting number of the countdown.
 */
void countDownDisplay(int countDown);

/**
 * @brief Draws the gesture list screen.
 *
 * @param gesture The gesture to be displayed.
 */
void drawGestureListScreen(const String& gesture);

/**
 * @brief Manages the interface of the application.
 */
void interfaceGestion();

#endif //EIT_PROJECT_DRAWS_H