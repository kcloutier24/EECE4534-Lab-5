#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

// FIFO stuff

#define FIFO_BASE 0x41630000
#define ISR (FIFO_BASE + 0x0)   // Interrupt Status Register
#define IER (FIFO_BASE + 0x4)   // Interrupt Enable Register
#define TDFR (FIFO_BASE + 0x8)  // Transmit Data FIFO Reset
#define TDFV (FIFO_BASE + 0xC)  // Transmit Data FIFO Vacancy
#define TDFD (FIFO_BASE + 0x10) // Transmit Data FIFO 32-bit Wide Data Write Port
#define TLR (FIFO_BASE + 0x14)  // Transmit Length Register

#define RDFR (FIFO_BASE + 0x18) // Receive Data FIFO reset
#define RFDO (FIFO_BASE + 0x1C) // Receive Data FIFO Occupancy
#define RFDD (FIFO_BASE + 0x20) // Receive Data FIFO 32-bit Wide Data Read Port
#define RLR (FIFO_BASE + 0x24)  // Receive Length Register
#define SRR (FIFO_BASE + 0x28)  // AXI4-Stream Reset
#define TDR (FIFO_BASE + 0x2C)  // Transmit Destination Register
#define RDR (FIFO_BASE + 0x30)  // Receive Destination Register

#define TRANS_ID_REG (FIFO_BASE + 0x34)    // Transmit ID Register
#define TRANS_USR_REG (FIFO_BASE + 0x38)   // Transmit USER Register
#define RECEIVE_ID_REG (FIFO_BASE + 0x3C)  // Receive ID Register
#define RECEIVE_USR_REG (FIFO_BASE + 0x40) // Receive USER Register

// NOTE use sizes from STDINT
// NOTE verify data alignment!
struct wave_header
{
  // TODO STUDENT: populate this struct correctly

  // RIFF header
  uint32_t ChunkID;   //"RIFF" (0x52494646 big-endian form) 4
  uint32_t ChunkSize; // file size - 8 bytes  4
  uint32_t Format;    // "WAVE" (0x57415645 big-endian form)  4

  //"WAVE" format, "fmt" and "data"
  //"fmt" describes the sound data's format
  uint32_t Subchunk1ID;   //"fmt " (0x666d7420 big-endian form) 4
  uint32_t Subchunk1Size; // size of theformat chunk 16 for PCM 4

  uint16_t AudioFormat; // PCM=1  2
  uint16_t NumChannels; // Mono = 1, Stereo = 2, etc. 2

  uint32_t SampleRate; // 8000,44100, etc.  4
  uint32_t ByteRate;   //= SampleRate * NumChannels * BitsPerSample/8  4

  uint16_t BlockAlign;    //= NumChannels * BitsPerSample/8   The number of bytes for one sample including all channels  2
  uint16_t BitsPerSample; // 8 bits = 8, 16 bits = 16, etc.  2

  // data subchunk that contains the size of the data and the actual sound
  uint32_t Subchunk2ID;   //"data" (0x64617461 big-endian form)  4
  uint32_t Subchunk2Size; //= NumSamples * NumChannels * BitsPerSample/8  This is the number of bytes in the data.  4
  // Data;
};

void pr_usage(char *pname)
{
  printf("usage: %s WAV_FILE\n", pname);
}

/* @brief Read WAVE header
   @param fp file pointer
   @param dest destination struct
   @return 0 on success, < 0 on error */
int read_wave_header(FILE *fp, struct wave_header *dest)
{
  if (!dest || !fp)
  {
    return -ENOENT;
  }

  // NOTE do not assume file pointer is at its starting point
  // TODO read header

  fseek(fp, 0, SEEK_SET); // file pointer to the beginning

  size_t read_size = fread(dest, sizeof(struct wave_header), 1, fp); // WAVE header into dest

  // check
  if (read_size != 1)
  {
    {
      printf("Error reading in wave header \n");

      return -EIO; // I/O error
    }

    return 0;
  }

  printf("Wave header read in \n");

  return 0;
}

/* @brief Parse WAVE header and print parameters
   @param hdr a struct wave_header variable
   @return 0 on success, < 0 on error or if not WAVE file*/
int parse_wave_header(struct wave_header hdr)
{

  // TODO verify that this is a RIFF file header

  // ChunkID  "RIFF" (0x52494646 big-endian form) turns into 46464952

  // Validate the format is wav
  if (hdr.ChunkID != 0x46464952)
  {
    fprintf(stderr, "Not RIFF header \n");
    return 1;
  }

  // TODO verify that this is WAVE file
  // Format "WAVE" (0x57415645 big-endian form)  turns into 45564157

  if (hdr.Format != 0x45564157)
  {
    fprintf(stderr, "Not WAVE file \n");
    return 1;
  }

  // AudioFormat PCM=1  2
  if (hdr.AudioFormat != 1)
  {
    fprintf(stderr, "Not in PCM format \n");
    return 1;
  }

  // TODO print out information: number of channels, sample rate, total size

  printf("Number of channels: %u \n", hdr.NumChannels);
  printf("Sample rate in Hz: %u \n", hdr.SampleRate);
  printf("Bits per sample: %u \n", hdr.BitsPerSample);

  return 0;
}

/*
Write the FIFO registers directly as described in the Programming Sequence section of the datasheet.
Resetting will empty stale/old FIFO contents (if any).
*/

void fifo_init_reset(void)
{

  (*(volatile uint32_t *)TDFR) = SRR;
  (*(volatile uint32_t *)RDFR) = SRR;
}

/* @brief Check if FIFO is full
   @return 0 if not full, "true" otherwise */
unsigned char fifo_full(void)
{
  // TODO implement this
  // Read the FIFO full status register
  uint32_t status = ((volatile uint32_t)TDFV);

  // Check if the FIFO full bit is set
  if (status == 0)
    return 1; // full
  else
    return 0; // not full
}

/* @brief Transmit a word (put into FIFO)
   @param word a 32-bit word */
void fifo_transmit_word(uint32_t word)
{
  // TODO implement this
  // NOTE block if full

  while (fifo_full())

  {
    // Write the 32-bit word to the TX data register
    *((volatile uint32_t *)TDFD) = word;

    printf("Transmitting word %08x \n", word);

    // usleep(41900); //0.0419 seconds
  }
}

/* @brief Build a 32-bit audio word from a buffer
   @param hdr WAVE header
   @param buf a byte array
   @return 32-bit word */
uint32_t audio_word_from_buf(struct wave_header hdr, uint8_t *buf)
{
  // TODO build word depending on bits per sample, etc
  uint32_t audio_word = 0;

  // The AXI FIFO expects samples as 32-bit words interleaved stereo.
  // This means, the first 32-bit word is the first sample for the left channel,
  // the second 32-bit word is the first sample for the right channel.

  if (hdr.BitsPerSample == 8)
  {
    audio_word = (uint32_t)buf[0];          // Left channel
    audio_word |= ((uint32_t)buf[1] << 16); // Right channel
  }
  else if (hdr.BitsPerSample == 16)
  {
    audio_word = *((uint16_t *)buf);                // Left channel
    audio_word |= (*((uint16_t *)(buf + 2)) << 16); // Right channel
  }
  else if (hdr.BitsPerSample == 24)
  {

    audio_word = ((uint32_t)buf[0] << 8) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 24); // Left channel
    audio_word |= ((uint32_t)buf[3] << 8) | ((uint32_t)buf[4] << 24);                           // Right channel
  }
  else if (hdr.BitsPerSample == 32)
  {
    audio_word = ((uint32_t)buf);                // Left channel
    audio_word |= (((uint32_t)(buf + 4)) << 16); // Right channel
  }

  return audio_word;
}

/* @brief Play sound samples
   @param fp file pointer
   @param hdr WAVE header
   @param sample_count how many samples to play or -1 plays to end of file
   @param start starting point in file for playing
   @return 0 if successful, < 0 otherwise */
int play_wave_samples(FILE *fp,
                      struct wave_header hdr,
                      int sample_count,
                      unsigned int start)
{

  if (!fp)
  {
    printf("Error \n");
    return -EINVAL;
  }

  // NOTE reject if number of channels is not 1 or 2
  // TODO calculate starting point and move there
  if (hdr.NumChannels != 1 && hdr.NumChannels != 2)
  {
    printf("Error \n");

    return -1; // Error: Unsupported number of channels
  }
  unsigned int start_byte = hdr.NumChannels * (hdr.BitsPerSample / 8);
  printf("start_byte  %d \n", start_byte);

  // the starting point
  fseek(fp, start_byte, SEEK_SET);

  // TODO continuously read frames/samples and use fifo_transmit_word to
  //      simulate transmission
  while (sample_count > 0)
  {
    // TODO read chunk (whole frame)
    // printf( "Sample count: %d \n", sample_count);

    uint8_t buf[4]; // one channel

    size_t read_size = fread(buf, 1, hdr.NumChannels * (hdr.BitsPerSample / 8), fp);

    if (read_size != hdr.NumChannels * (hdr.BitsPerSample / 8))
    {
      printf("Error reading samples from file\n");
      return -1;
    }
    // printf( "test:  \n");

    // TODO write samples properly independently if file is mono or stereo
    //  uint16_t NumChannels; // Mono = 1, Stereo = 2, etc. 2

    if (hdr.NumChannels == 1) // mono
    {

      uint32_t audio_word = audio_word_from_buf(hdr, buf);
      fifo_transmit_word(audio_word);
    }
    else // stereo
    {

      uint32_t left_channel = audio_word_from_buf(hdr, buf);
      fifo_transmit_word(left_channel);

      // Move the file pointer to the next channel's sample
      fseek(fp, hdr.BitsPerSample / 8, SEEK_CUR);

      // Convert and transmit the right channel sample
      uint32_t right_channel = audio_word_from_buf(hdr, buf + (hdr.BitsPerSample / 8));
      fifo_transmit_word(right_channel);
      // next
    fseek(fp, hdr.NumChannels * (hdr.BitsPerSample / 8), SEEK_CUR);
    }
    
    // printf( "Sample count: %d \n", sample_count);

    // update sample count
    sample_count--;
  }

  return 0;
}

int main(int argc, char **argv)
{
  FILE *fp;
  struct wave_header hdr;

  // first validation to check size of the struct, should be 44
  printf("Size of the wave_header struct in bytes: %zu \n", sizeof(struct wave_header));

  // check number of arguments
  if (argc < 2)
  {
    // fail, print usage
    pr_usage(argv[0]);
    // return 1;
  }

  // TODO open file

  fp = fopen("hal_9000.wav", "rb");
  if (!fp)
  {
    printf("Error opening file \n");
    return 1;
  }

  // TODO read file header

  int read_result = read_wave_header(fp, &hdr);

  // check
  if (read_result != 0)
  {
    printf("Error reading WAVE header: %d\n", read_result);
    fclose(fp);
    return 1;
  }

  // ChunkID  "RIFF" (0x52494646 big-endian form) turns into 46464952
  // Format "WAVE" (0x57415645 big-endian form)  turns into 45564157

  // Validate the format is wav
  if (hdr.ChunkID != 0x46464952 || hdr.Format != 0x45564157)
  {
    printf("Invalid WAV format\n");
    fclose(fp);
    return 1;
  }

  // Get the file size from the header
  uint32_t file_size = hdr.ChunkSize + 8;

  // Print obtained file size and compare with file size on the file system
  printf("Obtained file size from header: %u bytes\n", file_size);

  // TODO parse file header, verify that is wave

  int parse_results = parse_wave_header(hdr);

  if (parse_results != 0)
  {
    printf("Error parsing WAVE header: %d \n", parse_results);
    fclose(fp);
    return 1;
  }

  // TODO play one second from start of file and stop

  int first_sample_count = hdr.SampleRate * 1; // one second worth of samples
  int start_point = 0;                         // start from beginning
  printf("First second \n");
  int first_play_result = play_wave_samples(fp, hdr, first_sample_count, start_point);

  // check if playing was successful
  if (first_play_result != 0)
  {
    printf("Error playing audio: %d\n", first_play_result);
    fclose(fp);
    return 1;
  }

  // printf("rate %d channel %d",hdr.SampleRate, sample_count);

  // TODO play last one second of file

  int total_duration = sizeof(struct wave_header) / (hdr.SampleRate * hdr.NumChannels * (hdr.BitsPerSample / 8));

  int last_sample_count = hdr.SampleRate * 1; //  One second of audio
  int end = total_duration - 1;               // Starting point for the last second
  printf("Last second \n");
  int last_play_result = play_wave_samples(fp, hdr, last_sample_count, end);

  // Check if playing audio was successful
  if (last_play_result != 0)
  {
    fprintf(stderr, "Error playing audio: %d\n", last_play_result);
    fclose(fp);
    return 1;
  }

  // TODO cleanup, uninitialize

  // Close the file
  fclose(fp);
  return 0;
}
