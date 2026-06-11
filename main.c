#define MA_NO_DECODING
#define MA_NO_ENCODING

#include <stdio.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define MAX_LENGTH 256
const float SAMPLE_RATE = 48000;
const float DURATION_SAMPLE = 1.0f / SAMPLE_RATE;

float frekvence = 440;
float t = 0;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    float* pOutputF32 = (float*)pOutput;

    for (ma_uint32 curFrame = 0; curFrame < frameCount; ++curFrame) {
        float y = sin(MA_TAU_D * frekvence * t);

        t += DURATION_SAMPLE;

        if (t >= 1.0) {
          t = 0;
        }

        pOutputF32[curFrame] = y;
    }

    // Unused
    (void) pInput;
    (void) pDevice;
}

typedef enum Nastroj {
    HOUSLE,
    VIOLA,
    CELLO,
    KONTRABAS
} Nastroj;

int main(void) {
    ma_device_config deviceConfig;
    ma_device device;

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = ma_format_f32;
    deviceConfig.playback.channels = 1;
    deviceConfig.sampleRate = SAMPLE_RATE;
    deviceConfig.dataCallback = data_callback;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        return 1;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        return 1;
    }

    Nastroj nastroj = HOUSLE;
    char *input = malloc(sizeof(char)*MAX_LENGTH);
    char *nabidka;

    start:
    switch (nastroj) {
        case HOUSLE:
            printf("Nástroj: Housle\n");
            nabidka = "g = 196.0 Hz\nD1 (d) = 294.0 Hz\nA1 (a) = 440.0 Hz\nE2 (e) = 659.0 Hz\n";
            break;
        case VIOLA:
            printf("Nástroj: Viola\n");
            nabidka = "c = 130.0 Hz\ng = 196.0 Hz\nD1 (d) = 294.0 Hz\nA1 (a) = 440.0 Hz\n";
            break;
        case CELLO:
            printf("Nástroj: Violoncello\n");
            nabidka = "C (c) = 65.0 Hz\nG (g) = 98.0 Hz\nd = 147.0 Hz\na = 220.0 Hz\n";
            break;
        case KONTRABAS:
            printf("Nástroj: Kontrabas\n");
            nabidka = "E' (e) = 41.0 Hz\nA' (a) = 55.0 Hz\nD (d) = 73.0 Hz\nG = (g) 98.0 Hz\n";
            break;
        default:
            assert("NEMOŽNÉ!");
            break;
    }
    printf("Z = Změnit nástroj\nPrávě zní %.2f Hz\n", frekvence);
    printf(nabidka);
    printf("[+/-] Hz\nJiná frekvence: ");

    fgets(input, MAX_LENGTH, stdin);

    if (nastroj == HOUSLE) {
        if (strcmp(input, "g\n") == 0)
            frekvence = 196.0f;
        if (strcmp(input, "d\n") == 0)
            frekvence = 294.0f;
        if (strcmp(input, "a\n") == 0)
            frekvence = 440.0f;
        if (strcmp(input, "e\n") == 0)
            frekvence = 659.0f;
    } else if (nastroj == VIOLA) {
        if (strcmp(input, "c\n") == 0)
            frekvence = 130.0f;
        if (strcmp(input, "g\n") == 0)
            frekvence = 196.0f;
        if (strcmp(input, "d\n") == 0)
            frekvence = 294.0f;
        if (strcmp(input, "a\n") == 0)
            frekvence = 440.0f;
    } else if (nastroj == CELLO) {
        if (strcmp(input, "c\n") == 0)
            frekvence = 65.0f;
        if (strcmp(input, "g\n") == 0)
            frekvence = 98.0f;
        if (strcmp(input, "d\n") == 0)
            frekvence = 147.0f;
        if (strcmp(input, "a\n") == 0)
            frekvence = 220.0f;
    } else if (nastroj == KONTRABAS) {
        if (strcmp(input, "e\n") == 0)
            frekvence = 41.0f;
        if (strcmp(input, "a\n") == 0)
            frekvence = 55.0f;
        if (strcmp(input, "d\n") == 0)
            frekvence = 73.0f;
        if (strcmp(input, "g\n") == 0)
            frekvence = 98.0f;
    } else assert("NEMOŽNÉ!");

    float freq_input;
    if ((freq_input = atof(input)) != 0) {
        if (input[0] == '+' || input[0] == '-')
            frekvence += freq_input;
        else
            frekvence = freq_input;
    }
    if (strcmp(input, "z\n") == 0 || strcmp(input, "Z\n") == 0) {
        printf("Na jaký nástroj chcete změnit?\nH = Housle\nV = Viola\nC = Violoncello\nK = Kontrabas: ");
        fgets(input, MAX_LENGTH, stdin);
        if (strcmp(input, "h\n") == 0 || strcmp(input, "H\n") == 0) {
            printf("Správná volba!\n");
            nastroj = HOUSLE;
            frekvence = 440.0f;
        } else if (strcmp(input, "v\n") == 0 || strcmp(input, "V\n") == 0) {
            nastroj = VIOLA;
            frekvence = 440.0f;
        } else if (strcmp(input, "c\n") == 0 || strcmp(input, "C\n") == 0) {
            nastroj = CELLO;
            frekvence = 220.0f;
        } else if (strcmp(input, "k\n") == 0 || strcmp(input, "K\n") == 0) {
            nastroj = KONTRABAS;
            frekvence = 55.0f;
        }
    }
    if (strcmp(input, "q\n") != 0)
        goto start;

    ma_device_uninit(&device);

    return 0;
}
