<p align=left>
   <img src="https://img.shields.io/github/v/release/IPdotSetAF/EZButton"/>
   <img src="https://img.shields.io/github/release-date/IPdotSetAF/EZButton"/>
   <img src="https://img.shields.io/github/last-commit/IPdotSetAF/EZButton"/>
   <img src="https://img.shields.io/github/license/IPdotSetAF/EZButton"/>
   <!--<img src="https://img.shields.io/github/downloads/IPdotSetAF/EZButton/total"/>-->
</p>

# EZButton
Transform raw button/touch inputs into events easily.

Subscibe to Pressed/Released/Hold/HoldReleased events of as many buttons as you want. Customize time tresholds. Works with any button read method.

## Features
- Flexibility
    - Works with:
        - Buttons
        - Touch
        - Any other signal
    - Also works with:
        - Pulling
        - Multiplexing
        - Interupts
        - AnalogReads
        - etc
- Event Subscibtion : You can subscribe to any of the following events for any button:
    - Pressed
    - Released
    - Hold
    - Hold Released
- Unlimited Buttons/Touches : You can config for as many buttons as you need.
- Customizability: You can change any of the time tresholds to customize you user experience.
    - `HoldTreshHold` : The time it takes before the first `HOLD` event is executed after button is held down.
    - `HoldInterval` : The Time Interval that corresponds to `HOLD` event being executed repeatedly after the first `HOLD` event was registerd.
- Debugging : Easily enable/disable debugging for all button states and events.
- Blackout Time: Disable any event execution for any amount of time.

## How To
### Installation
This Library is available in `Arduino Library Repository` and `PIO` and you can install it from: 
- Arduino IDE Library Manager
![arduino library manager]()
- PlatformIO Libraries
![pltformio library]()
`ipdotsetaf/EZButton@^2.2.0`
### Usage

1. Include the library
``` C++
#include <EZButton.h>
```
2. Create an object from `EZButton`
``` C++
#define BTN_1 0
#define BTN_2 1
#define BTN_3 2
#define BTN_4 3
//config for 4 buttons
//Read button states from the 'ReadButtons' function
//HoldTreshHold: 500ms
//HoldInterval: 300ms
EZButton _ezb(4, ReadButtons, 500, 300);
```
3. Initialize you buttons/touches however you want.
4. Attach any Interrups if needed.
5. Subscribe to any event you need
``` C++
//button index, function to execute, event type
_ezb.Subscribe(BTN_1, Btn1HoldRelease, HOLD_RELEASED);
_ezb.Subscribe(BTN_2, Btn2Release, RELEASED);
_ezb.Subscribe(BTN_3, Btn3Hold, HOLD);
_ezb.Subscribe(BTN_3, Btn3Release, RELEASED);
_ezb.Subscribe(BTN_4, Btn4Hold, HOLD);
_ezb.Subscribe(BTN_4, Btn4Release, RELEASED);
``` 
> [!IMPORTANT]
> 'button index' stands for an array inside EZButton that hold you buttins states and IS NOT pin number of the button.

6. Define `ReadButtons` function
``` C++
void ReadButtons(bool *states, int num)
{
    //Read all button states however you want
	states[BTN_1] = !digitalRead(2);
	states[BTN_2] = touchRead(3) <= 50;
	states[BTN_3] = touchRead(4) <= 50;
	states[BTN_4] = touchRead(5) <= 50;
}
```
7. Call EZButtons `Loop()` function in your main loop.
``` C++
void loop()
{
    //...

	_ezb.Loop();
}
```

> [!TIP]
> #### Debugging
> In order to enable debugging, you need to add the `-DEZBUTTON_DEBUG` parameter to your `build_flags`.
> 
> This will log event subscibtions and event executions to the serial.

> [!IMPORTANT]
> Right now only one subscribtion is possible for each button event.
>
> e.g. You can only subscribe to the `PRESSED` event of `BTN_2` once and the second subscribtion to this event will override the last one.
>
> You can still subscribe to other events of the same button with no problem. 

## TODO:
- Rewrite in C 
- Add multiple subscribtions to a single event

## Contribution
- You can open Issues for any bug report or feature request.
- You are free to contribute to this project by following these steps:
   1. Fork this Repo.
   2. Create a new branch for your feature/bugfix in your forked Repo.
   3. Commit your changes to the new branch you just made.
   4. Create a pull request from your branch into the `main` branch of This Repo([https://github.com/IPdotSetAF/EZButton](https://github.com/IPdotSetAF/EZButton)).
