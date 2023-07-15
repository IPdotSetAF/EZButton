#include "EZButton.h"

EZButton::EZButton(int buttonCount,
                   void (*readButtons)(bool *, int),
                   unsigned int holdTreshold,
                   unsigned int holdInterval)
{
    _numButtons = buttonCount;
    HoldTreshHold = holdTreshold;
    HoldInterval = holdInterval;
    _readButtons = readButtons;
    _events = new Event[_numButtons * EVENT_COUNT];
    _buttonDownMillis = new unsigned long[_numButtons];
    _buttonLastState = new bool[_numButtons];
    _lastHoldInterval = new unsigned int[_numButtons];

    for (int i = 0; i < _numButtons * EVENT_COUNT; i++)
        _events[i] = nullptr;

    Reset();
}

EZButton::~EZButton()
{
    delete[] _buttonDownMillis;
    delete[] _buttonLastState;
    delete[] _lastHoldInterval;
    delete[] _events;
}

void EZButton::Reset()
{
    for (int i = 0; i < _numButtons; i++)
    {
        _buttonDownMillis[i] = 0;
        _buttonLastState[i] = false;
        _lastHoldInterval[i] = 0;
    }
}

void EZButton::CheckButtons()
{
    bool *buttonStates = new bool[_numButtons];
    _readButtons(buttonStates, _numButtons);

    for (int i = 0; i < _numButtons; i++)
    {
        if (buttonStates[i])
        {
            if (!_buttonLastState[i])
            {
                _buttonDownMillis[i] = millis();
                _buttonLastState[i] = 1;
                CallEvent(i, EventTypes::PRESSED);
            }
            else if (millis() - _buttonDownMillis[i] > HoldTreshHold)
            {
                unsigned long interval = (millis() - _buttonDownMillis[i]) / HoldInterval;
                if (interval > _lastHoldInterval[i])
                {
                    _lastHoldInterval[i] = interval;
                    CallEvent(i, EventTypes::HOLD);
                }
            }
        }
        else
        {
            if (_buttonLastState[i])
            {
                _buttonLastState[i] = 0;
                _lastHoldInterval[i] = 0;
                if (millis() - _buttonDownMillis[i] > HoldTreshHold)
                    CallEvent(i, EventTypes::HOLD_RELEASE);
                else
                    CallEvent(i, EventTypes::RELEASE);
            }
        }
    }

    delete[] buttonStates;
}

void EZButton::Subscribe(int index, void (*event)(), EventTypes type)
{
    _events[index + type * EVENT_COUNT] = event;
}

void EZButton::CallEvent(int index, EventTypes type)
{
    int i = index + type * EVENT_COUNT;
    if (_events[i] != nullptr)
        _events[i]();
}