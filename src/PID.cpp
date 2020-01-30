#include "PID.h"
#include <vector>
#include <numeric>
#include <iostream>
#include <unistd.h>

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_)
{
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */

  this->Kp = Kp_;
  this->Ki = Ki_;
  this->Kd = Kd_;

  this->p_error = 0.0;
  this->i_error = 0.0;
  this->d_error = 0.0;

  this->calibration_counter = 100;
}

void PID::UpdateError(double cte)
{
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  double elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - update_time_point).count();

  update_time_point = now;

  if (elapsed_milliseconds < 0)
  {
    elapsed_milliseconds = 0.0;
  }

  /**
   * TODO: Update PID errors based on cte.
   */
  d_error = (cte - p_error) / elapsed_milliseconds; // change in error within a certain time interval
  p_error = cte;
  i_error += cte;
}

double PID::TotalError()
{
  /**
   * TODO: Calculate and return the total error
   */
  return Kp * p_error + Ki * i_error + Kd * d_error; // TODO: Add your total error calc here!
}

void PID::Twiddle(double tolerance)
{
  double pdp = 1.0;
  double pdi = 1.0;
  double pdd = 1.0;

  double error_best = TotalError();

  while ((pdp + pdi + pdd) > 1)
  {
    adjust(Kp, pdp, error_best);
    adjust(Ki, pdi, error_best);
    adjust(Kd, pdd, error_best);
  }
}

void PID::adjust(double &K, double &pd, double &error_best)
{
  K += pd;
  double error = TotalError();

  if (error < error_best)
  {
    error_best = error;
    pd *= 1.1;
  }
  else
  {
    K -= 2 * pd;
    double error = TotalError();

    if (error < error_best)
    {
      error_best = error;
      pd *= 1.1;
    }
    else
    {
      K += pd;
      pd *= 0.9;
    }
  }
}
