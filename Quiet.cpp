#include "Quiet.h"

#include "Qontrols/Qnob.h"
#include "Qontrols/Qlider.h"
#include "Qontrols/Qoggle.h"
#include "Qontrols/UndoQutton.h"
#include "Qontrols/RedoQutton.h"
#include "Qontrols/QnodeSystem.h"
#include "Qontrols/QurvePresets/Qenu.h"
#include "Qontrols/QurvePresets/Qutton.h"

#include "Qontrols/QUI/Qrid.h"
#include "Qontrols/QUI/QSVG.h"
#include "Qontrols/QUI/Sqope.h"
#include "Qontrols/QUI/Qabel.h"
#include "Qontrols/QUI/Qubble.h"
#include "Qontrols/QUI/Baqground.h"
#include "Qontrols/QUI/Qinformation.h"
#include "Qontrols/QUI/QornerResizer.h"

#include "IPopupMenuControl.h"
#include "IPlug_include_in_plug_src.h"

//*****************************************************************************************************************************************
Quiet::Quiet(const InstanceInfo& _qinfo)
: Plugin(_qinfo, MakeConfig(qNumParams, qNumPresets))
{
  GetParam(qSnap)->InitInt("snap", 8, 0, 16);

  GetParam(qWaveform)->InitBool("waveforms", true);
  GetParam(qTheme)->InitEnum("theme", 0, 5, "", 0, "", "default", "green", "yellow", "light", "dark");
  GetParam(qLookahead)->InitEnum("lookahead", 0, 5, "", 0, "", "  0 ms", "10 ms", "20 ms", "30 ms", "40 ms");
  GetParam(qVelocity)->InitBool("velocity", false);

  GetParam(qRate)->InitEnum("rate", 12, _rates);
  GetParam(qFrequency)->InitDouble("frequency", 20., .1, 40., .001, "Hz");
  GetParam(qPhaseShift)->InitInt("phase", 0, 0, 360, "°");

  GetParam(qDepth)->InitDouble("depth", 100., .0, 100., .001);
  GetParam(qDepth)->SetDisplayPrecision(0);

  GetParam(qSmoothing)->InitDouble("smoothing", .0, .0, 50., .01, "ms");
  GetParam(qSmoothing)->SetDisplayPrecision(1);

  GetParam(qSync)->InitBool("sync mode", false, "", 0, "", "tempo", "free");
  GetParam(qMIDI)->InitBool("midi retrigger", false);

  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* _qui) {
    const IRECT bounds = _qui->GetBounds();
    const ISVG quietLogo = _qui->LoadSVG(QUIETLOGO_SVG);
    const ISVG boxShadow = _qui->LoadSVG(BOXSHADOW_SVG);
    const ISVG fadeShadow = _qui->LoadSVG(FADESHADOW_SVG);

    _qui->EnableMouseOver(true);
    _qui->AttachControl(new Baqground(bounds, qheme), qCtrlTagBaq);
    _qui->LoadFont("Roboto-Regular", ROBOTO_FN);
    _qui->AttachControl(new QSVG(bounds.GetPadded(0, -33, -580, -346).GetPadded(-2), qheme, quietLogo, [_qui](IControl*) {
      // Display About Info
      _qui->GetControlWithTag(qCtrlTagInfo)->Hide(false);
    }, true));


    // Menu theme initialize
    updateQenuTheme(_qui);


    // Main Window (460 x 270)
    const IRECT windowBounds = bounds.GetPadded(-120, -35, -120, -110);
    _qui->AttachControl(new Qrid(windowBounds, qheme, qSnap));
    _qui->AttachControl(new Sqope(windowBounds, qheme, qWaveform, qp.getPhase(), showSidechain, qp.getIns(), qp.getOuts(), qp.getSc()), qCtrlTagSqope);
    _qui->AttachControl(new QnodeSystem(windowBounds, qurve, qheme, [&](IControl*) {
      const IGraphics* ui = GetUI();

      updateUndoRedoQuttons(ui);

      std::string name = qurve.getQurveName();

      if (addAsterisk(name))
      {
        qurve.setQurveName(name.c_str());
        ui->GetControlWithTag(qCtrlTagQenu)->As<Qenu>()->setPresetName(name.c_str(), true);
      }
    }), qCtrlTagQnodeSys);
    _qui->AttachControl(new QSVG(windowBounds.GetPadded(1), qheme, boxShadow, nullptr, true, false));


    // Undo, Redo
    const IRECT undoBounds = bounds.GetPadded(-173, -6.5f, -505, -386.5f);
    _qui->AttachControl(new UndoQutton(undoBounds.GetPadded(-1), qheme, [&](IControl*) {
      const IGraphics* ui = GetUI();

      qurve.undoCurrentState();
      updateUndoRedoQuttons(ui);

      std::string name = qurve.getQurveName();

      if (addAsterisk(name))
      {
        qurve.setQurveName(name.c_str());
        ui->GetControlWithTag(qCtrlTagQenu)->As<Qenu>()->setPresetName(name.c_str(), true);
      }

      ui->GetControlWithTag(qCtrlTagQnodeSys)->SetDirty(false);
    }), qCtrlTagUndo);

    const IRECT redoBounds = bounds.GetPadded(-194, -6.5f, -482, -386.5f);
    _qui->AttachControl(new RedoQutton(redoBounds.GetPadded(-1), qheme, [&](IControl*) {
      const IGraphics* ui = GetUI();

      qurve.redoCurrentState();
      updateUndoRedoQuttons(ui);

      std::string name = qurve.getQurveName();

      if (addAsterisk(name))
      {
        qurve.setQurveName(name.c_str());
        ui->GetControlWithTag(qCtrlTagQenu)->As<Qenu>()->setPresetName(name.c_str(), true);
      }

      ui->GetControlWithTag(qCtrlTagQnodeSys)->SetDirty(false);
    }), qCtrlTagRedo);

    updateUndoRedoQuttons(GetUI());


    // Preset Open/Save menu (460 x 22)
    const IRECT presetsBounds = bounds.GetPadded(-120, -6.5f, -120, -386.5f);

    _qui->AttachControl(new QuttonA(presetsBounds.GetPadded(-115, 0, -320, 0).GetPadded(-1), qheme, "<", [_qui](IControl*) {
      // Previous preset
      _qui->GetControlWithTag(qCtrlTagQenu)->As<Qenu>()->loadPreviousPreset();
    }));

    _qui->AttachControl(new Qenu(presetsBounds.GetHPadded(-140).GetPadded(-1), qurve, qheme, [&](IControl*) {
      const IGraphics* ui = GetUI();

      updateUndoRedoQuttons(ui);
      ui->GetControlWithTag(qCtrlTagQnodeSys)->SetDirty(false);
    }), qCtrlTagQenu);

    _qui->AttachControl(new QuttonA(presetsBounds.GetPadded(-320, 0, -115, 0).GetPadded(-1), qheme, ">", [_qui](IControl*) {
      // Next preset
      _qui->GetControlWithTag(qCtrlTagQenu)->As<Qenu>()->loadNextPreset();
    }));

    auto saveQutton = [](IGraphics& _qui, const IRECT& _bounds, const IColor& _color, float _smoothGrow) {
      float w = _bounds.GetLengthOfShortestSide();
      const IRECT r = IRECT::MakeXYWH(_bounds.MW() - w / 2, _bounds.MH() - w / 2, w, w).GetScaledAboutCentre(_smoothGrow);
      float spac = r.W() / 5;
      float thickness = .817f;

      _qui.DrawLine(_color, r.L, r.T, r.R - spac, r.T, 0, thickness);
      _qui.DrawLine(_color, r.R - spac, r.T, r.R, r.T + spac, 0, thickness);
      _qui.DrawLine(_color, r.R, r.T + spac, r.R, r.B, 0, thickness);
      _qui.DrawLine(_color, r.R, r.B, r.L, r.B, 0, thickness);
      _qui.DrawLine(_color, r.L, r.B, r.L, r.T, 0, thickness);

      _qui.DrawLine(_color, r.L + spac, r.T, r.L + spac, r.T + r.H() / 3.f, 0, thickness);
      _qui.DrawLine(_color, r.L + spac, r.T + r.H() / 3.f, r.R - spac, r.T + r.H() / 3.f, 0, thickness);
      _qui.DrawLine(_color, r.R - spac, r.T + r.H() / 3.f, r.R - spac, r.T, 0, thickness);

      _qui.DrawLine(_color, r.L + r.W() / 9, r.B, r.L + r.W() / 9, r.B - r.H() / 2.5f, 0, thickness);
      _qui.DrawLine(_color, r.L + r.W() / 9, r.B - r.H() / 2.5f, r.R - r.W() / 9, r.B - r.H() / 2.5f, 0, thickness);
      _qui.DrawLine(_color, r.R - r.W() / 9, r.B - r.H() / 2.5f, r.R - r.W() / 9, r.B, 0, thickness);
    };
    _qui->AttachControl(new QuttonB(presetsBounds.GetPadded(-356, 0, -81, 0).GetPadded(-2.5f), qheme, saveQutton, [_qui](IControl*) {
      // Save preset
      _qui->GetControlWithTag(qCtrlTagQenu)->As<Qenu>()->savePreset();
    }));


    // Utilities LEFT
    const IRECT utilBoundsL = bounds.GetPadded(-90, -35, -585, -255);

    auto randomizeQutton = [](IGraphics& _qui, const IRECT& _bounds, const IColor& _color, float _smoothGrow) {
      float l = _bounds.GetLengthOfShortestSide();
      const IRECT r = IRECT::MakeXYWH(_bounds.MW() - l / 2, _bounds.MH() - l / 2, l, l).GetScaledAboutCentre(_smoothGrow);
      float size = r.W() / 10;
      float thickness = .81f;
      const IColor c = _color.WithOpacity(thickness);

      _qui.DrawLine(_color, r.L + size, r.T + size * 2.5f, r.MW(), r.T + size, 0, thickness);
      _qui.DrawLine(_color, r.MW(), r.T + size, r.R - size, r.T + size * 2.5f, 0, thickness);
      _qui.DrawLine(_color, r.L + size, r.T + size * 2.5f, r.L + size, r.B - size * 2.5f, 0, thickness);
      _qui.DrawLine(_color, r.L + size, r.B - size * 2.5f, r.MW(), r.B - size, 0, thickness);
      _qui.DrawLine(_color, r.MW(), r.B - size, r.R - size, r.B - size * 2.5f, 0, thickness);
      _qui.DrawLine(_color, r.R - size, r.B - size * 2.5f, r.R - size, r.T + size * 2.5f, 0, thickness);

      _qui.DrawLine(_color, r.L + size, r.T + size * 2.5f, r.MW(), r.T + size * 4, 0, thickness);
      _qui.DrawLine(_color, r.MW(), r.T + size * 4, r.R - size, r.T + size * 2.5f, 0, thickness);
      _qui.DrawLine(_color, r.MW(), r.T + size * 4, r.MW(), r.B - size, 0, thickness);

      _qui.FillEllipse(c, r.MW(), r.T + size * 2.5f, size / 2, size / 3, .0f);

      _qui.FillEllipse(c, r.L + size * 2, r.T + size * 4, size / 2, size / 2.5f, 60.f);
      _qui.FillEllipse(c, r.L + size * 4, r.B - size * 2.5f, size / 2, size / 2.5f, 60.f);

      _qui.FillEllipse(c, r.R - size * 2, r.T + size * 4, size / 2, size / 2.5f, -60.f);
      _qui.FillEllipse(c, r.R - size * 4, r.T + size * 5, size / 2, size / 2.5f, -60.f);
      _qui.FillEllipse(c, r.R - size * 2, r.B - size * 3.5f, size / 2, size / 2.5f, -60.f);
      _qui.FillEllipse(c, r.R - size * 4, r.B - size * 2.5f, size / 2, size / 2.5f, -60.f);
    };
    _qui->AttachControl(new QuttonB(utilBoundsL.GetPadded(-1, -51, -1, -51), qheme, randomizeQutton, [&](IControl*) {
      const IGraphics* ui = GetUI();

      qurve.updateCurrentState();
      qurve.randomizeQurve();
      updateUndoRedoQuttons(ui);

      std::string name = qurve.getQurveName();

      if (addAsterisk(name))
      {
        qurve.setQurveName(name.c_str());
        ui->GetControlWithTag(qCtrlTagQenu)->As<Qenu>()->setPresetName(name.c_str(), true);
      }

      ui->GetControlWithTag(qCtrlTagQnodeSys)->SetDirty(false);
    }));

    _qui->AttachControl(new Qoggle(utilBoundsL.GetPadded(-1, -76, -1, -26), qheme, qTheme, "T", "T", true));

    _qui->AttachControl(new Qoggle(utilBoundsL.GetPadded(-1, -101, -1, -1), qheme, qWaveform, "W", "W"));


    // Utilities RIGHT
    const IRECT utilBoundsR = bounds.GetPadded(-585, -35, -90, -255);

    auto smoothQQutton = [](IGraphics& _qui, const IRECT& _bounds, const IColor& _color, float _smoothGrow) {
      float l = _bounds.GetLengthOfShortestSide();
      const IRECT r = IRECT::MakeXYWH(_bounds.MW() - l / 2, _bounds.MH() - l / 2, l, l).GetScaledAboutCentre(_smoothGrow);
      float size = 3 * r.W() / 9;
      const IRECT tR = r.GetFromTRHC(size, size).GetFromBLHC(size / 1.7f, size / 1.7f); // topRight
      const IRECT bL = r.GetFromBLHC(size, size).GetFromTRHC(size / 1.7f, size / 1.7f); // bottomLeft
      float thickness = r.W() / 20;
      float sineLike = thickness / 2;

      _qui.FillRoundRect(_color.WithOpacity(.0673f), _bounds.GetScaledAboutCentre(.94f), 3.f);

      _qui.PathMoveTo(tR.L, tR.MH());
      _qui.PathQuadraticBezierTo((tR.L + bL.R) / 2 - sineLike, tR.MH() - sineLike, (tR.L + bL.R) / 2, (tR.B + bL.T) / 2);
      _qui.PathQuadraticBezierTo((tR.L + bL.R) / 2 + sineLike, bL.MH() + sineLike, bL.R, bL.MH());
      _qui.PathStroke(_color, thickness);

      _qui.DrawRoundRect(_color, tR, .67f, 0, thickness);
      _qui.DrawRoundRect(_color, bL, .67f, 0, thickness);
    };
    _qui->AttachControl(new QuttonB(utilBoundsR.GetPadded(-1, -1, -1, -101), qheme, smoothQQutton, [&](IControl*) {
      // smooth
      if (qurve.getSmooth())
        return;

      const IGraphics* ui = GetUI();

      qurve.updateCurrentState();
      qurve.setSmooth(true);
      qurve.qreateQurve();
      updateUndoRedoQuttons(ui);

      std::string name = qurve.getQurveName();

      if (addAsterisk(name))
      {
        qurve.setQurveName(name.c_str());
        ui->GetControlWithTag(qCtrlTagQenu)->As<Qenu>()->setPresetName(name.c_str(), true);
      }

      ui->GetControlWithTag(qCtrlTagQnodeSys)->SetDirty(false);
    }));

    auto linearQQutton = [&](IGraphics& _qui, const IRECT& _bounds, const IColor& _color, float _smoothGrow) {
      float l = _bounds.GetLengthOfShortestSide();
      const IRECT r = IRECT::MakeXYWH(_bounds.MW() - l / 2, _bounds.MH() - l / 2, l, l).GetScaledAboutCentre(_smoothGrow);
      float size = 3 * r.W() / 9;
      const IRECT tR = r.GetFromTRHC(size, size).GetFromBLHC(size / 1.7f, size / 1.7f); // topRight
      const IRECT bL = r.GetFromBLHC(size, size).GetFromTRHC(size / 1.7f, size / 1.7f); // bottomLeft
      float thickness = r.W() / 20;

      _qui.FillRoundRect(_color.WithOpacity(.0673f), _bounds.GetScaledAboutCentre(.94f), 3.f);

      _qui.DrawLine(_color, tR.L, tR.B, bL.R, bL.T, 0, thickness);
      _qui.DrawRoundRect(_color, tR, .67f, 0, thickness);
      _qui.DrawRoundRect(_color, bL, .67f, 0, thickness);
    };
    _qui->AttachControl(new QuttonB(utilBoundsR.GetPadded(-1, -26, -1, -76), qheme, linearQQutton, [&](IControl*) {
      // linear
      if (!qurve.getSmooth())
        return;

      const IGraphics* ui = GetUI();

      qurve.updateCurrentState();
      qurve.setSmooth(false);
      qurve.qreateQurve();
      updateUndoRedoQuttons(ui);

      std::string name = qurve.getQurveName();

      if (addAsterisk(name))
      {
        qurve.setQurveName(name.c_str());
        ui->GetControlWithTag(qCtrlTagQenu)->As<Qenu>()->setPresetName(name.c_str(), true);
      }

      ui->GetControlWithTag(qCtrlTagQnodeSys)->SetDirty(false);
    }));

    auto flipVQutton = [](IGraphics& _qui, const IRECT& _bounds, const IColor& _color, float _smoothGrow) {
      float l = _bounds.GetLengthOfShortestSide();
      const IRECT r = IRECT::MakeXYWH(_bounds.MW() - l / 2, _bounds.MH() - l / 2, l, l).GetScaledAboutCentre(_smoothGrow);
      float sizeX = r.W() / 20;
      float sizeY = r.H() / 8;

      std::pair<float, float> line1A{r.L + sizeX * 6.5f, r.B - sizeY};
      std::pair<float, float> line1B{line1A.first, r.B - sizeY * 4.7f};
      std::pair<float, float> line2A{inverted(line1A, r)};
      std::pair<float, float> line2B{inverted(line1B, r)};

      _qui.FillRoundRect(_color.WithOpacity(.0673f), _bounds.GetScaledAboutCentre(.94f), 3.f);

      _qui.DrawLine(_color, line1A.first, line1A.second, line1B.first, line1B.second, 0, sizeX);
      _qui.DrawLine(_color, line2A.first, line2A.second, line2B.first, line2B.second, 0, sizeX);

      std::pair<float, float> t1VertxA{r.L + sizeX * 4, r.B - sizeY * 5};
      std::pair<float, float> t1VertxB{r.L + sizeX * 9, t1VertxA.second};
      std::pair<float, float> t1VertxC{line1A.first, r.B - sizeY * 6.5f};
      std::pair<float, float> t2VertxA{inverted(t1VertxA, r)};
      std::pair<float, float> t2VertxB{inverted(t1VertxB, r)};
      std::pair<float, float> t2VertxC{inverted(t1VertxC, r)};

      _qui.DrawTriangle(_color, t1VertxA.first, t1VertxA.second, t1VertxB.first, t1VertxB.second, t1VertxC.first, t1VertxC.second, 0, sizeX);
      _qui.DrawTriangle(_color, t2VertxA.first, t2VertxA.second, t2VertxB.first, t2VertxB.second, t2VertxC.first, t2VertxC.second, 0, sizeX);
    };
    _qui->AttachControl(new QuttonB(utilBoundsR.GetPadded(-1, -51, -1, -51), qheme, flipVQutton, [&](IControl*) {
      // flip
      const IGraphics* ui = GetUI();

      qurve.updateCurrentState();
      qurve.flipVertical();
      updateUndoRedoQuttons(ui);

      std::string name = qurve.getQurveName();

      if (addAsterisk(name))
      {
        qurve.setQurveName(name.c_str());
        ui->GetControlWithTag(qCtrlTagQenu)->As<Qenu>()->setPresetName(name.c_str(), true);
      }

      ui->GetControlWithTag(qCtrlTagQnodeSys)->SetDirty(false);
    }));

    _qui->AttachControl(new Qoggle(utilBoundsR.GetPadded(-1, -76, -1, -26), qheme, qLookahead, "L", "L", true, 'R'), kNoTag, "onMIDI")->SetDisabled(true);

    _qui->AttachControl(new Qoggle(utilBoundsR.GetPadded(-1, -101, -1, -1), qheme, qVelocity, "V", "V"), kNoTag, "onMIDI")->SetDisabled(true);


    auto displayValue = [_qui](IControl* _qaller) {
      WDL_String theValue;
      const IRECT valueBounds = _qaller->GetRECT().GetPadded(13, -21, 13, 16).GetPadded(-1);

      _qaller->GetParam()->GetDisplayWithLabel(theValue);
      _qui->GetBubbleControl()->As<Qubble>()->showQubble(_qaller, valueBounds, theValue.Get());
    };


    // Rate (Tempo) Slider
    const IRECT rateBounds = bounds.GetPadded(0, -205, -580, -170).GetPadded(-1);
    const IRECT rateQabelBounds = bounds.GetPadded(0, -194, -580, -200).GetPadded(-2); // 116 x 17
    _qui->AttachControl(new Qabel(rateQabelBounds, qheme, "rate"), kNoTag, "tempo");
    _qui->AttachControl(new Qlider(rateBounds.GetPadded(-13, -9, -13, -9), qheme, qRate, displayValue), -1, "tempo");


    // Frequency (Free) Slider
    const IRECT freqBounds = bounds.GetPadded(0, -276, -580, -99).GetPadded(-1);
    const IRECT freqQabelBounds = bounds.GetPadded(0, -265, -580, -129).GetPadded(-2); // 116 x 17
    _qui->AttachControl(new Qabel(freqQabelBounds, qheme, "frequency"), kNoTag, "free");
    _qui->AttachControl(new Qlider(freqBounds.GetPadded(-13, -9, -13, -9), qheme, qFrequency, displayValue), -1, "free");


    // Snap Slider
    const IRECT snapBounds = bounds.GetPadded(-580, -205, 0, -170).GetPadded(-1);
    const IRECT snapQabelBounds = bounds.GetPadded(-580, -194, 0, -200).GetPadded(-2);
    _qui->AttachControl(new Qabel(snapQabelBounds, qheme, "snap"));
    _qui->AttachControl(new Qlider(snapBounds.GetPadded(-13, -9, -13, -9), qheme, qSnap, displayValue));


    // Phase Shift Slider
    const IRECT phaseShiftBounds = bounds.GetPadded(-580, -276, 0, -99).GetPadded(-1);
    const IRECT phaseShiftQabelBounds = bounds.GetPadded(-580, -265, 0, -129).GetPadded(-2);
    _qui->AttachControl(new Qabel(phaseShiftQabelBounds, qheme, "phase"));
    _qui->AttachControl(new Qlider(phaseShiftBounds.GetPadded(-13, -9, -13, -9), qheme, qPhaseShift, displayValue));


    // TODO: Filter cuttoff maybe....
    const IRECT nullBounds = bounds.GetPadded(-180, -311, -430, -14);
    _qui->AttachControl(new Qabel(nullBounds.GetPadded(-1, -3, -1, -69), qheme, "null"))->SetDisabled(true);
    _qui->AttachControl(new Qnob(nullBounds.GetPadded(-23), qheme, -1, nullptr))->SetDisabled(true);


    // Depth (Dry/Wet) Knob  (90 x 90)
    const IRECT depthBounds = bounds.GetPadded(-305, -321, -305, -4);
    const IRECT depthQabelBounds = bounds.GetPadded(-305, -312, -305, -86); // 90 x 17
    _qui->AttachControl(new Qabel(depthQabelBounds, qheme, "depth"));
    _qui->AttachControl(new Qnob(depthBounds.GetPadded(-9), qheme, qDepth, [_qui](IControl* _this) {
      WDL_String theValue;
      const IRECT valueBounds = _qui->GetBounds().GetPadded(-305, -390, -305, 0).GetPadded(-4);

      _this->GetParam()->GetDisplayWithLabel(theValue);
      _qui->GetBubbleControl()->As<Qubble>()->showQubble(_this, valueBounds, theValue.Get());
    }));


    // Smooth Qurve
    const IRECT smoothingBounds = bounds.GetPadded(-430, -311, -180, -14);
    _qui->AttachControl(new Qabel(smoothingBounds.GetPadded(-1, -3, -1, -69), qheme, "smooth"));
    _qui->AttachControl(new Qnob(smoothingBounds.GetPadded(-23), qheme, qSmoothing, [_qui](IControl* _this) {
      WDL_String theValue;
      const IRECT valueBounds = _qui->GetBounds().GetPadded(-500, -330, -140, -55).GetPadded(-4);

      _this->GetParam()->GetDisplayWithLabel(theValue);
      _qui->GetBubbleControl()->As<Qubble>()->showQubble(_this, valueBounds, theValue.Get());
    }));


    // Synced/free Qoggle
    const IRECT syncBounds = bounds.GetPadded(-85, -370, -535, -5).GetPadded(-1);
    _qui->AttachControl(new Qoggle(syncBounds.GetPadded(-14, -9, -14, -9), qheme, qSync, "free", "tempo"));


    // Midi Retrigger Qoggle
    const IRECT midiBounds = bounds.GetPadded(-535, -370, -85, -5).GetPadded(-1);
    _qui->AttachControl(new Qoggle(midiBounds.GetPadded(-14, -9, -14, -9), qheme, qMIDI, "midi", "midi"));


    // Other...
    _qui->SetScaleConstraints(.8f, 1.5f);
    _qui->AttachControl(new QSVG(bounds.GetPadded(0, -327, 0, 0), qheme, fadeShadow));
    _qui->AttachCornerResizer(new QornerResizer(bounds, qheme), EUIResizerMode::Scale, false);


    // Information
    _qui->AttachBubbleControl(new Qubble(qheme));
    _qui->AttachControl(new Qinformation(bounds.GetPadded(-1), qheme), qCtrlTagInfo)->Hide(true);
  };
}

//*****************************************************************************************************************************************
void Quiet::OnIdle()
{
  if (transporting)
  {
    const IGraphics* ui = GetUI();

    if (ui)
      ui->GetControlWithTag(qCtrlTagSqope)->SetDirty(false);
  }
}

//*****************************************************************************************************************************************
void Quiet::OnReset()
{
  qp.reset(GetSampleRate(), GetLatency());
}

//*****************************************************************************************************************************************
void Quiet::OnParamChange(int _paramIdx)
{
  if (_paramIdx == qDepth)
    qp.setDepth(GetParam(qDepth)->Value() / 100.);
  else if (_paramIdx == qRate)
    qp.setRate(rateScalars[GetParam(qRate)->Int()]);
  else if (_paramIdx == qFrequency)
  {
    double val = GetParam(qFrequency)->Value();

    if (val < 1.)
      GetParam(qFrequency)->SetDisplayPrecision(2);
    else
      GetParam(qFrequency)->SetDisplayPrecision(1);

    qp.setFrequency((1. / GetSampleRate()) * val);
  }
  else if (_paramIdx == qPhaseShift)
    qp.setPhaseShift(GetParam(qPhaseShift)->Value() / 360.);
  else if (_paramIdx == qSync)
  {
    bool freeRunning = GetParam(qSync)->Bool();

    qp.setTempoSynqd(!freeRunning);

    if (freeRunning)
      qp.setFrequency((1. / GetSampleRate()) * GetParam(qFrequency)->Value());
  }
  else if (_paramIdx == qMIDI)
  {
    bool _MIDITriggered = GetParam(qMIDI)->Bool();

    qp.setMIDITriggered(_MIDITriggered);

    if (_MIDITriggered)
      updateLookAhead();
    else
      turnLookAheadOff(!qp.isLookingAhead());
  }
  else if (_paramIdx == qVelocity)
    qp.setUseVelocity(GetParam(qVelocity)->Bool());
  else if (_paramIdx == qSmoothing)
    qp.setQurveSmoothing(GetParam(qSmoothing)->Value());
  else if (_paramIdx == qLookahead)
  {
    if (GetParam(qMIDI)->Bool()) // Look ahead only with MIDI trigger on...
      updateLookAhead();
  }
}

//*****************************************************************************************************************************************
void Quiet::OnParamChangeUI(int _paramIdx, EParamSource _eps)
{
  if (_paramIdx == qSnap)
  {
    const IGraphics* ui = GetUI();

    if (ui)
    {
      QnodeSystem* qs = ui->GetControlWithTag(qCtrlTagQnodeSys)->As<QnodeSystem>();

      if (qs)
      {
        int lines = GetParam(qSnap)->Int();

        if (lines == 0 || lines == 1)
          qs->setSnap(false);
        else
        {
          qs->setSnap(true);
          qs->setGridSize(lines);
        }
      }
    }
  }
  else if (_paramIdx == qTheme)
  {
    IGraphics* ui = GetUI();

    if (ui)
    {
      IPopupMenuControl* pum = ui->GetPopupMenuControl();

      qheme.updateQheme(GetParam(qTheme)->Int());

      if (pum)
      {
        updateQenuTheme(ui);

        if (pum->GetState() == pum->kExpanded && _eps != EParamSource::kUI)
          pum->CollapseEverything();  // had to make this public...laziness i guess...
      }

      ui->SetAllControlsDirty();
    }
  }
  else if (_paramIdx == qSync)
  {
    IGraphics* ui = GetUI();

    if (ui)
    {
      if (GetParam(qSync)->Bool()) // free run is true!
      {
        ui->ForControlInGroup("free", [](IControl* _qontrol) { _qontrol->SetDisabled(false); });
        ui->ForControlInGroup("tempo", [](IControl* _qontrol) { _qontrol->SetDisabled(true); });
      }
      else
      {
        ui->ForControlInGroup("free", [](IControl* _qontrol) { _qontrol->SetDisabled(true); });
        ui->ForControlInGroup("tempo", [](IControl* _qontrol) { _qontrol->SetDisabled(false); });
      }
    }
  }
  else if (_paramIdx == qMIDI)
  {
    IGraphics* ui = GetUI();

    if (ui)
      ui->ForControlInGroup("onMIDI", [&](IControl* _qontrol) { _qontrol->SetDisabled(!GetParam(qMIDI)->Bool()); });
  }
  else if (_paramIdx == qLookahead)
  {
    IGraphics* ui = GetUI();

    if (ui)
    {
      IPopupMenuControl* pum = ui->GetPopupMenuControl();

      if (pum)
      {
        if (pum->GetState() == pum->kExpanded && _eps != EParamSource::kUI)
          pum->CollapseEverything();
      }
    }
  }
}

//*****************************************************************************************************************************************
void Quiet::ProcessBlock(sample** _inputs, sample** _outputs, int _frames)
{
  bool showScope = GetUI() && GetParam(qWaveform)->Bool();

  showSidechain = IsChannelConnected(ERoute::kInput, 2);
  transporting = qp.processBlock(_inputs, _outputs, _frames, mTimeInfo, showSidechain, showScope);
}

//*****************************************************************************************************************************************
void Quiet::ProcessMidiMsg(const IMidiMsg& _msg)
{
  if (!qp.isMIDITriggered())
    return;

  int status = _msg.StatusMsg();

  switch (status)
  {
  case IMidiMsg::kNoteOn:
    break;
  default:
    return; // if !note message, nothing gets added to the queue
  }

  qp.addMIDIMessage(_msg);
}

//*****************************************************************************************************************************************
bool Quiet::SerializeState(IByteChunk& _chunk) const
{
  std::string qName = qurve.getQurveName();
  int qQnodeCount = qurve.getQnodeCount();
  bool qSmooth = qurve.getSmooth();
  int nameSize = (int)qName.length();

  _chunk.Put(&nameSize);

  for (int i = 0; i < nameSize; i++)
    _chunk.Put(&qName[i]);

  _chunk.Put(&qQnodeCount);
  _chunk.Put(&qSmooth);

  for (int j = 0; j < qQnodeCount; j++)
  {
    float qQurvature = qurve.getQurvature(j);
    std::pair<float, float> qQnode = qurve.getQnode(j);

    _chunk.Put(&qQurvature);
    _chunk.Put(&qQnode);
  }

  return SerializeParams(_chunk);
}

//*****************************************************************************************************************************************
int Quiet::UnserializeState(const IByteChunk& _chunk, int _startPosition)
{
  std::string qName;
  char sentinel;
  int qQnodeCount;
  bool qSmooth;
  int nameSize;

  _startPosition = _chunk.Get(&nameSize, _startPosition);

  for (int i = 0; i < nameSize; i++)
  {
    _startPosition = _chunk.Get(&sentinel, _startPosition);
    qName.push_back(sentinel);
  }

  qurve.setQurveName(qName.c_str());
  _startPosition = _chunk.Get(&qQnodeCount, _startPosition);
  qurve.setQnodeCount(qQnodeCount);
  _startPosition = _chunk.Get(&qSmooth, _startPosition);
  qurve.setSmooth(qSmooth);

  for (int j = 0; j < qQnodeCount; j++)
  {
    float qQurvature;
    std::pair<float, float> qQnode;

    _startPosition = _chunk.Get(&qQurvature, _startPosition);
    qurve.setQurvature(j, qQurvature);
    _startPosition = _chunk.Get(&qQnode, _startPosition);
    qurve.setQnode(j, qQnode);
  }

  qurve.qreateQurve();

  return UnserializeParams(_chunk, _startPosition);
}

//*****************************************************************************************************************************************
void Quiet::updateLookAhead()
{
  switch (GetParam(qLookahead)->Int())
  {
  case 0:
    SetLatency(0); // 0 ms
    qp.setLookAhead(0);
    qp.setLookingAhead(false);
    qurve.setMultiplyQurvature(false);
    break;
  case 1: // 10 ms
    SetLatency(441);
    qp.setLookAhead(441);
    qp.setLookingAhead(true);
    qurve.setMultiplyQurvature(true);
    qurve.setQurvatureMultiplier(1.25f);
    break;
  case 2: // 20 ms
    SetLatency(882);
    qp.setLookAhead(882);
    qp.setLookingAhead(true);
    qurve.setMultiplyQurvature(true);
    qurve.setQurvatureMultiplier(1.5f);
    break;
  case 3: // 30 ms
    SetLatency(1323);
    qp.setLookAhead(1323);
    qp.setLookingAhead(true);
    qurve.setMultiplyQurvature(true);
    qurve.setQurvatureMultiplier(2.f);
    break;
  case 4: // 40 ms
    SetLatency(1764);
    qp.setLookAhead(1764);
    qp.setLookingAhead(true);
    qurve.setMultiplyQurvature(true);
    qurve.setQurvatureMultiplier(2.5f);
    break;
  }
}

//*****************************************************************************************************************************************
void Quiet::turnLookAheadOff(bool _alreadyOff)
{
  if (_alreadyOff)
    return;

  SetLatency(0);
  qp.setLookAhead(0);
  qp.setLookingAhead(false);
  qurve.setMultiplyQurvature(false);
}

//*****************************************************************************************************************************************
bool Quiet::addAsterisk(std::string& _presetName)
{
  if (_presetName.size() == 0)
    return false;

  if (_presetName.back() == '*')
    return false;
  else
  {
    _presetName += '*';
    return true;
  }
}

//*****************************************************************************************************************************************
void Quiet::updateQenuTheme(IGraphics* _qui)
{
  _qui->AttachPopupMenuControl(IText(16.f, EAlign::Center, qheme.foreGround));
  _qui->GetPopupMenuControl()->SetItemColor(qheme.foreGround);
  _qui->GetPopupMenuControl()->SetPanelColor(qheme.qrey.WithOpacity(.23f));
  _qui->GetPopupMenuControl()->SetItemMouseoverColor(qheme.qrey.WithContrast(.23f));
  _qui->GetPopupMenuControl()->SetCellBackgroundColor(qheme.foreGround.WithOpacity(.073f));
}

//*****************************************************************************************************************************************
void Quiet::updateUndoRedoQuttons(const IGraphics* _qui)
{
  _qui->GetControlWithTag(qCtrlTagUndo)->SetDisabled(!qurve.undoPossible());
  _qui->GetControlWithTag(qCtrlTagRedo)->SetDisabled(!qurve.redoPossible());
}