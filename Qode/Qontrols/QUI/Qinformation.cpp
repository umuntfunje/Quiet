#include "Qinformation.h"
#include "config.h"

//*****************************************************************************************************************************************
Qinformation::Qinformation(const IRECT& _bounds, const Qheme& _qheme)
  : IControl(_bounds)
  , qheme(_qheme)
{
  mDblAsSingleClick = true;
}

//*****************************************************************************************************************************************
void Qinformation::Draw(IGraphics& _qui)
{
  const ISVG quietLogo = _qui.LoadSVG(QUIETLOGO_SVG);
  const IRECT r = mRECT.GetPadded(-100);
  const IText qText(15, EAlign::Center, qheme.foreGround);
  const IRECT uNjeBounds = r.GetPadded(-452, -3, -13, -177).GetPadded(-1);

  _qui.FillRect(qheme.backGround.WithOpacity(0.8173f), mRECT);
  _qui.FillRoundRect(qheme.grayGround.WithOpacity(0.543f), r, 3);

  traceOut(_qui, quietLogo, r.GetPadded(-191, 0, -180, -150), qheme.foreGround);
  _qui.DrawEllipse(qheme.foreGround.WithOpacity(0.67f), uNjeBounds.MW(), uNjeBounds.MH(), uNjeBounds.W() / 2, uNjeBounds.H() / 4, 0, 0, 5);
  
  _qui.DrawText(qText.WithSize(18), PLUG_NAME " v" PLUG_VERSION_STR, r.GetPadded(0, -50, 0, -140));
  _qui.DrawText(qText, PLUG_COPYRIGHT_STR, r.GetPadded(0, -75, 0, -120));

  _qui.DrawText(qText.WithAlign(EAlign::Near), "Made with iPlug2", r.GetPadded(-10, -139, -325, -54));
  _qui.DrawText(qText.WithAlign(EAlign::Near), "VST Plugin Technology by Steinberg", r.GetPadded(-10, -161, -255, -32));
  _qui.DrawText(qText.WithAlign(EAlign::Near), "TO DO: Audio Unit", r.GetPadded(-10, -183, -245, -10));
}

//*****************************************************************************************************************************************
void Qinformation::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
    if (!IsHidden())
      Hide(true);
}