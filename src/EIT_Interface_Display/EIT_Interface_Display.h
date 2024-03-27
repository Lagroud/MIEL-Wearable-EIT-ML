//
// Created by tvannier1 on 13/03/2024.
//
#ifndef EIT_PROJECT_DRAWS_H
#define EIT_PROJECT_DRAWS_H

#include <M5CoreS3.h>
#include "EIT_Shared_Values.h"

const int baseX = 160; // X coordinate of the first icon
const int baseY = 50; // Y coordinate of the first icon
const int iconSpacing = 100; // Spacing between the icons

void drawPearLogo();
void drawHomeScreen();
void drawRecordScreen();
void drawFileScreen();
void drawRandomCycleScreen(int _gesture_repetition, int _sample_repetition, boolean _isGesture);
void gestureRepetitionDisplay(int _gesture_repetition);
void sampleRepetitionDisplay(int _sample_repetition);
void doGestureDisplay(const String& gesture);
void countDownDisplay(int countDown);
void drawGestureListScreen(const String& gesture);
void interfaceGestion(); // Manage the interface

#endif //EIT_PROJECT_DRAWS_H
