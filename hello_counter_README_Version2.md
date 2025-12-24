# Hello Counter — simple Flipper Zero app

What it is
- A tiny example Flipper Zero application that displays a counter.
- Increment with A/right, decrement with B/left (mappings depend on your device/firmware).
- Long-press Back to exit.

Files
- `hello_counter.c` — main source file
- `manifest.json` — app metadata
- `README.md` — this file

How to install & build
1. Clone the official firmware (if you haven't already):
   ```
   git clone https://github.com/flipperdevices/flipperzero-firmware.git
   ```

2. Copy this folder into the firmware's `applications_user` directory:
   ```
   cp -r hello_counter flipperzero-firmware/applications_user/
   ```

3. Build the firmware using the repository's build tool. The firmware repo typically includes an `fbt` script:
   ```
   cd flipperzero-firmware
   ./fbt
   ```
   Or follow the official build instructions for your OS found in the firmware repo README.

4. Flash the produced firmware to your Flipper Zero following the firmware repo instructions (use `qflipper` or the CLI flash steps mentioned in the firmware docs).

Notes & tips
- The Furi/GUI API evolves. If your build fails with missing symbols or different function names, inspect the firmware's `applications` examples to adapt:
  - header locations might be `gui/view_port.h` vs `gui/gui.h` or similar.
  - input key constants may differ; check `input.h` in your firmware tree.
- If you prefer the app to be a standalone package (not part of firmware), look up "Flipper Zero user applications" in the firmware docs — the process is the same: put your app under `applications_user` and build.
- The entry function name (`hello_counter_app` here) can be adapted if your firmware expects a specific symbol (some templates use `main` or `app_main`). If the build system or loader complains, rename the function to match your firmware's examples.

That's it — a tiny interactive example to get you started. If you want:
- a different app idea (IR remote, simple NFC tag viewer, radio scanner stub),
- or a version that uses a menu or persistent storage,
tell me which and I'll extend this into that direction and adjust it to a specific SDK version.