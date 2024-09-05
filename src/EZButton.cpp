#include "EZButton.h"

#ifndef EZBUTTON_SerialOutput
    #define EZBUTTON_SerialOutput Serial
#endif

#ifdef EZBUTTON_DEBUG
    void EZButton::LogEventToSerial(String message, int index, EventTypes type)
    {
        EZBUTTON_SerialOutput.println(message);
        EZBUTTON_SerialOutput.println("button index: " + (String)index);
        EZBUTTON_SerialOutput.println("event type: " + (String)type);
        EZBUTTON_SerialOutput.println("event index: " + (String)EventIndex(index, type));
    }
    #define LogEvent(...) LogEventToSerial(__VA_ARGS__)
#else
    #define LogEvent(...) 
#endif

EZButton::EZButton(int buttonCount,
                   void (*readButtons)(bool *, int),
                   unsigned int holdThreshold,
                   unsigned int holdInterval,
                   unsigned int debounceTime)
{
    _numButtons = buttonCount;
    HoldThreshold = holdThreshold;
    HoldInterval = holdInterval;
    DebounceTime = debounceTime;
    _readButtons = readButtons;
    _events = new Event[_numButtons * EVENT_COUNT];
    _buttonDownMillis = new unsigned long[_numButtons];
    _buttonLastState = new bool[_numButtons];
    _lastHoldInterval = new unsigned int[_numButtons];

    for (int i = 0; i < _numButtons * EVENT_COUNT; i++)
        _events[i] = nullptr;

    Blackout(0);
}

EZButton::~EZButton()
{
    delete[] _buttonDownMillis;
    delete[] _buttonLastState;
    delete[] _lastHoldInterval;
    delete[] _events;
}

void EZButton::Blackout(unsigned long milis)
{
    for (int i = 0; i < _numButtons; i++)
    {
        _buttonDownMillis[i] = 0;
        _buttonLastState[i] = false;
        _lastHoldInterval[i] = 0;
    }
    _blackoutTime = millis() + milis;
}

void EZButton::Loop()
{
    if (_blackoutTime > millis())
        return;

    bool *buttonStates = new bool[_numButtons];
    _readButtons(buttonStates, _numButtons);

    for (int i = 0; i < _numButtons; i++)
    {
        unsigned long stateChangeDiff = millis() - _buttonDownMillis[i];
        if (stateChangeDiff < DebounceTime)
            continue;
            
        if (buttonStates[i])
        {
            if (!_buttonLastState[i])
            {
                _buttonDownMillis[i] = millis();
                _buttonLastState[i] = 1;
                CallEvent(i, EventTypes::PRESSED);
            }
            else if (stateChangeDiff > HoldThreshold)
            {
                unsigned int interval = stateChangeDiff / HoldInterval;
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
                if (stateChangeDiff > HoldThreshold)
                    CallEvent(i, EventTypes::HOLD_RELEASED);
                else
                    CallEvent(i, EventTypes::RELEASED);
            }
        }
    }

    delete[] buttonStates;
}

void EZButton::Subscribe(int index, void (*event)(), EventTypes type)
{
    _events[EventIndex(index, type)] = event;

    LogEvent("Subscribe:", index, type);
}

void EZButton::CallEvent(int index, EventTypes type)
{
    int i = EventIndex(index, type);

    LogEvent("Call:",index, type);

    if (_events[i] != nullptr)
        _events[i]();
}

int EZButton::EventIndex(int index, EventTypes type)
{
    return index + type * _numButtons;
}