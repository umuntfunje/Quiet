#pragma once

#include "../../Qheme.h"

//*****************************************************************************************************************************************
class QuttonA : public IControl
{
public:
  //***************************************************************************************************************************************
  QuttonA(const IRECT& _bounds, const Qheme& _qheme, const char* _ch, IActionFunction _actionFunq, float _textSize = 20.f);

  void Draw(IGraphics& _qui) override;

  void OnMouseOver(float, float, const IMouseMod&) override;
  void OnMouseDown(float, float, const IMouseMod& _mod) override;
  void OnMouseOut() override;

private:
  //***************************************************************************************************************************************
  const char* ch;
  const Qheme& qheme;
  const float textSize;

  //***************************************************************************************************************************************
  float glow = .73f;
  bool isRising = false;
  Smoothing<float> glowSmoother{glow, .251f, 44100.f, true, glow};
};


//*****************************************************************************************************************************************
class QuttonB : public IControl
{
public:
  //***************************************************************************************************************************************
  QuttonB(const IRECT& _bounds, const Qheme& _qheme, const DrawQutton& _quttonFunq, IActionFunction _actionFunq);

  void Draw(IGraphics& _qui) override;

  void OnMouseOver(float, float, const IMouseMod&) override;
  void OnMouseDown(float, float, const IMouseMod& _mod) override;
  void OnMouseOut() override;

private:
  //***************************************************************************************************************************************
  const Qheme& qheme;
  const DrawQutton quttonFunq;

  //***************************************************************************************************************************************
  float grow = .88f;
  bool isRising = false;
  Smoothing<float> growSmoother{grow, .251f, 44100.f, true, grow};
};