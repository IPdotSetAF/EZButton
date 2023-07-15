#ifndef EZButton_H
#define EZButton_H

#include <Arduino.h>

#define EVENT_COUNT 4
enum EventTypes
{
    PRESSED,
    HOLD,
    HOLD_RELEASE,
    RELEASE
};

class EZButton
{
public:
    unsigned int HoldTreshHold;
    unsigned int HoldInterval;

    EZButton(int buttonCount,
             void (*readButtons)(bool *, int),
             unsigned int holdTreshold = 500,
             unsigned int holdInterval = 500);

    ~EZButton();

    void Reset();
    void CheckButtons();
    void Subscribe(int index, void (*event)(), EventTypes type);

private:
    int _numButtons;

    unsigned long *_buttonDownMillis;
    unsigned int *_lastHoldInterval;
    bool *_buttonLastState;

    typedef void (*Event)(); // Type alias for function pointer
    Event *_events;
    void (*_readButtons)(bool *, int);

    void CallEvent(int index, EventTypes type);
};

#endif // EZButton_H