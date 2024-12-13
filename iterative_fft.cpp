#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <algorithm>
#include <iomanip>
#include <limits>

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

#define WINDOW_SIZE 4096
#define STEP_SIZE 1024
const double PI = acos(-1);

void iterative_fft(std::vector<std::complex<double>>& data) {
    size_t N = data.size();

    for (size_t i = 1, j = 0; i < N; ++i) {
        size_t bit = N >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) std::swap(data[i], data[j]);
    }


    for (size_t len = 2; len <= N; len <<= 1) {
        double angle = -2 * PI / len;
        std::complex<double> wlen(cos(angle), sin(angle));
        for (size_t i = 0; i < N; i += len) {
            std::complex<double> w(1);
            for (size_t j = 0; j < (len >> 1) ; ++j) {
                std::complex<double> u = data[i + j];
                std::complex<double> v = data[i + j + (len >> 1)] * w;
                data[i + j] = u + v;
                data[i + j + (len >> 1)] = u - v;
                w *= wlen;
            }
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    mp3dec_t mp3d;
    mp3dec_file_info_t info;
    if (mp3dec_load(&mp3d, "input.mp3", &info, NULL, NULL)) {
        std::cerr << "Decode error" << std::endl;
        return 1;
    }

    std::vector<short> signal(info.buffer, info.buffer + info.samples);
    free(info.buffer);

    std::vector<double> hann_window(WINDOW_SIZE);
    for (size_t i = 0; i < WINDOW_SIZE; ++i) {
        hann_window[i] = 0.5 * (1 - cos(2 * PI * i / (WINDOW_SIZE - 1)));
    }

    std::vector<double> windowed_signal(WINDOW_SIZE);

    for (size_t start = 0; start + WINDOW_SIZE <= signal.size(); start += STEP_SIZE) {
        for (size_t i = 0; i < WINDOW_SIZE; ++i) {
            windowed_signal[i] = signal[start + i] * hann_window[i];
        }

        std::vector<std::complex<double>> complex_signal(WINDOW_SIZE);
        for (size_t i = 0; i < WINDOW_SIZE; ++i) {
            complex_signal[i] = std::complex<double>(windowed_signal[i], 0.0);
        }

        iterative_fft(complex_signal);

        double max_amplitude = 0.0;
        for (size_t k = 0; k < WINDOW_SIZE / 2; ++k) {
            double magnitude = std::abs(complex_signal[k]);
            if (magnitude > max_amplitude) {
                max_amplitude = magnitude;
            }
        }

        std::cout << std::fixed << std::setprecision(20) << max_amplitude << '\n';
    }

    return 0;
}
