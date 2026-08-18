#include "audio_process.h"

/*
 * The only normal edit point for the firmware.
 *
 * Inputs and outputs are normalized PCM samples in [-1, 1). The callback runs
 * once per stereo sample at 48 kHz in a FreeRTOS task, not an interrupt.
 * Do not allocate, print, block, or perform I/O here.
 *
 * y[k] = C x[k] + D u[k]
 * x[k+1] = A x[k] + B u[k]
 */

enum { STATE_COUNT = 2, INPUT_COUNT = 2, OUTPUT_COUNT = 2 };

/* Identity stereo model: replace these matrices with the target model. */
static const float A[STATE_COUNT][STATE_COUNT] = {{0.0f, 0.0f}, {0.0f, 0.0f}};
static const float B[STATE_COUNT][INPUT_COUNT] = {{1.0f, 0.0f}, {0.0f, 1.0f}};
static const float C[OUTPUT_COUNT][STATE_COUNT] = {{0.0f, 0.0f}, {0.0f, 0.0f}};
static const float D[OUTPUT_COUNT][INPUT_COUNT] = {{1.0f, 0.0f}, {0.0f, 1.0f}};
static float state[STATE_COUNT];

void audio_process_reset(void) {
  for (unsigned index = 0; index < STATE_COUNT; ++index) state[index] = 0.0f;
}

void audio_process(float in_l, float in_r, float *out_l, float *out_r) {
  const float input[INPUT_COUNT] = {in_l, in_r};
  float output[OUTPUT_COUNT] = {0.0f};
  float next_state[STATE_COUNT] = {0.0f};

  for (unsigned row = 0; row < OUTPUT_COUNT; ++row) {
    for (unsigned column = 0; column < STATE_COUNT; ++column) output[row] += C[row][column] * state[column];
    for (unsigned column = 0; column < INPUT_COUNT; ++column) output[row] += D[row][column] * input[column];
  }
  for (unsigned row = 0; row < STATE_COUNT; ++row) {
    for (unsigned column = 0; column < STATE_COUNT; ++column) next_state[row] += A[row][column] * state[column];
    for (unsigned column = 0; column < INPUT_COUNT; ++column) next_state[row] += B[row][column] * input[column];
  }

  for (unsigned index = 0; index < STATE_COUNT; ++index) state[index] = next_state[index];
  *out_l = output[0];
  *out_r = output[1];
}
