# Contributing

Thanks for helping students learn electronics! Fixes, better explanations and new projects are all welcome.

## Reporting a problem

Open an [issue](https://github.com/hussainzia/Arduino-ESP32-Robotics/issues/new/choose) and include:
- the project folder name
- your board (UNO, Nano, ESP32 ...) and Arduino IDE / board package version
- the full error message or what the hardware does wrong

## Adding a new project

1. Pick the right category folder and add the next number, e.g. `03_Smart_Automation/13_My_Project/`.
2. The `.ino` file **must have the same name as its folder** (`13_My_Project/13_My_Project.ino`).
3. Keep it **one self-contained sketch**: no extra `.h` files, so students can copy-paste it.
4. Start the sketch with the standard header (the README is generated from it):

```cpp
/*
  ============================================================
  PROJECT : My Project Name
  LEVEL   : Beginner | Intermediate | Advanced
  BOARD   : Arduino UNO / Nano   (or ESP32 Dev Module)
  ============================================================
  ABOUT
    What it does and why it is useful.

  PARTS
    - Arduino UNO
    - ...

  WIRING
    Sensor OUT -> D2   (optional note)

  LIBRARIES
    - Library name by Author (Library Manager)

  HOW IT WORKS
    1. ...

  TIPS
    - ...
  ============================================================
*/
```

5. Use the standard pins from the main README when you can, and comment every non-obvious line.
6. Add a `README.md` to the project folder and a row in the category `README.md`.
7. Add your sketch path to `.github/workflows/compile.yml` and any new library to its `libraries` list.

## Code style

- `#define` or `const` for every pin and setting, grouped at the top.
- Clear names (`leftSpeed`, not `ls`) and short comments that explain *why*.
- Avoid long `delay()` calls in WiFi projects, use `millis()` timing instead.
- Never commit real WiFi passwords, tokens or API keys: keep the `YOUR_...` placeholders.

## Pull requests

Every pull request is compiled automatically by GitHub Actions. Please make sure the check is green and that you have tested the project on real hardware.
