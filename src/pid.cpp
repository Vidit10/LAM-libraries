#include <config.h>   // Include configuration file for constants (e.g., gains, limits)
#include <cstdlib>    // Include for dynamic memory allocation
#include <cmath>      // Include for math functions like ceil and sqrt

// External variables used by the PID controller
extern float alpha;                    // Filter coefficient for exponential moving average
extern float last_integral;            // Last calculated integral for anti-windup
extern float temperature_target;       // Desired target temperature
extern float temperature_current;      // Current temperature reading
extern unsigned long time_stamp_current;  // Current timestamp
extern unsigned long time_stamp_previous; // Previous timestamp
extern float error_filtered_previous;  // Previously filtered error

// PID control function for pump and peltier control
int * PID()
{
    // Calculate the temperature error
    float error = temperature_target - temperature_current;

    // Get the current timestamp and calculate the sampling time in seconds
    time_stamp_current = millis();
    double Ts = (time_stamp_current - time_stamp_previous) / 1000.0;

    // Apply exponential moving average filter to reduce noise in error signal
    float error_filtered = alpha * error + (1 - alpha) * error_filtered_previous;

    // Calculate derivative and integral of the filtered error
    float derivative = (error_filtered - error_filtered_previous) / Ts;
    float integral = last_integral + error * Ts;

    // Calculate control values for pump and peltier using PID formula and apply ceiling to control values
    int pump_control_value = ceil(pump_Kp * error + pump_Kd * derivative + pump_Ki * integral);
    int peltier_control_value = ceil(peltier_Kp * error + peltier_Kd * derivative + peltier_Ki * integral);

    // Clamp the pump control output within max and min limits, applying anti-windup for the integral
    if (pump_control_value > pump_max_output)
    {
        pump_control_value = pump_max_output;
    }
    else if (pump_control_value < pump_min_output)
    {
        pump_control_value = pump_min_output;
    }
    else
    {
        // Anti-windup: Update the last integral only if the control is within bounds
        last_integral = integral;
    }

    // Clamp the peltier control output within max and min limits, applying anti-windup for the integral
    if (peltier_control_value > peltier_max_output)
    {
        peltier_control_value = peltier_max_output;
    }
    else if (peltier_control_value < peltier_min_output)
    {
        peltier_control_value = peltier_min_output;
    }
    else
    {
        // Anti-windup: Update the last integral only if the control is within bounds
        last_integral = integral;
    }

    // Save the filtered error for the next iteration
    error_filtered_previous = error_filtered;

    // Dynamically allocate memory for control values (pump and peltier) and return
    int *control_values = (int *)malloc(2 * sizeof(int));
    control_values[0] = pump_control_value;
    control_values[1] = peltier_control_value;
    return control_values;
}

// Function to calculate the alpha value for an EMA filter, based on a specified cutoff frequency
float calculateAlphaEMAFilter(float fn)
{
    if (fn <= 0)
    {
        return 1; // Return 1 if cutoff frequency is non-positive (no filtering)
    }
    // Calculate alpha using a cosine-based formula for cutoff frequency
    float c = cos(2 * M_PI * fn);
    return c - sqrt(c * c - 4 * c + 3);
}
