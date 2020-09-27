# arduino-button (button_asukiaaa)

This library supplies useful functions to manage button behavior on Arduino.

## Usage

See [an example project](./examples/gpioButton/gpioButton.ino).

Include
```c
#include <utils_asukiaaa.h>
#include <utils_asukiaaa/button.h>
```

Define with gpio pin
```c
#define PIN_BTN 10
utils_asukiaaa::button::Button btn(PIN_BTN);
```

Call update in loop to check pin state
```c
void loop() {
  btn.update();
}
```

Call process on changed to release or press
```c
if (btn.changedToPress()) {
  // process on change to press
} else if (btn.changedToRelease()) {
  // process on change to release
}
```

Call process by button state
```c
if (btn.isPressing()) {
  // process during pressed
} else { // btn.isReleasing()
  // process during released
}
```


## License

MIT
