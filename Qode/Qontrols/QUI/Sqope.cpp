#include "Sqope.h"

//*****************************************************************************************************************************************
Sqope::Sqope(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, const double& _phase, const bool& _showSidechain, const bool& _avoidInputs,
  const float* _inputBuffer, const float* _ouputBuffer, const float* _sidechainBuffer)
  : IControl(_bounds, _paramIdx)
  , qheme(_qheme)
  , phase(_phase)
  , showSidechain(_showSidechain)
  , avoidInputs(_avoidInputs)
  , inputBuffer(_inputBuffer)
  , outputBuffer(_ouputBuffer)
  , sidechainBuffer(_sidechainBuffer)
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
  return GetValue() >= 0.5;
}

//*****************************************************************************************************************************************
void Sqope::drawSeeq(IGraphics& _qui) const
{
  float cx = mRECT.L + mRECT.W() * static_cast<float>(phase);

  _qui.DrawLine(qheme.foreGround.WithOpacity(0.54f), cx, mRECT.T, cx, mRECT.B, 0, strokeWidth * 0.75f);
}

//*****************************************************************************************************************************************
void Sqope::drawSqope(IGraphics& _qui) const
{
  float left = mRECT.L;
  float top = mRECT.T;
  float width = mRECT.W();
  float height = mRECT.H();

  auto getY = [top, height](int _index, const float* _buffer) {
    float val = _buffer[(maxResolution + _index) % maxResolution];

    val = (val + 1) / 2; // normalizing btw min and max (from -1 to 1 in our case)

    return top + height - (val * height); // return y coordinate
  };

  // Input waveform
  if (avoidInputs == false)
  {
    _qui.PathMoveTo(left, getY(0, inputBuffer));

    for (int i = 0; i < maxResolution; i++)
    {
      float vx = left + i / static_cast<float>(maxResolution - 1) * width;
      float vy = getY(i, inputBuffer);

      _qui.PathLineTo(vx, vy);
    }

    _qui.PathStroke(qheme.grayGround, 1, IStrokeOptions());
  }


  // Output waveform
  _qui.PathMoveTo(left, getY(0, outputBuffer));

  for (int j = 0; j < maxResolution; j++)
  {
    float vx = left + j / static_cast<float>(maxResolution - 1) * width;
    float vy = getY(j, outputBuffer);

    _qui.PathLineTo(vx, vy);
  }

  _qui.PathStroke(qheme.foreGround, 1, IStrokeOptions());


  // Sidechain waveform
  if (showSidechain)
  {
    _qui.PathMoveTo(left, getY(0, sidechainBuffer));

    for (int k = 0; k < maxResolution; k++)
    {
      float vx = left + k / static_cast<float>(maxResolution - 1) * width;
      float vy = getY(k, sidechainBuffer);

      _qui.PathLineTo(vx, vy);
    }

    _qui.PathStroke(qheme.type == Qheme::QhemeType::red || qheme.type == Qheme::QhemeType::light ? COLOR_BLUE : COLOR_WHITE, 1, IStrokeOptions());
  }
}