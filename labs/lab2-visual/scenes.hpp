#pragma once

// One "scene" per exercise. Each scene fills the renderer with lines/points.

#include <string>

const int SCENE_COUNT = 5;

void newRandomValues();               // pick new random vectors (Space key)
void buildScene(int scene);           // add this frame's shapes for exercise 1..5
bool sceneIs2D(int scene);            // exercise 5 is drawn flat, the rest in 3D
float sceneCameraDistance(int scene); // how far the 3D camera stands
std::string sceneTitle(int scene);
void printSceneLegend(int scene);     // explains the colours in the console
