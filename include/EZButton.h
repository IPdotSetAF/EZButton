#ifndef EZButton_H
#define EZButton_H

#include <Arduino.h>

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
    unsigned long HoldTreshHold;
    unsigned long HoldInterval;

    EZButton(const String buttonNames[],
             bool *(*readButtons)(String *, int),
             unsigned long holdTreshold = 500,
             unsigned long holdInterval = 500);

    ~EZButton();

    void Reset();
    void CheckButtons();
    void Subscribe(String buttonName, void (*event)(), EventTypes type);

private:
    int _numButtons;
    String *_buttons;

    unsigned long *_buttonDownMillis;
    unsigned int *_lastHoldInterval;
    bool *_buttonLastState;

    typedef void (*Event)(); // Type alias for function pointer
    Event *_onPressedEvents;
    Event *_onReleaseEvents;
    Event *_onHoldReleaseEvents;
    Event *_onHoldEvents;
    bool *(*_readButtons)(String *, int);
};

#endif // EZButton_H