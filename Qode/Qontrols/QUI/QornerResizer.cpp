#include "QornerResizer.h"

//*****************************************************************************************************************************************
QornerResizer::QornerResizer(const IRECT& _bounds, const Qheme& _qheme)
  : ICornerResizerControl(_bounds, 18)
  , qheme(_qheme)
{}

//*****************************************************************************************************************************************
void QornerResizer::Draw(IGraphics& _qui)
{
  if (GetUI()->GetResizingInProcess())
    drawResizer(_qui, qheme.foreGround, true);
  else if (GetMouseIsOver())
    drawResizer(_qui, qheme.foreGround);
  else
    drawResizer(_qui, qheme.foreGround.WithOpacity(0.67f));
}

//*****************************************************************************************************************************************
void QornerResizer::drawResizer(IGraphics& _qui, const IColor& _color, bool _isDragging)
{
  const IRECT r = mRECT.GetPadded(2, 2, -2, -2);
  float size = r.W() < r.H() ? r.W() / 3 : r.H() / 3;

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if (i + j < 2)
        continue;

      if (_isDragging)
        _qui.DrawRoundRect(_color, IRECT::MakeXYWH(r.L + size * i, r.T + size * j, size, size).GetScaledAboutCentre(0.6f), 0.5f, 0, 0.73f);
      else
        _qui.FillRoundRect(_color, IRECT::MakeXYWH(r.L + size * i, r.T + size * j, size, size).GetScaledAboutCentre(0.6f), 0.5f);
    }
  }
}