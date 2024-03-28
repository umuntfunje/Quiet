#include "Sqope.h"

//*****************************************************************************************************************************************
Sqope::Sqope(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, const double& _phase, const bool& _showSidechain,
  sampleBuffer _ins, sampleBuffer _outs, sampleBuffer _sc)
  : IControl(_bounds, _paramIdx)
  , qheme(_qheme)
  , phase(_phase)
  , showSidechain(_showSidechain)
  , ins(_ins)
  , outs(_outs)
  , sc(_sc)
{
  mIgnoreMouse = true;
}

//*****************************************************************************************************************************************
void Sqope::Draw(IGraphics& _qui)
{
  if (showSqope())
    drawSqope(_qui);

  drawSeeq(_qui);
}

//*****************************************************************************************************************************************
bool Sqope::showSqope() const
{
  return GetValue() >= .5;
}

//*****************************************************************************************************************************************
void Sqope::drawSeeq(IGraphics& _qui)
{
  float cx = mRECT.L + mRECT.W() * static_cast<float>(phase);

  _qui.DrawLine(qheme.foreGround.WithOpacity(.54f), cx, mRECT.T, cx, mRECT.B, 0, thiQness);
}

//*****************************************************************************************************************************************
void Sqope::drawSqope(IGraphics& _qui)
{
  float x = mRECT.L, y = mRECT.T;
  float w = mRECT.W(), h = mRECT.H();

  auto getY = [h](int _x, float _top, float _h, sampleBuffer _buffer) {
    float v = _buffer[(maxResolution + _x) % maxResolution];

    v = (v + 1) / 2; // normalizing btw min and max (from -1 to 1 in our case)

    return _top + h - (v * h); // return y coordinate
  };

  // Input waveform
  _qui.PathMoveTo(x, getY(0, y, h, ins));

  for (int i = 0; i < maxResolution; i++)
  {
    float vx = x + (static_cast<float>(i) / (maxResolution - 1)) * w;
    float vyi = getY(i, y, h, ins);

    _qui.PathLineTo(vx, vyi);
  }

  _qui.PathStroke(qheme.qrey, 1, IStrokeOptions());


  // Output waveform
  _qui.PathMoveTo(x, getY(0, y, h, outs));

  for (int j = 0; j < maxResolution; j++)
  {
    float vx = x + (static_cast<float>(j) / (maxResolution - 1)) * w;
    float vy = getY(j, y, h, outs);

    _qui.PathLineTo(vx, vy);
  }

  _qui.PathStroke(qheme.foreGround, 1, IStrokeOptions());


  // Sidechain waveform
  if (showSidechain)
  {
    _qui.PathMoveTo(x, getY(0, y, h, sc));

    for (int k = 0; k < maxResolution; k++)
    {
      float vx = x + (static_cast<float>(k) / (maxResolution - 1)) * w;
      float vy = getY(k, y, h, sc);

      _qui.PathLineTo(vx, vy);
    }

    _qui.PathStroke(COLOR_ORANGE, 1, IStrokeOptions());
  }
}