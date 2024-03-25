# Prelab for Lab 5: File I/O and Audio TX

The overall lab deals with audio output. Since audio transmission involves streaming data continuously, timliness and buffer issues will become important. This lab will use file I/O to read and decode a WAVE file and play its content. 

Please review the hardware and the overall overview of the [audio setup](https://neu-ece-4534.github.io/audio.html). In short, to stream audio samples to the CODEC, we will use an AXI FIFO instance as a buffer. You will write audio samples to the FIFO and they will be buffered and passed on to the CODEC, ultimately producing audio output.

# Reading List

1. [WAVE file format specification](http://soundfile.sapp.org/doc/WaveFormat/)

2. [AXI4-Stream FIFO v4.1](https://www.xilinx.com/support/documentation/ip_documentation/axi_fifo_mm_s/v4_1/pg080-axi-fifo-mm-s.pdf)

3. Analog Devices, ADAU1761: SigmaDSP® Stereo, Low Power, 96 kHz, 24-Bit Audio Codec with Integrated PLL Data Sheet (Rev. C), [http://www.analog.com/media/en/technical-documentation/data-sheets/ADAU1761.pdf](http://www.analog.com/media/en/technical-documentation/data-sheets/ADAU1761.pdf)

# Pre-lab assignment

The instructions for ths lab are detailed in the following steps:

1. (Reserved for feedback branch pull request. You will receive top level feedback there). 
2. [WAV File Format](.github/STARTING_ISSUES/2.%20WAV%20File%20Format.md)
3. [Reading Structured Data](.github/STARTING_ISSUES/3.%20Reading%20Structured%20Data.md)
4. [Parsing File Header](.github/STARTING_ISSUES/4.%20Parsing%20File%20Header.md)
5. [Sample Conversion](.github/STARTING_ISSUES/5.%20Sample%20Conversion.md)
6. [AXI FIFO Interfacing](.github/STARTING_ISSUES/6.%20AXI%20FIFO%20Interfacing.md)
7. [AXI FIFO Polling Delay](.github/STARTING_ISSUES/7.%20AXI%20FIFO%20Polling%20Delay.md)

After accepting this assignment in github classroom, each step is converted into a [github issue](https://docs.github.com/en/issues). Follow the issues in numerically increasing issue number (the first issue is typically on the bottom of the list). 

## General Rules

Please commit your code frequently or at e very logical break. Each commit should have a meaningful commit message and [cross reference](https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/autolinked-references-and-urls#issues-and-pull-requests) the issue the commit belongs to. Ideally, there would be no commits without referencing to a github issue. 

Please comment on each issue with the problems faced and your approach to solve them. Close an issue when done. 
