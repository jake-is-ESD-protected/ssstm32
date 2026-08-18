# ssstm32

Stereo USB-audio state-space model runner for the NUCLEO-H753ZI.

## Hardware

- Connect ST-Link USB for flashing/debugging.
- Connect the board USB FS device connector to the host for audio.

The device exposes 48 kHz, 16-bit stereo USB speaker and microphone streams.

## Build and upload

```bash
uv run pio run
uv run pio run --target upload
```

The initial model is stereo identity loopback. Play audio to the USB speaker and record from the USB microphone to verify it.

## Edit the model

Edit only `src/audio_process.c` under normal use. It receives normalized stereo samples and implements:

```text
y[k] = C x[k] + D u[k]
x[k+1] = A x[k] + B u[k]
```

State resets at firmware boot. The USB transport adapter, TinyUSB descriptors, and STM32 port live outside this file.

## Measure headroom

Run the `tinyusb_audio` jescore job while audio is streaming. `audio cycles` reports maximum observed cycles for the state-space frame (`dsp`), the full USB-audio processing pass (`worker`), and one callback sample, against their 1 ms and 1/48,000 s deadlines. `free` is the remaining percentage in each budget. Run `tinyusb_audio_zero` before a new measurement.

## Pinned upstream sources

- `jescore`: `e81b78f28210e4fbc67edf53b97a846b7b90bc03`
- TinyUSB: `0.21.0`
- `lib/tinyusb_audio_h753zi/` is vendored from `jescore-examples` dev commit `89c5a4bfa9483a439afc2da18d0471ea09a367f4`.
