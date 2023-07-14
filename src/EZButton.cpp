#include "EZButton.h"

EZButton::EZButton(const String buttonNames[],
                   bool *(*readButtons)(String *, int),
                   unsigned long holdTreshold,
                   unsigned long holdInterval)
{
    _numButtons = buttonNames->length() - 1;
    HoldTreshHold = holdTreshold;
    HoldInterval = holdInterval;
    _readButtons = readButtons;
    _buttons = new String[_numButtons];
    _onPressedEvents = new Event[_numButtons];
    _onReleaseEvents = new Event[_numButtons];
    _onHoldReleaseEvents = new Event[_numButtons];
    _onHoldEvents = new Event[_numButtons];
    _buttonDownMillis = new unsigned long[_numButtons];
    _buttonLastState = new bool[_numButtons];
    _lastHoldInterval = new unsigned int[_numButtons];

    for (int i = 0; i < _numButtons; i++)
    {
        _buttons[i] = buttonNames[i];
        _onPressedEvents[i] = nullptr;
        _onReleaseEvents[i] = nullptr;
        _onHoldReleaseEvents[i] = nullptr;
        _onHoldEvents[i] = nullptr;
        _buttonDownMillis[i] = 0;
        _buttonLastState[i] = false;
        _lastHoldInterval[i] = 0;
    }
}

EZButton::~EZButton()
{
    delete[] _buttons;
    delete[] _buttonDownMillis;
    delete[] _buttonLastState;
    delete[] _lastHoldInterval;
    delete[] _onPressedEvents;
    delete[] _onReleaseEvents;
    delete[] _onHoldEvents;
    delete[] _onHoldReleaseEvents;
}

void EZButton::Reset(){
    for (int i = 0; i < _numButtons; i++)
    {
        _buttonDownMillis[i] = 0;
        _buttonLastState[i] = false;
        _lastHoldInterval[i] = 0;
    }
}

void EZButton::CheckButtons()
{
    bool *buttonStates = _readButtons(_buttons, _numButtons);

    for (int i = 0; i < _numButtons; i++)
    {
        if (buttonStates[i])
        {
            if (!_buttonLastState[i])
            {
                _buttonDownMillis[i] = millis();
                _buttonLastState[i] = 1;
                if (_onPressedEvents[i] != nullptr)
                    _onPressedEvents[i]();
            }
            else if (millis() - _buttonDownMillis[i] > HoldTreshHold)
            {
                int interval = (millis() - _buttonDownMillis[i]) / HoldInterval;
                if (interval > _lastHoldInterval[i])
                {
                    _lastHoldInterval[i] = interval;
                    if (_onHoldEvents[i] != nullptr)
                        _onHoldEvents[i]();
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
                {
                    if (_onHoldReleaseEvents[i] != nullptr)
                        _onHoldReleaseEvents[i]();
                }
                else
                {
                    if (_onReleaseEvents[i] != nullptr)
                        _onReleaseEvents[i]();
                }
            }
        }
    }

    delete[] buttonStates;
}

void EZButton::Subscribe(String buttonName, void (*event)(), EventTypes type)
{
    int i = 0;
    for (; i < _numButtons; i++)
        if (_buttons[i].equals(buttonName))
            break;

    switch (type)
    {
    case EventTypes::PRESSED:
        _onPressedEvents[i] = event;
        break;
    case EventTypes::HOLD_RELEASE:
        _onHoldReleaseEvents[i] = event;
        break;
    case EventTypes::RELEASE:
        _onReleaseEvents[i] = event;
        break;
    case EventTypes::HOLD:
        _onHoldEvents[i] = event;
        break;
    }
}