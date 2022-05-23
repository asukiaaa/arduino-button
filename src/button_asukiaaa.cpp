#include "button_asukiaaa.h"

namespace button_asukiaaa {
  ButtonState::ButtonState(bool pressedPinState, unsigned long bufferMs) {
    this->bufferMs = bufferMs;
    this->pressedPinState = pressedPinState;
    changedAt = 0;
    prevPinState = currentPinState = holedPinState = !pressedPinState;
  }

  void ButtonState::update(bool newState) {
    prevPinState = currentPinState;
    currentPinState = newState;
    if (currentPinState != prevPinState) {
      if (currentPinState == holedPinState) {
        // detect changing state as noize
        changedOverBufferTime = true;
      } else {
        changedAt = millis();
        changedOverBufferTime = false;
        onOverBufferTime = false;
      }
    }
    if (changedOverBufferTime && onOverBufferTime) {
      onOverBufferTime = false; // become false after first time of over buffer time
    }
    if (!changedOverBufferTime && millis() - changedAt > bufferMs) {
      changedOverBufferTime = true;
      onOverBufferTime = true;
      holedPinState = currentPinState;
    }
  }

  bool ButtonState::changedToPress() const {
    return currentPinState == pressedPinState && onOverBufferTime;
  }
  bool ButtonState::changedToRelease() const {
    return currentPinState != pressedPinState && onOverBufferTime;
  }
  bool ButtonState::isPresseing() const {
    return holedPinState == pressedPinState;
  }
  bool ButtonState::isReleasing() const {
    return holedPinState != pressedPinState;
  }

  Button::Button(int pin,
                 unsigned long bufferMs,
                 int pinModeOption,
                 bool pressedPinState)
    : ButtonState(pressedPinState, bufferMs) {
    this->pin = pin;
    this->pinModeOption = pinModeOption;
  }

  void Button::begin() {
    pinMode(pin, pinModeOption);
  }

  void Button::update() {
    ButtonState::update(digitalRead(pin));
  }
}
