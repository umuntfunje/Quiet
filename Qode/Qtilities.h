#pragma once

#include "IControl.h"

using namespace iplug;
using namespace igraphics;

//*****************************************************************************************************************************************
static constexpr int outputChannels = 2;
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
static constexpr std::initializer_list<const char*> _rates = { "4 bars",
                                                               "3 bars", "2 bars t", "2 bars",
                                                               "1.5 bars", "1 bar t", "1 bar",
                                                               "1/2.", "1/2 t", "1/2",
                                                               "1/4.", "1/4 t", "1/4",
                                                               "1/8.", "1/8 t", "1/8",
                                                               "1/16.", "1/16 t", "1/16",
                                                               "1/32.", "1/32 t", "1/32",
                                                               "1/64.", "1/64 t", "1/64" };

using sampleBuffer = const std::vector<float>&;
using DrawQutton = std::function<void(IGraphics&, const IRECT&, const IColor&, float)>;

//*****************************************************************************************************************************************
/* A logarithmic/linear smoother class */
template <class T>
class Smoothing
{
public:
  /* Constructs a smoother
   * @param _initialValue Initial value of parameter / value being smoothed
   * @param _timeInMS Time in Milliseconds before smoothing completes
   * @param _sampleRate The current Sample Rate in Hertz (Hz)
   * @param _linearSmoothing Set True for linear smoothing, or False for logarithmic smoothing
   * @param _minValue The minimum value of the parameter being smoothed (for linear smoothing)
   * @param _maxValue The maximum value of the parameter being smoothed (for linear smoothing) */
  Smoothing(T _initialValue, T _timeInMS, T _sampleRate = 44100, bool _linearSmoothing = false, T _minValue = 0, T _maxValue = 1)
  {
    outPut = _initialValue;

    linearSmoothing = _linearSmoothing;
    minValue = _minValue;
    maxValue = _maxValue;
    linearIncrement = 0;

    setSmoothingTime(_timeInMS, _sampleRate);
  }

  /* Sets / resets the initial value
   * @param _initialValue Initial value of parameter / value being smoothed */
  void setInitValue(T _initialValue) { outPut = _initialValue; }

  /* Resets the smoothing time
   * @param _timeInMS Time in Milliseconds before smoothing completes
   * @param _sampleRate The current Sample Rate in Hertz (Hz)
   * @param _linearSmoothing Set True for linear smoothing, or False for logarithmic smoothing */
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
   * @param _inPut Current value being smoothed
   * @return The smoothed value */
  T smoothe(T _inPut)
  {
    if (linearSmoothing)
    {
      if (_inPut == outPut)
        return outPut;
      else if (_inPut > outPut)
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

private:
  T outPut, a, b;
  T linearIncrement, minValue, maxValue;
  bool linearSmoothing;
};

/* A circular buffer class of "numChans" channels */
template <class T>
class CircularBuffer
{
public:
  /* Default constructor */
  CircularBuffer()
  {
    currPosition = 0;
    buffer.clear();
  }

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
    uint32_t size = buffer.size();
    
    buffer.clear();
    buffer.resize(size, 0);
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

  /* Returns the current size of the buffer
   * @return The current size of the buffer (typically in samples) */
  uint32_t getSize() const { return buffer.size(); }

private:
  std::vector<T> buffer;
  uint32_t currPosition;
};

//*****************************************************************************************************************************************
/* Trace an image from an SVG onto a Graphics context
 * @param _qui Graphics context
 * @param _svg The SVG image to be traced out
 * @param _bounds Bounds to trace image to
 * @param _c Original color of the image */
static void traceOut(IGraphics& _qui, const ISVG& _svg, const IRECT& _bounds, const IColor& _c)
{
  NSVGshape* shape = _svg.mImage->shapes;

  while (shape)
  {
    NSVGpath* path = shape->paths;
    float strokeW = shape->strokeWidth;
    float opa = shape->opacity;

    while (path)
    {
      float* test = path->pts;
      const int N = path->npts;

      _qui.PathMoveTo(test[0] + _bounds.L, test[1] + _bounds.T);

      for (int k = 0; k < N * 2; k += 2)
      {
        float x1 = test[k] + _bounds.L;
        float y1 = test[k + 1] + _bounds.T;

        _qui.PathLineTo(x1, y1);
      }

      _qui.PathStroke(_c.WithOpacity(opa), strokeW);
      path = path->next;
    }

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
