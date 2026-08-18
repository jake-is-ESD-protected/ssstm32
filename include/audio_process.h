#ifndef AUDIO_PROCESS_H
#define AUDIO_PROCESS_H

void audio_process(float in_l, float in_r, float *out_l, float *out_r);
void audio_process_reset(void);

#endif
