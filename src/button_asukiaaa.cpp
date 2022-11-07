#include "button_asukiaaa.h"

namespace button_asukiaaa {
ButtonState::ButtonState(bool pressedPinState, unsigned long bufferMs)
    : pressedPinState(pressedPinState) {
  this->bufferMs = bufferMs;
  changedAt = 0;
  prevPinState = currentPinState = holedPinState = !pressedPinState;
}

void ButtonState::update(bool newState) {
  updatedAtPrev = updatedAtCurrent;
  updatedAtCurrent = millis();
  prevPinState = currentPinState;
  currentPinState = newState;
  if (currentPinState != prevPinState) {
    if (currentPinState == holedPinState) {
      // detect changing state as noize
      changedOverBufferTime = true;
    } else {
      changedAt = updatedAtCurrent;
      changedOverBufferTime = false;
      onOverBufferTime = false;
    }
  }
  if (changedOverBufferTime && onOverBufferTime) {
    onOverBufferTime =
        false;  // become false after first time of over buffer time
  }
  if (!changedOverBufferTime && updatedAtCurrent - changedAt > bufferMs) {
    changedOverBufferTime = true;
    onOverBufferTime = true;
    holedPinState = currentPinState;
  }
}

bool ButtonState::changedForMs(unsigned long ms) const {
  auto diffCurrent = updatedAtCurrent - changedAt;
  auto diffPrev = updatedAtPrev - changedAt;
  return diffPrev < ms && ms <= diffCurrent;
}
bool ButtonState::changedToPress() const {
  return currentPinState == pressedPinState && onOverBufferTime;
}
bool ButtonState::changedToPressForMs(unsigned long ms) const {
  return isPresseing() && changedForMs(ms);
}
bool ButtonState::changedToRelease() const {
  return currentPinState != pressedPinState && onOverBufferTime;
}
bool ButtonState::changedToReleaseForMs(unsigned long ms) const {
  return isReleasing() && changedForMs(ms);
}
bool ButtonState::isPresseing() const {
  return holedPinState == pressedPinState;
}
bool ButtonState::isReleasing() const {
  return holedPinState != pressedPinState;
}

bool ButtonState::rawCurrentPressed() const {
  return currentPinState == pressedPinState;
}

Button::Button(int pin, unsigned long bufferMs, int pinModeOption,
               bool pressedPinState)
    : ButtonState(pressedPinState, bufferMs) {
  this->pin = pin;
  this->pinModeOption = pinModeOption;
}

void Button::begin() { pinMode(pin, pinModeOption); }

void Button::update() { ButtonState::update(digitalRead(pin)); }
}  // namespace button_asukiaaa
