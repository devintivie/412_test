/*
 * audio_test.c
 *
 *  Created on: Sep 13, 2020
 *      Author: devin
 */


#include "audio_test.h"

#include "fatfs.h"


/*Return 0 on success and -1 on failure*/
int write_PCM16_stereo_header(FILE* file_p, int32_t SampleRate, int32_t FrameCount)
{
//    int ret;
    FRESULT ret;

    wavfile_header_t wav_header;
    int32_t subchunk2_size;
    int32_t chunk_size;

    size_t write_count;

    subchunk2_size  = FrameCount * NUM_CHANNELS * BITS_PER_SAMPLE/8;
    chunk_size      = 4 + (8 + SUBCHUNK1SIZE) + (8 + subchunk2_size);

    wav_header.ChunkID[0] = 'R';
    wav_header.ChunkID[1] = 'I';
    wav_header.ChunkID[2] = 'F';
    wav_header.ChunkID[3] = 'F';

    wav_header.ChunkSize = chunk_size;

    wav_header.Format[0] = 'W';
    wav_header.Format[1] = 'A';
    wav_header.Format[2] = 'V';
    wav_header.Format[3] = 'E';

    wav_header.Subchunk1ID[0] = 'f';
    wav_header.Subchunk1ID[1] = 'm';
    wav_header.Subchunk1ID[2] = 't';
    wav_header.Subchunk1ID[3] = ' ';

    wav_header.Subchunk1Size = SUBCHUNK1SIZE;
    wav_header.AudioFormat = AUDIO_FORMAT;
    wav_header.NumChannels = NUM_CHANNELS;
    wav_header.SampleRate = SampleRate;
    wav_header.ByteRate = BYTE_RATE;
    wav_header.BlockAlign = BLOCK_ALIGN;
    wav_header.BitsPerSample = BITS_PER_SAMPLE;

    wav_header.Subchunk2ID[0] = 'd';
    wav_header.Subchunk2ID[1] = 'a';
    wav_header.Subchunk2ID[2] = 't';
    wav_header.Subchunk2ID[3] = 'a';
    wav_header.Subchunk2Size = subchunk2_size;

    UINT byteCount;
	ret = f_write(file_p, &wav_header, sizeof(wavfile_header_t), &byteCount);
	printf("header write result = %d\r\n", ret);
//	ret = f_close(file_p);
//	printf("header close result = %d\r\n", ret);
    return ret;
}
PCM16_stereo_t *allocate_PCM16_stereo_buffer(   int32_t FrameCount)
{
    return (PCM16_stereo_t *)malloc(sizeof(PCM16_stereo_t) * FrameCount);
}

/*Return the number of audio frames sucessfully written*/
size_t  write_PCM16wav_data(FILE* file_p, int32_t FrameCount, PCM16_stereo_t  *buffer_p)
{
    size_t ret;
    ret = fwrite(buffer_p, sizeof(PCM16_stereo_t), FrameCount, file_p);
    return ret;
}
