#pragma once

#include "IControl.h"

using namespace iplug;
using namespace igraphics;

//*****************************************************************************************************************************************
static constexpr int ioChannels = 2;
static constexpr int maxQnodes = 100;
static constexpr int maxResolution = 4096;
static constexpr int xtraValues = 2;
static constexpr float maxQurvature = 50.f;
static constexpr float thiQness = 1.5f;

static constexpr double rateScalars[] = { .25 / 4.,
                                          (1. / 3.) / 4., .375 / 4., .5 / 4.,
                                          (2. / 3.) / 4., .75 / 4., 1. / 4.,
                                          (4. / 3.) / 4., 1.5 / 4., 2. / 4.,
                                          (8. / 3.) / 4., 3. / 4., 4. / 4.,
                                          (16. / 3.) / 4., 6. / 4., 8. / 4.,
                                          (32. / 3.) / 4., 12. / 4., 16. / 4.,
                                          (64. / 3.) / 4., 24. / 4., 32. / 4.,
                                          (128. / 3.) / 4., 48. / 4., 64. / 4. };
static const std::initializer_list<const char*> _rates = { "4 bars",
                                                               "3 bars", "2 bars t", "2 bars",
                                                               "1.5 bars", "1 bar t", "1 bar",
                                                               "1/2.", "1/2 t", "1/2",
                                                               "1/4.", "1/4 t", "1/4",
                                                               "1/8.", "1/8 t", "1/8",
                                                               "1/16.", "1/16 t", "1/16",
                                                               "1/32.", "1/32 t", "1/32",
                                                               "1/64.", "1/64 t", "1/64" };

using SampleBuffer = std::vector<float>;
using DrawQutton = std::function<void(IGraphics&, const IRECT&, const IColor&, float)>;

//*****************************************************************************************************************************************
/* A logarithmic/linear smoother class */
template <class T>
class Smoothing
{
public:
  /* Constructs a smoother (with default 44100Hz sampling rate)
   * @param _initialValue Initial value of parameter / value being smoothed
   * @param _timeInMS Time in milliseconds before smoothing completes (approx. ?)
   * @param _linearSmoothing Set True for linear smoothing, or False for logarithmic smoothing
   * @param _minValue The minimum value of the parameter being smoothed (for linear smoothing)
   * @param _maxValue The maximum value of the parameter being smoothed (for linear smoothing) */
  Smoothing(T _initialValue, T _timeInMS, bool _linearSmoothing = false, T _minValue = 0, T _maxValue = 1)
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

  /* Resets the smoothing time
   * @param _timeInMS Time in milliseconds before smoothing completes (approx. ?)
   * @param _sampleRate The current sampling rate in Hertz (Hz) */
  void setSmoothingTime(T _timeInMS, T _sampleRate)
  {
    if (linearSmoothing)
      linearIncrement = (maxValue - minValue) / static_cast<T>(_timeInMS * static_cast<T>(.001) * _sampleRate);
    else
    {
      static constexpr double twoPI = 6.283185307179586476925286766559;

      a = static_cast<T>(std::exp(static_cast<T>(-twoPI) / (_timeInMS * static_cast<T>(.001) * _sampleRate)));
      b = 1 - a;
    }
  }

  /* Applies smoothing on the current value
   * @param _inPut Current (new) value being smoothed
   * @return The smoothed value */
  T smoothen(T _inPut)
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
      return outPut = (_inPut * b) + (outPut * a);
  }

  /* @return The minimum value (typically for linearSmoothing!!!) */
  T getMinValue() const { return minValue; }

  /* @return The maximum value (typically for linearSmoothing!!!) */
  T getMaxValue() const { return maxValue; }

private:
  T outPut, a, b;
  T linearIncrement, minValue, maxValue;
  bool linearSmoothing;
};

/* A circular buffer class */
template <class T>
class CircularBuffer
{
public:
  /* Default constructor */
  CircularBuffer()
  {}

  /* Initialize the buffer
   * @param _size The size of the new buffer (typically in samples) */
  void resize(uint32_t _size)
  {
    buffer.clear();
    buffer.resize(_size, 0);
    currPosition = 0;
  }

  /* Cleans the buffer (sets every value to zero, sets current position to zero) */
  void clean()
  {
    uint32_t currSize = buffer.size();

    buffer.clear();
    buffer.resize(currSize, 0);
    currPosition = 0;
  }

  /* Add a new value to the buffer
   * @param _s The value that updates the buffer (typically a sample) */
  void write(T _s)
  {
    if (buffer.empty())
      return;

    buffer[currPosition++] = _s;
    currPosition %= buffer.size();
  }

  /* Returns the delayed value from the buffer (look ahead)
   * @return The delayed value (typically a sample) */
  T read()
  {
    if (buffer.empty())
      return 0;
    else
      return buffer[currPosition];
  }

  /* Returns the current size the buffer
   * @return The current size the buffer (typically in samples) */
  uint32_t getSize() const { return buffer.size(); }

private:
  std::vector<T> buffer;
  uint32_t currPosition = 0;
};

//*****************************************************************************************************************************************
/* Trace an image from an SVG onto a Graphics context...sort of...
 * @param _qui Graphics context
 * @param _svg The SVG image to be traced out
 * @param _bounds Bounds to trace image to
 * @param _c Original color of the image */
static void traceOut(IGraphics& _qui, const ISVG& _svg, const IRECT& _bounds, const IColor& _color)
{
  NSVGshape* shape = _svg.mImage->shapes;

  while (shape)
  {
    NSVGpath* path = shape->paths;
    float strokeW = shape->strokeWidth;
    float opa = shape->opacity;

    while (path)
    {
      float* pointList = path->pts;
      const int numPoints = path->npts;

      _qui.PathMoveTo(pointList[0] + _bounds.L, pointList[1] + _bounds.T);

      for (int k = 0; k < numPoints * 2; k += 2)
      {
        float x1 = pointList[k] + _bounds.L;
        float y1 = pointList[k + 1] + _bounds.T;

        _qui.PathLineTo(x1, y1);
      }

      path = path->next;
    }

    _qui.PathStroke(_color.WithOpacity(opa), strokeW);
    shape = shape->next;
  }
}

/* Inverts (or reverses?) coordinates
 * @param _point Coordinates to be inverted
 * @param _bounds Bounds to plot the point
 * @return Inverted coordinates */
static inline std::pair<float, float> inverted(const std::pair<float, float>& _point, const IRECT& _bounds)
{
  return {_bounds.R - _point.first + _bounds.L, _bounds.B - _point.second + _bounds.T};
}