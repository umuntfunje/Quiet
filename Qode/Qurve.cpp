#include "Qurve.h"

//*****************************************************************************************************************************************
Qurve::Qurve()
{
  qurveBuffer = std::make_unique<float[]>(maxResolution + xtraValues);

  now.qnodeList[0] = {.0f, .0f};
  now.qurvature[0] = now.qurvature[1] = .0f;
  now.qnodeList[1] = {1.f, .0f};

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::qreateQurve()
{
  int qnodeIndex = 0;
  float currQurvature = .0f;
  std::pair<float, float> prevQnode = now.qnodeList[qnodeIndex];
  std::pair<float, float> currQnode = now.qnodeList[qnodeIndex];

  for (int i = 0; i < maxResolution; i++)
  {
    float x = i / (maxResolution - 1.f); // normalization : from .0f to 1.f
    float t = 1.f;

    if (currQnode.first > prevQnode.first)
      t = (x - prevQnode.first) / (currQnode.first - prevQnode.first);

    if (now.isSmooth)
      t = smoothTransition(t);

    t = Clip(qurveScale(t, currQurvature), .0f, 1.f); 

    float y = prevQnode.second + t * (currQnode.second - prevQnode.second);

    qurveBuffer[i + 1] = 1.f - y;

    while (x > currQnode.first && qnodeIndex < now.qnodeCount)
    {
      currQurvature = now.qurvature[qnodeIndex % now.qnodeCount];
      qnodeIndex++;
      prevQnode = currQnode;
      currQnode = now.qnodeList[qnodeIndex % now.qnodeCount];

      if (qnodeIndex >= now.qnodeCount)
      {
        currQnode.first += 1.f;
        break;
      }
    }
  }

  //qurve[0] = qurve[maxResolution];
  //qurve[maxResolution + 1] = qurve[1];
  //qurve[maxResolution + 2] = qurve[2];
  
  //qurve[0] = qurve[1];
  //qurve[maxResolution + 1] = qurve[maxResolution];
  //qurve[maxResolution + 2] = qurve[maxResolution];

  qurveBuffer[0] = qurveBuffer[1];
  qurveBuffer[maxResolution + 1] = qurveBuffer[0];
}

//*****************************************************************************************************************************************
void Qurve::addQnode(int _index, std::pair<float, float> _pos)
{
  for (int i = now.qnodeCount; i > _index; i--)
  {
    now.qnodeList[i] = now.qnodeList[i - 1];
    now.qurvature[i] = now.qurvature[i - 1];
  }

  now.qnodeCount++;
  now.qnodeList[_index] = _pos;
  now.qurvature[_index] = .0f;

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::removeQnode(int _index)
{
  now.qnodeCount--;

  for (int i = _index; i < now.qnodeCount; i++)
  {
    now.qnodeList[i] = now.qnodeList[i + 1];
    now.qurvature[i] = now.qurvature[i + 1];
  }

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::setQnode(int _index, std::pair<float, float> _newPos)
{
  assert(_index >= 0 && _index < maxQnodes);
  now.qnodeList[_index] = _newPos;
}

//*****************************************************************************************************************************************
std::pair<float, float> Qurve::getQnode(int _index) const
{
  assert(_index >= 0 && _index < maxQnodes);
  return now.qnodeList[_index];
}

//*****************************************************************************************************************************************
void Qurve::setQnodeCount(int _qnodeCount)
{
  now.qnodeCount = _qnodeCount;
}

//*****************************************************************************************************************************************
int Qurve::getQnodeCount() const
{
  return now.qnodeCount;
}

//*****************************************************************************************************************************************
float* Qurve::getQurveBuffer() const
{
  return qurveBuffer.get();
}

//*****************************************************************************************************************************************
void Qurve::setQurvature(int _index, float _qurvature)
{
  assert(_index >= 0 && _index < maxQnodes);
  now.qurvature[_index] = _qurvature;
}

//*****************************************************************************************************************************************
float Qurve::getQurvature(int _index) const
{
  return now.qurvature[_index];
}

//*****************************************************************************************************************************************
void Qurve::setMultiplyQurvature(bool _multiplyQurvature)
{
  if (_multiplyQurvature == multiplyQurvature)
    return;

  multiplyQurvature = _multiplyQurvature;
  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::setQurveMultiplier(float _qurveMultiplier)
{
  qurveMultiplier = _qurveMultiplier;
}

//*****************************************************************************************************************************************
void Qurve::flipVertical()
{
  for (int i = 0; i < now.qnodeCount; i++)
    now.qnodeList[i].second = 1.f - now.qnodeList[i].second;

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::setSmooth(bool _isSmooth)
{
  now.isSmooth = _isSmooth;
}

//*****************************************************************************************************************************************
bool Qurve::getSmooth() const
{
  return now.isSmooth;
}

//*****************************************************************************************************************************************
void Qurve::setQurveName(const char* _qurveName)
{
  qurveName = _qurveName;
}

//*****************************************************************************************************************************************
const char* Qurve::getQurveName() const
{
  return qurveName.c_str();
}

//*****************************************************************************************************************************************
float Qurve::getValueAtPhase(float _phase) const
{
  for (int i = 0; i < now.qnodeCount - 1; i++)
    if (now.qnodeList[i].first <= _phase && now.qnodeList[i + 1].first >= _phase)
      return getValueBetween(_phase, i, i + 1);

  return now.qnodeList[now.qnodeCount - 1].second;
}

//*****************************************************************************************************************************************
void Qurve::randomizeQurve()
{
  qurveName = "random";
  now.isSmooth = static_cast<int>(std::roundf(randomness.next()));
  now.qnodeCount = static_cast<int>(std::roundf(randomness.next() * (maxQnodes / 10))); // 100 is too many, i think...

  if (now.qnodeCount <= 2 || now.qnodeCount > maxQnodes)
  {
    now.qnodeCount = 2;
    now.qnodeList[0] = {.0f, randomness.next()};
    now.qurvature[0] = now.qurvature[now.qnodeCount - 1] = randomness.next() * maxQurvature;
    now.qnodeList[now.qnodeCount - 1] = {1.f, now.qnodeList[0].second};

    std::srand(static_cast<uint32_t>(time(NULL)));
    randomness.seed(std::rand());

    qreateQurve();
    return;
  }

  now.qnodeList[0] = {.0f, randomness.next()};
  now.qurvature[0] = now.qurvature[now.qnodeCount - 1] = (randomness.next() * 2 - 1) * maxQurvature; // -maxQurvature to maxQurvature
  now.qnodeList[now.qnodeCount - 1] = {1.f, now.qnodeList[0].second};

  std::vector<float> x;

  for (int i = 1; i < now.qnodeCount - 1; i++)
    x.push_back(randomness.next());

  std::sort(x.begin(), x.end());

  for (int j = 1; j < now.qnodeCount - 1; j++)
  {
    now.qurvature[j] = (randomness.next() * 2 - 1) * maxQurvature; // -maxQurvature to maxQurvature
    now.qnodeList[j] = {x[j - 1], randomness.next()};
  }

  std::srand(static_cast<uint32_t>(time(NULL)));
  randomness.seed(std::rand());

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::undoCurrentState()
{
  if (pastStates.empty())
    return;

  futureStates.push_back(now);
  now = pastStates.back();
  pastStates.pop_back();

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::redoCurrentState()
{
  if (futureStates.empty())
    return;

  pastStates.push_back(now);
  now = futureStates.back();
  futureStates.pop_back();

  qreateQurve();
}

//*****************************************************************************************************************************************
void Qurve::updateCurrentState()
{
  pastStates.push_back(now);
  futureStates.clear();
}

//*****************************************************************************************************************************************
void Qurve::updateCurrentState(const QurveState& _past)
{
  pastStates.push_back(_past);
  futureStates.clear();
}

//*****************************************************************************************************************************************
bool Qurve::undoPossible() const
{
  return !pastStates.empty();
}

//*****************************************************************************************************************************************
bool Qurve::redoPossible() const
{
  return !futureStates.empty();
}

//*****************************************************************************************************************************************
Qurve::QurveState Qurve::getNow() const
{
  return now;
}

//*****************************************************************************************************************************************
float Qurve::smoothTransition(float t) const
{
  return .5f * sinf((t - .5f) * static_cast<float>(PI)) + .5f;
}

//*****************************************************************************************************************************************
float Qurve::qurveScale(float _value, float _qurvature) const
{
  static constexpr float minQurvature = .01f;

  if (fabsf(_qurvature) < minQurvature)
    return _value;

  float numerator = expf(_qurvature * _value) - 1.f;
  float denominator = expf(_qurvature) - 1.f;

  return numerator / denominator;
}

//*****************************************************************************************************************************************
float Qurve::getValueBetween(float _phase, int _from, int _to) const
{
  assert(_from >= 0 && _to < now.qnodeCount);

  std::pair<float, float> first = now.qnodeList[_from];
  std::pair<float, float> second = now.qnodeList[_to];
  float _qurvature = now.qurvature[_from];

  if (multiplyQurvature)
    _qurvature *= qurveMultiplier;

  float width = second.first - first.first;

  if (width <= .0f)
    return 1.f - second.second;

  float t = (_phase - first.first) / width;

  if (now.isSmooth)
    t = smoothTransition(t);

  t = Clip(qurveScale(t, _qurvature), .0f, 1.f);

  return 1.f - (t * (second.second - first.second) + first.second);
}