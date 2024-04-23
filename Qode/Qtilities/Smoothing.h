#pragma once

//*****************************************************************************************************************************************
/* A logarithmic / linear smoother class */
template <class T>
class Smoothing
{
public:
  //***************************************************************************************************************************************
  /* Constructs a smoother (with default 44100Hz sampling rate)
   * @param _initialValue Initial value of parameter / value being smoothed
   * @param _timeInMS Time in milliseconds before smoothing completes (approx. ?)
   * @param _linearSmoothing Set True for linear smoothing, or False for logarithmic smoothing
   * @param _minValue The minimum value of the parameter being smoothed (for linear smoothing)
   * @param _maxValue The maximum value of the parameter being smoothed (for linear smoothing) */
  Smoothing(T _initialValue, T _timeInMS, bool _linearSmoothing = true, T _minValue = 0, T _maxValue = 1);

  /* Resets the smoothing time
   * @param _timeInMS Time in milliseconds before smoothing completes (approx. ?)
   * @param _sampleRate The current Sample Rate in Hertz (Hz) */
  void setSmoothingTime(T _timeInMS, T _sampleRate);

  /* Applies smoothing on the current value
   * @param _inPut Current (new) value being smoothed
   * @return The smoothed value */
  T smoothen(T _inPut);

  /* @return The minimum value (typically for linearSmoothing!!!) */
  T getMinValue() const;

  /* @return The maximum value (typically for linearSmoothing!!!) */
  T getMaxValue() const;

private:
  //***************************************************************************************************************************************
  T outPut, a, b;
  bool linearSmoothing;
  T linearIncrement, minValue, maxValue;
};