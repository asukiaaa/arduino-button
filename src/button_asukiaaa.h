#ifndef _BUTTON_ASUKIAAA_H_
#define _BUTTON_ASUKIAAA_H_

#include <Arduino.h>

namespace button_asukiaaa {
class ButtonState {
 public:
  ButtonState(bool pressedPinState = HIGH, unsigned long bufferMs = 20UL);

  void update(bool newState);
  bool changedToPress() const;
  bool changedToPressForMs(unsigned long ms) const;
  bool changedToRelease() const;
  bool changedToReleaseForMs(unsigned long ms) const;
  bool isPresseing() const;
  bool isReleasing() const;
  bool rawCurrentPressed() const;

 private:
  unsigned long bufferMs;
  unsigned long changedAt;
  unsigned long updatedAtCurrent = 0;
  unsigned long updatedAtPrev = 0;
  char timingState;
  bool onOverBufferTime;
  bool changedOverBufferTime;
  bool currentPinState;
  bool changedPinState;
  bool prevPinState;
  bool holedPinState;
  bool pressedPinState;

  bool changedForMs(unsigned long ms) const;
};

class Button : public ButtonState {
 public:
  Button(int pin, unsigned long bufferMs = 20UL,
         int pinModeOption = INPUT_PULLUP, bool pressedPinState = LOW);
  void begin();
  void update();

 private:
  int pin;
  int pinModeOption;
};
}  // namespace button_asukiaaa

#endif
