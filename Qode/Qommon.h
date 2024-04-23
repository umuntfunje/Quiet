#pragma once

/* Most Qontrols use this... */
#include "IControl.h"
#include "Qtilities/Qheme.h"
#include "Qtilities/Qonstants.h"
#include "Qtilities/Smoothing.h"

using namespace iplug;
using namespace igraphics;

//*****************************************************************************************************************************************
using DrawQutton = std::function<void(IGraphics&, const IRECT&, const IColor&, float)>;

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
static std::pair<float, float> inverted(const std::pair<float, float>& _point, const IRECT& _bounds)
{
  return {_bounds.R - _point.first + _bounds.L, _bounds.B - _point.second + _bounds.T};
}

/* Normalizes a value from old range (between minimum & maximum)
 * @param _value Value being mapped (should be between minimum & maximum)
 * @param _min Lower bound of range
 * @param _max Upper bound of range
 * @return Normalized value (between 0 and 1) */
template <class T>
T normalize(T _value, T _min, T _max)
{
  return (_value - _min) / (_max - _min);
}

/* Maps a value to a new range (between new minimum & maximum) from old range (between old minimum & maximum)
 * @param _value Value being mapped (should be between old minimum & maximum)
 * @param _oldMin Lower bound of old range
 * @param _oldMax Upper bound of old range
 * @param _newMin Lower bound of new range
 * @param _newMax Upper bound of new range
 * @return Mapped value */
template <class T>
T map(T _value, T _oldMin, T _oldMax, T _newMin, T _newMax)
{
  return Lerp(_newMin, _newMax, normalize(_value, _oldMin, _oldMax));
}