#ifndef EZButton_H
#define EZButton_H

#include <Arduino.h>

#define EVENT_COUNT 4
enum EventTypes
{
    PRESSED,
    HOLD,
    HOLD_RELEASED,
    RELEASED
};

class EZButton
{
public:
    unsigned int HoldThreshold;
    unsigned int HoldInterval;
    unsigned int DebounceTime;

    EZButton(int buttonCount,
             void (*readButtons)(bool *, int),
             unsigned int holdThreshold = 500,
             unsigned int holdInterval = 500,
             unsigned int debounceTime = 15);

    ~EZButton();

    void Blackout(unsigned long milis);
    void Loop();
    void Subscribe(int index, void (*event)(), EventTypes type);

private:
    int _numButtons;

    unsigned long *_buttonDownMillis;
    unsigned int *_lastHoldInterval;
    bool *_buttonLastState;
    unsigned long _blackoutTime = 0;

    typedef void (*Event)(); // Type alias for function pointer
    Event *_events;
    void (*_readButtons)(bool *, int);

    void CallEvent(int index, EventTypes type);
    int EventIndex(int index, EventTypes type);

#ifdef EZBUTTON_DEBUG
    void LogEventToSerial(String message, int index, EventTypes type);
#endif
};

#endif // EZButton_H