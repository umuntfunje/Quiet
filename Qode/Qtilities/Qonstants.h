#pragma once

#include <initializer_list>

//*****************************************************************************************************************************************
static constexpr int ioChannels = 2;

static constexpr int xtraValues = 2;
static constexpr int maxQnodes = 100;
static constexpr int maxResolution = 4096;
static constexpr float maxQurvature = 50.0f;

static constexpr float strokeWidth = 2.0f;

static constexpr double rateScalars[] = { 0.25 / 4.0,
                                        (  1.0 / 3.0) / 4.0,  0.375 / 4.0,  0.5 / 4.0,
                                        (  2.0 / 3.0) / 4.0,   0.75 / 4.0,  1.0 / 4.0,
                                        (  4.0 / 3.0) / 4.0,    1.5 / 4.0,  2.0 / 4.0,
                                        (  8.0 / 3.0) / 4.0,    3.0 / 4.0,  4.0 / 4.0,
                                        ( 16.0 / 3.0) / 4.0,    6.0 / 4.0,  8.0 / 4.0,
                                        ( 32.0 / 3.0) / 4.0,   12.0 / 4.0, 16.0 / 4.0,
                                        ( 64.0 / 3.0) / 4.0,   24.0 / 4.0, 32.0 / 4.0,
                                        (128.0 / 3.0) / 4.0,   48.0 / 4.0, 64.0 / 4.0 };
static const std::initializer_list<const char*> _rates = {   "4 bars",
                                                             "3 bars", "2 bars t", "2 bars",
                                                           "1.5 bars", "1 bar t",  "1 bar",
                                                            "1/2.",  "1/2 t", "1/2",
                                                            "1/4.",  "1/4 t", "1/4",
                                                            "1/8.",  "1/8 t", "1/8",
                                                           "1/16.", "1/16 t", "1/16",
                                                           "1/32.", "1/32 t", "1/32",
                                                           "1/64.", "1/64 t", "1/64" };