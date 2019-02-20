#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/msg.h> 
#include "network.h"
#include "4tbox_define.h"
#include 	 "ql_oe.h"
#include "general.h"

#define quec_log(...) fprintf(stderr, __VA_ARGS__)
#define QLLOG(...)    fprintf(stderr, __VA_ARGS__)
extern int voice_num;
static int __ql_wav_pcm16Le_check(int fd)
{
    struct wav_header hdr;
    int offset;

    if (read(fd, &hdr, sizeof(hdr)) != sizeof(hdr)) {
        QLLOG("%s: cannot read header\n", __FUNCTION__);
        return -1;
    }

    QLLOG("read wav hdr\n");
    if ((hdr.riff_id != ID_RIFF)
        || (hdr.riff_fmt != ID_WAVE)
        || (hdr.fmt_id != ID_FMT)) {
        QLLOG("%s:  is not a riff/wave file\n", __FUNCTION__);
        return -1;
    }
        
    if ((hdr.audio_format != FORMAT_PCM) || (hdr.fmt_sz != 16)) {
        QLLOG("%s: is not pcm format\n", __FUNCTION__);
        return -1;
    }
        
    if (hdr.bits_per_sample != 16) {
        QLLOG("%s: is not 16bit per sample\n", __FUNCTION__);
        return -1;
    }

    offset = lseek(fd, 0, SEEK_CUR);
    QLLOG("get wav hdr offset\n");
    return offset;
}


static int __ql_wav_amr_check(int fd)
{
    return 0;
}


static int __ql_wav_pcm16Le_set(int fd)
{
	struct wav_header hdr;

    memset(&hdr, 0, sizeof(struct wav_header));

    hdr.riff_id = ID_RIFF;
	hdr.riff_fmt = ID_WAVE;
	hdr.fmt_id = ID_FMT;
	hdr.fmt_sz = 16;
	hdr.audio_format = FORMAT_PCM;
	hdr.num_channels = 1;
	hdr.sample_rate = 8000;
	hdr.bits_per_sample = 16;
	hdr.byte_rate = (8000 * 1 * hdr.bits_per_sample) / 8;
	hdr.block_align = (hdr.bits_per_sample * 1) / 8;
	hdr.data_id = ID_DATA;
	hdr.data_sz = 0;

	hdr.riff_sz = hdr.data_sz + 44 - 8;
	if (write(fd, &hdr, sizeof(hdr)) != sizeof(hdr)) {
        return -1;
	}

    return 0;
}


static int __ql_wav_amr_set(int fd)
{
    return 0;
}

/**
 * param '*offset' value is the first wav data position
 * @Note if offset == NULL, then 'offset' should be -1 when call Ql_AudioPlayFileStart()
 */
int Ql_PcmFileOpen(char *file, struct ST_MediaParams *mediaParams, int *offset)
{
    int fd;
    int ret;

    if (file == NULL && mediaParams == NULL) {
        QLLOG("%s: args invalid\n", __FUNCTION__);
        goto openfile_failed;
    }

    fd = open(file, O_RDONLY);
    if (fd < 0) {
        QLLOG("%s: open file failed\n", __FUNCTION__);
        goto openfile_failed;
    }

    /* check media params */
    switch (mediaParams->format) {
    case AUD_STREAM_FORMAT_AMR:
        __ql_wav_amr_check(fd);
        break;

    case AUD_STREAM_FORMAT_PCM:
        ret = __ql_wav_pcm16Le_check(fd);
        if (ret < 0) {
            QLLOG("parseMediafilePcm16Le failed\n");
            goto openfile_failed;
        }
        if (offset != NULL) {
            *offset = ret;
        }
        break;

    default:
        QLLOG("parse Unknown File\n");
        goto openfile_failed;
    }

    return fd;
openfile_failed:
    return -1;
}


int Ql_PcmFileCreat(char *file, struct ST_MediaParams *mediaParams, int *offset)
{
    int fd = -1;
    int ret = -1;

    if(access(file, 0) == 0)	{
		QLLOG("%s: file already exist.\n", __FUNCTION__);
		return -1;
	}

    fd = open(file, O_RDWR | O_CREAT, 0x0666);
    if (fd < 0) {
        QLLOG("%s: open file failed\n", __FUNCTION__);
        return -1;
    }

    switch (mediaParams->format) {
    case AUD_STREAM_FORMAT_AMR:
        ret = __ql_wav_amr_set(fd);
        break;

    case AUD_STREAM_FORMAT_PCM:
        ret = __ql_wav_pcm16Le_set(fd);
        break;

    default:
        ret = -1;
        break;
    }

    if (ret < 0) {
        close(fd);
        return -1;
    }

    return fd;
}

static int play1Flag = 0;

int Ql_cb_Player1(int hdl, int result)
{
	quec_log("%s: hdl=%d, result=%d\n\r", __func__, hdl, result);
	if (result == AUD_PLAYER_FINISHED || result == AUD_PLAYER_NODATA)
	{
		quec_log("%s: play finished\n\r", __func__);
        play1Flag = 1;
    }
    return 0;
}



// Usage:
//   Play the specified audio file: 
//        <process> play1 <audio file>

int audio_play(char *pathname)
{
    int iRet;
    int devphdl1;//paly1
    int filefd1;
    struct ST_MediaParams mediaParas;  

	mediaParas.format = AUD_STREAM_FORMAT_PCM;
    filefd1 = Ql_PcmFileOpen(pathname, &mediaParas, NULL);

        devphdl1 = Ql_AudPlayer_Open(NULL, Ql_cb_Player1);
        if (devphdl1 < 0) {
            quec_log("open aud play%d failed\n\r", devphdl1);
            return 0;
        }

        play1Flag = 0;
        iRet = Ql_AudPlayer_PlayFrmFile(devphdl1, filefd1, -1);
        if (iRet != 0) {
            quec_log("start aud play%d failed\n\r", devphdl1);
        }

        while (play1Flag != 1) {
            sleep(1);
        }

        Ql_AudPlayer_Stop(devphdl1);
        Ql_AudPlayer_Close(devphdl1);
    
		return 1;
}

int audio_thread()
{
	int msgid = -1;  

	struct mcu_msg_stru msg;  

	msgid = msgget((key_t)MSG_ID_AUDIO,IPC_EXCL);
	if(msgid == -1)
	{ 
		Log(__FUNCTION__,"msgget failed");  
	}

	while(1)
	{
		if(msgrcv(msgid, (void*)&msg, sizeof(struct mcu_msg_stru), 0, 0) == -1)
		{
		     Log(__FUNCTION__,"msgrcv failed\n");  
		}//end if
		else
		{
			Log_print(__FUNCTION__,"CMD = %d\n",msg.data[0]);
			Ql_GPIO_SetLevel(GPIO_BIG_VOICE, PINLEVEL_HIGH);
			usleep(150000);
			Ql_GPIO_SetLevel(GPIO_MUTE, PINLEVEL_LOW);
			switch(msg.data[0])
			{
			case CMD_GET_RIGHT:
				QL_TTS_client_play(QL_TTS_PLAY_MODE_NORMAL,QL_TTS_DATA_MODE_UCS,UNICODE_GET_RIGHT,strlen(UNICODE_GET_RIGHT));
				break;
			case CMD_PULL_CHARGE:
				QL_TTS_client_play(QL_TTS_PLAY_MODE_NORMAL,QL_TTS_DATA_MODE_UCS,UNICODE_PULL_CHARGE,strlen(UNICODE_PULL_CHARGE));
				break;
			case CMD_PUSH_RED:
				QL_TTS_client_play(QL_TTS_PLAY_MODE_NORMAL,QL_TTS_DATA_MODE_UCS,UNICODE_PULL_RED,strlen(UNICODE_PULL_RED));
				break;
			case CMD_OVER_SPEED:
				QL_TTS_client_play(QL_TTS_PLAY_MODE_NORMAL,QL_TTS_DATA_MODE_UCS,UNICODE_OVER_SPEED,strlen(UNICODE_OVER_SPEED));
				break;
			case CMD_NEED_PARK:
				QL_TTS_client_play(QL_TTS_PLAY_MODE_NORMAL,QL_TTS_DATA_MODE_UCS,UNICODE_NEED_PARK,strlen(UNICODE_NEED_PARK));
				break;
			case CMD_ENGIN_START:
				QL_TTS_client_play(QL_TTS_PLAY_MODE_NORMAL,QL_TTS_DATA_MODE_UCS,UNICODE_ENGIN_START,strlen(UNICODE_ENGIN_START));
				break;
			case CMD_WECHAT_PAY:
				QL_TTS_client_play(QL_TTS_PLAY_MODE_NORMAL,QL_TTS_DATA_MODE_UCS,UNICODE_WECHAT_PAY,strlen(UNICODE_WECHAT_PAY));
				break;
			case CMD_OUT_OF_RANGE:
				QL_TTS_client_play(QL_TTS_PLAY_MODE_NORMAL,QL_TTS_DATA_MODE_UCS,UNICODE_OUT_OF_RANGE,strlen(UNICODE_OUT_OF_RANGE));
				break;
			}
			ql_tts_client_state_e_type state;
			ql_tts_client_state_e_type finish = QL_TTS_EVT_PLAY_FINISH;
			ql_tts_client_state_e_type init = QL_TTS_EVT_INIT;
			ql_tts_client_state_e_type deinit = QL_TTS_EVT_DEINIT;
			ql_tts_client_state_e_type failed = QL_TTS_EVT_PLAY_FAILED;			
			do{
				QL_TTS_client_state_get(&state);
				Log(__FUNCTION__,"playing %d",state);
				usleep(300000);
			}while((state != finish) && (state != init) && 
				   (state != deinit) && (state != failed));
			Ql_GPIO_SetLevel(GPIO_MUTE, PINLEVEL_HIGH);
			usleep(100000);
			Ql_GPIO_SetLevel(GPIO_BIG_VOICE, PINLEVEL_LOW);
		}
		Log_print(__FUNCTION__,"in audio while");
	}//end while
	return 1;
}
