# Daily Checklist IoT Device — Project Plan

## Who this is for
Ali — student at Jensen YH (IoT/embedded), learning C, Python, and embedded Linux.
Motivation: wants recruiter-relevant skills (C, Python, embedded Linux) tied to a real
project, not isolated exercises. Personal interests: productivity, helping people
(prior background in social work).

## Final product (long-term vision)
A physical desk device showing a daily-changing task checklist. A screen (eventually
touchscreen) shows today's tasks; pressing a button/touch marks a task done. The list
changes daily because it's pulled from a real database, not hardcoded. Two input
devices report in over different radios (WiFi and Bluetooth), both unified by a
Raspberry Pi gateway — this mirrors how real heterogeneous IoT systems work.

## Boards actually owned (confirmed, no purchase needed)
- **Raspberry Pi Zero 2 W** (RP3A0-AU, red case, 32GB microSD) — real Linux SBC,
  quad-core. This is the one genuine **embedded Linux** board. Role: gateway.
- **Raspberry Pi Pico 2 W** (RP2350) — microcontroller, WiFi confirmed working via
  onboard CYW43439 chip. Native Pico C SDK is lower-level than Arduino framework
  (Arduino-for-Pico is literally a wrapper around the same C SDK) — best board for
  genuine low-level C practice. Caveat: community Arduino libraries for Pico are
  reportedly less mature/maintained than for ESP32/Arduino boards; Bluetooth access
  from the RP2350 side of this chip is currently limited (WiFi is fine, BT isn't
  fully wired up yet on this revision) — not an issue since this board's job is WiFi.
  Role: **main checklist input device**.
- **ESP32-H2-DevKitM-1** — Bluetooth LE + 802.15.4 (Zigbee/Thread), genuinely has
  **no WiFi radio** (confirmed: this chip trades WiFi for Zigbee/Thread support).
  Role: **second input device**, connects via Bluetooth instead of WiFi — this is
  what makes the Pi's gateway/translator role actually necessary, not just added
  for practice.
- **Arduino Uno R4 WiFi** — has WiFi + Bluetooth via an onboard ESP32-S3
  co-processor (the Minima version has neither — confirmed this is the WiFi
  version). Role: **in reserve** — backup WiFi node if Pico library gaps cause
  problems, or a future 3rd device.

## Architecture
```
You (phone/laptop) --add tasks--> Flask API --> Postgres (source of truth)
                                        ^
                                        | (Pi calls Flask when a task is marked done)
                                        |
Postgres --each morning--> Pi Zero 2 W (embedded Linux: MQTT broker + bridge)
                              ^                    ^
                    MQTT "tasks/today" (down)      |
                              |                    |
                    MQTT "tasks/done" (up)         |
                              v                    |
                    Pico 2 W (C, WiFi)     ESP32-H2 (C, Bluetooth)
                    MAIN checklist button   2nd input device
```

## Why staged, not built all at once
Doing everything at once (WiFi + Bluetooth + MQTT + Pi service + Flask + DB) means
if something breaks, it's unclear which of 5+ new things failed. Build one proven
layer at a time.

## Staged build plan
- **v1 (current step):** Pico 2 W + one button, using the **native Pico C SDK**
  (not Arduino framework — more real low-level C practice, confirmed lower-level
  than Arduino). Connect to WiFi, on button press send a direct HTTP POST to Flask.
  No Pi, no MQTT, no second device yet. Goal: prove the full loop works end-to-end
  (button -> WiFi -> API -> DB).
- **v2:** Add Raspberry Pi Zero 2 W as MQTT broker (Mosquitto) + Python bridge
  script. Tasks now pulled from Postgres each morning and pushed down to the Pico
  via MQTT instead of being hardcoded. Completions publish back up through the Pi
  to Flask. Add the **ESP32-H2 as a second input device over Bluetooth**, relayed
  through the same Pi — this is what proves the gateway is genuinely earning its
  place, not just practice.
- **v3:** Polish — small screen or touchscreen (see hardware note below), nicer
  enclosure, streak/carry-over logic, optionally an AI agent for smarter scheduling
  much later.
- **v4 (deepening pass, after the product works — optional/stretch):**
  1. Rewrite the Pi's bridge script in C, cross-compiled for ARM (real embedded
     Linux skill: cross-compilation), instead of Python.
  2. Turn it into a proper systemd service (auto-start on boot, restart on crash).
  3. Stretch goal: build a minimal custom Linux image for the Pi with Buildroot
     instead of stock Raspberry Pi OS — the biggest step toward what specialized
     "embedded Linux" job postings actually mean (vs. just running scripts on a
     pre-built OS). Genuinely harder — optional, not required to call this done.

## Touchscreen note (for v3, not now)
"Cheap Yellow Display" (CYD, ESP32-2432S028R) — ESP32 with built-in 2.8" touchscreen,
~$15-20, uses the LVGL graphics library. Would require buying this specific board
when v3 is reached. Decision: finish WiFi/button/POST logic on the Pico first, move
that proven logic to the CYD later so touch/LVGL is the only new unknown at that
point.

## Tooling decisions + rationale
- **VS Code + PlatformIO**, not Arduino IDE — closer to real embedded workflows,
  better project structure, works cleanly with git.
- **Native Pico C SDK** for the main node (v1) — chosen over Arduino framework
  specifically for deeper, more realistic C practice (evidence: Arduino-for-Pico is
  built as a wrapper on top of this same SDK, not the reverse).
- Flask + Postgres backend reuses patterns from Ali's finished "IoT Telemetry &
  Analytics Platform" school assignment (github.com/aliazam94j/iot-telemetry-platform).

## Current status
- ESP32 PlatformIO project ("Productivity") previously created at
  `C:\Users\AliAz\Documents\esp32-projects\Productivity` — was based on the wrong
  board (esp32dev / classic ESP32) before discovering the H2 has no WiFi. This
  project's fate (reuse vs. start fresh for the Pico) still needs to be decided
  when v1 setup actually begins.
- Next immediate step: set up a PlatformIO (or native Pico SDK toolchain) project
  targeting the **Pico 2 W**, confirm board detection/flashing works, then **v1
  step 1 — connect to WiFi, print confirmation, using the native C SDK.**

## How Ali wants to be taught/worked with
- Explain briefly what new code does as it's introduced, especially anything
  platform-specific (WiFi, HTTPClient/lwIP, MQTT, Bluetooth) — don't assume it's
  remembered just because he's done Arduino before.
- One step at a time; check understanding before moving to the next step.
- Don't dump large finished files at once.
- Be direct/neutral/evidence-based when asked for an honest assessment — don't
  just cheerlead the project.
- **Do NOT just write the code for him.** He wants to type it himself. For each
  new piece of code:
  1. Explain the general skeleton/structure that's standard for this kind of task.
  2. Point out which parts are fixed (required exactly as-is, from a library/SDK)
     vs which parts are customizable (names he invents himself).
  3. If there's more than one common way to write it, show the most common/
     standard convention as the main answer, and only briefly mention
     alternatives exist — don't dump every possible variant.
  4. Let him write the actual line(s) himself. Give hints/corrections, not the
     finished code, unless he explicitly asks to just see the answer.
  5. Always be explicit about what comes from a library/framework (fixed) vs what
     is just a name he's free to choose (e.g. `WiFi.begin(ssid, password)` —
     `WiFi.begin` is fixed, `ssid`/`password` are just variable names).
- **"Round kick" rule (his own analogy, keep using it):** there's rarely only one
  correct way to write something, same as there's more than one way to throw a
  round kick. Show the most common 1-2 ways, say clearly which is most standard,
  briefly note other approaches exist. Apply proactively, not just when asked.
- **Syntax memorization is not the goal (his explicit view, evidence-backed as of
  2026):** he doesn't need to memorize exact syntax by heart — AI tools and docs
  handle recall now. What matters: reading code and knowing what it does, spotting
  when something looks wrong, debugging, and knowing there are other ways to write
  something (round-kick rule above). The specific gap to close: he often
  understands ~90% of copied code but isn't sure if what he copied is "the only
  way" — proactively flag alternatives/reasoning, not just when asked.