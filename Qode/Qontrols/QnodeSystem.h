#pragma once

#include "../Qurve.h"
#include "../Qheme.h"

//*****************************************************************************************************************************************
class QnodeSystem : public IControl
{
public:
  //***************************************************************************************************************************************
  QnodeSystem(const IRECT& _bounds, Qurve& _qurve, const Qheme& _qheme, IActionFunction _actionFunq);

  void Draw(IGraphics&) override;
  
  void OnMouseOver(float, float, const IMouseMod&) override;
  void OnMouseDown(float, float, const IMouseMod&) override;
  void OnMouseDrag(float, float, float, float, const IMouseMod&) override;
  void OnMouseUp(float, float, const IMouseMod&) override;
  void OnMouseOut() override;
  void OnMouseDblClick(float, float, const IMouseMod&) override;

  void setSnap(bool);
  void setGridSize(int);

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
  void shadeSeqt(IGraphics&);
  void drawQurve(IGraphics&);
  void drawQnodes(IGraphics&);

  float getMinX(int) const;
  float getMaxX(int) const;

  void snapToGrid(float&);
  bool isFineQontrol(const IMouseMod&) const;

  IRECT pairToRect(const std::pair<float, float>&) const;
  IRECT getQurveSeqt(int) const;
};