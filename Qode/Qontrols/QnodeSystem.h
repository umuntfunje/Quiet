#pragma once

#include "Qommon.h"
#include "DSP/Qurve.h"

//*****************************************************************************************************************************************
class QnodeSystem : public IControl
{
public:
  //***************************************************************************************************************************************
  QnodeSystem(const IRECT& _bounds, Qurve& _qurve, const Qheme& _qheme, IActionFunction _actionFunq);

  void Draw(IGraphics&) override;
  
  void OnMouseOver(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseDrag(float _x, float _y, float _dX, float _dY, const IMouseMod& _mod) override;
  void OnMouseUp(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseOut() override;
  void OnMouseDblClick(float _x, float _y, const IMouseMod& _mod) override;

  void setSnap(bool _snap);
  void setGridSize(int _gridSize);

private:
  //***************************************************************************************************************************************
  Qurve& qurve;
  const Qheme& qheme;
  Qurve::QurveState tempState;
  bool fromMouseDrag = false;

  float qnodeDiameter;
  int overQnodeIndex = -1;
  int downQnodeIndex = -1;

  int overQurveIndex = -1;
  int downQurveIndex = -1;

  bool snap = true;
  int gridSize = 8;

  //***************************************************************************************************************************************
  void shadeSeqt(IGraphics& _qui);
  void drawQurve(IGraphics& _qui);
  void drawQnodes(IGraphics& _qui);

  float getMinX(int _index) const;
  float getMaxX(int _index) const;

  void snapToGrid(float& _value);

  IRECT pairToRect(const std::pair<float, float>& _pair) const;
  IRECT getQurveSeqt(int _index) const;
};