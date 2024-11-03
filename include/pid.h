#ifndef PID_H
#define PID_H

#include <config.h>

/**
 * @brief Calculates the PID control output.
 * @return Pointer to an integer result (ensure this is intentional).
 */
int* PID();


/**
 * @brief Calculates the Exponential Moving Average (EMA) filter coefficient.
 * @param fn The current value to be filtered.
 * @return The filtered value as a float.
 */
float calculateAlphaEMAFilter(float fn);

#endif