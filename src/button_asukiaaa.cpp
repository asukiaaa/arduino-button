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

MultiTimesPressDetector::MultiTimesPressDetector(int timesPress,
                                               unsigned long msLessThan)
    : timesPress(timesPress), msLessThan(msLessThan) {
  dataArr = new unsigned long[timesPress];
  for (int i = 0; i < timesPress; ++i) {
    dataArr[i] = 0;
  }
  reset();
}

void MultiTimesPressDetector::reset() {
  usedLength = 0;
  nextIndex = 0;
}

MultiTimesPressDetector::~MultiTimesPressDetector() { delete[] dataArr; }

void MultiTimesPressDetector::updateByBtnState(
    const button_asukiaaa::ButtonState& btn) {
  updateByChangedToPress(btn.changedToPress());
}

void MultiTimesPressDetector::updateByChangedToPress(bool changedToPress) {
  if (changedToPress) {
    dataArr[nextIndex] = millis();
    incrementNextIndex();
  }
}

bool MultiTimesPressDetector::detect() {
  if (usedLength < timesPress) {
    return false;
  }
  return (getDataBefore(0) - getDataBefore(timesPress - 1)) < msLessThan;
}

bool MultiTimesPressDetector::detectAndResetIfTrue() {
  auto result = detect();
  if (result) {
    reset();
  }
  return result;
}

void MultiTimesPressDetector::incrementNextIndex() {
  if (usedLength <= nextIndex + 1) {
    usedLength = nextIndex + 1;
  }
  if (nextIndex == timesPress - 1) {
    nextIndex = 0;
  } else {
    ++nextIndex;
  }
}

unsigned long MultiTimesPressDetector::getDataBefore(int length) {
  auto targetIndex = nextIndex - 1 - length;
  while (targetIndex < 0) {
    targetIndex += timesPress;
  }
  return dataArr[targetIndex];
}

int MultiTimesPressDetector::getTimesPress() { return timesPress; }
unsigned long MultiTimesPressDetector::getMsLessThan() { return msLessThan; }

}  // namespace button_asukiaaa
