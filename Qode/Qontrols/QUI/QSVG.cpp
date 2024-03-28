#include "QSVG.h"

//*****************************************************************************************************************************************
QSVG::QSVG(const IRECT& _bounds, const Qheme& _qheme, const ISVG& _svg, IActionFunction _actionFunq, bool _trace, bool _qolorA)
  : IControl(_bounds, _actionFunq)
  , svg(_svg)
  , qheme(_qheme)
  , trace(_trace)
  , qolorA(_qolorA)
{
  if (_actionFunq)
    mDblAsSingleClick = true;
  else
    mIgnoreMouse = true;
}

//*****************************************************************************************************************************************
void QSVG::Draw(IGraphics& _qui)
{
  WDL_String s;
  const IColor c = qolorA ? qheme.foreGround : qheme.backGround;

  if (trace)
    traceOut(_qui, svg, mRECT, c);
  else
    _qui.DrawSVG(svg, mRECT);
}

//*****************************************************************************************************************************************
void QSVG::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
    SetDirty();
}