#include <music.h>
#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include <errno.h>
#include <regex.h>
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include <sys/types.h>
#include <dirent.h>

ma_result result;
ma_decoder decoder;
ma_device_config deviceConfig;
ma_device device;

char music_dir[32] = "./music";
list_t *music_list;

void init_music()
{
    music_list = create_list(sizeof(char) * 32);
    regex_t music_reg;

    DIR *dirp;
    struct dirent *dp;
    if (regcomp(&music_reg, "\\.(wav|mp3)$", REG_EXTENDED) != 0)
    {
        fprintf(stderr, "replacement_reg is not compliled successfully.\n");
        exit(1);
    }

    if ((dirp = opendir(music_dir)) == NULL)
    {
        perror("couldn't open '.' \n ");
        return;
    }

    do
    {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            if (regexec(&music_reg, dp->d_name, 0, NULL, 0) == 0)
            {
                printf("music file: %s\n", dp->d_name);
                push_item(music_list, dp->d_name);
            }
        }
    } while (dp != NULL);
    closedir(dirp);
}

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
    if (pDecoder == NULL)
    {
        return;
    }

    /* Reading PCM frames will loop based on what we specified when called ma_data_source_set_looping(). */
    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}

void play_music(char *music)
{
    result = ma_decoder_init_file("sample-3s.mp3", NULL, &decoder);
    if (result != MA_SUCCESS)
    {
        fprintf(stderr, "Failed in initialize decoder");
        return;
    }

    /*
    A decoder is a data source which means we just use ma_data_source_set_looping() to set the
    looping state. We will read data using ma_data_source_read_pcm_frames() in the data callback.
    */
    ma_data_source_set_looping(&decoder, MA_TRUE);

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
    {
        fprintf(stderr, "Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return;
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
        fprintf(stderr, "Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return;
    }

    // TODO: implement
}
void stop_music()
{
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
    // TODO: implement
}