#pragma once

#include "../Qheme.h"

//*****************************************************************************************************************************************
class Qoggle : public IControl
{
public:
  //***************************************************************************************************************************************
  Qoggle(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, const char* _on, const char* _off, bool _asPopupQenu = false, char _side = 'L');

  Qoggle(const IRECT& _bounds, const Qheme& _qheme, bool _defaultValue, const char* _on, const char* _off, IActionFunction _actionFunq);

  void Draw(IGraphics&) override;
  void OnPopupMenuSelection(IPopupMenu*, int) override;

  void OnMouseOver(float, float, const IMouseMod&) override;
  void OnMouseDown(float, float, const IMouseMod&) override;
  void OnMouseOut() override;

private:
  //***************************************************************************************************************************************
  const char side;
  const Qheme& qheme;
  bool menuOn = false;
  const char *on, *off;

  //***************************************************************************************************************************************
  float glow = .63f;
  bool isRising = false;
  Smoothing<float> glowSmoother{glow, .251f, 44100.f, true, glow};

  //***************************************************************************************************************************************
  bool isOn(bool _isMenu = false) const;
};