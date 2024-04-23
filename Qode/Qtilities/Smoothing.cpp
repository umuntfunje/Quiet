#include "Smoothing.h"
#include <cmath>

//*****************************************************************************************************************************************
template <class T>
Smoothing<T>::Smoothing(T _initialValue, T _timeInMS, bool _linearSmoothing, T _minValue, T _maxValue)
{
  outPut = _initialValue;

  if (_linearSmoothing)
  {
    linearIncrement = 0;
    minValue = _minValue;
    maxValue = _maxValue;
    linearSmoothing = _linearSmoothing;
  }

  setSmoothingTime(_timeInMS, 44100);
}

//*****************************************************************************************************************************************
template <class T>
void Smoothing<T>::setSmoothingTime(T _timeInMS, T _sampleRate)
{
  T d = static_cast<T>(_timeInMS * static_cast<T>(0.001) * _sampleRate);

  if (linearSmoothing)
    linearIncrement = (maxValue - minValue) / (d == 0 ? 1 : d);
  else
  {
    static constexpr double twoPI = 6.283185307179586476925286766559;

    a = d == 0 ? 0 : static_cast<T>(std::exp(static_cast<T>(-twoPI) / d));
    b = 1 - a;
  }
}

//*****************************************************************************************************************************************
template <class T>
T Smoothing<T>::smoothen(T _inPut)
{
  if (_inPut == outPut)
    return outPut;

  if (linearSmoothing)
  {
    if (_inPut > outPut)
    {
      outPut += linearIncrement;

      if (_inPut < outPut)
        outPut = _inPut;

      return outPut;
    }
    else // if (_inPut < outPut)
    {
      outPut -= linearIncrement;

      if (_inPut > outPut)
        outPut = _inPut;

      return outPut;
    }
  }
  else
  {
    if (std::abs(static_cast<double>(_inPut - outPut)) < b) // logarithmic smoothing never reaches 0 (long smooth times make this useless)
      return outPut = _inPut;
    else
      return outPut = (_inPut * b) + (outPut * a);
  }
}

//*****************************************************************************************************************************************
template <class T>
T Smoothing<T>::getMinValue() const
{
  return minValue;
}

//*****************************************************************************************************************************************
template <class T>
T Smoothing<T>::getMaxValue() const
{
  return maxValue;
}

//*****************************************************************************************************************************************
template class Smoothing<float>;
template class Smoothing<double>;