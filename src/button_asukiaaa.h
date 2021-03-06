#ifndef _BUTTON_ASUKIAAA_H_
#define _BUTTON_ASUKIAAA_H_

#include <Arduino.h>

namespace button_asukiaaa {
  class ButtonState {
  public:
    ButtonState(bool pressedPinState = LOW, unsigned long bufferMs = 20UL);

    void update(bool newState);
    bool changedToPress();
    bool changedToRelease();
    bool isPresseing();
    bool isReleasing();

  private:
    unsigned long bufferMs;
    unsigned long changedAt;
    char timingState;
    bool onOverBufferTime;
    bool changedOverBufferTime;
    bool currentPinState;
    bool changedPinState;
    bool prevPinState;
    bool holedPinState;
    bool pressedPinState;
  };

  class Button: public ButtonState {
  public:
    Button(int pin,
           unsigned long bufferMs = 20UL,
           int pinModeOption = INPUT_PULLUP,
           bool pressedPinState = LOW);
    void begin();
    void update();

  private:
    int pin;
    int pinModeOption;
  };
}

#endif
