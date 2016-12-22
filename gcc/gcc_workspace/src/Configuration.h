#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

/* ���������־���� */
#define DUMP_TAG_INFO_ENABLED_CONSOLE 0					//�ڿ���̨�������Tag��Ϣ
#define DUMP_VIDEO_TAG_INFO_ENABLED_CONSOLE 0			//����̨���������ƵTag��ϸ��Ϣ

#define DUMP_TAG_INFO_ENABLED_LOG 1						//����־�ļ������Tag��Ϣ
#define DUMP_VIDEO_TAG_INFO_ENABLED_LOG 1				//����־�ļ��������ƵTag��ϸ��Ϣ

#define DUMP_VIDEO_TAG_PAYLOAD_INFO 1
#define DUMP_DECODER_CONFIG_RECORD_LOG 1				//����־�ļ������AVCDecoderConfigurationRecord��Ϣ
#define DUMP_NAL_UNIT_INFO_LOG 1						//����־�ļ������NAL Unit��Ϣ

#define DUMP_AUDIO_TAG_INFO_ENABLED_LOG 1				//����־�ļ��������ƵTag��ϸ��Ϣ

#define DUMP_ONLY_VIDEO_TAG_INFO 1						//ֻ�����ƵTag����Ϣ

#define DUMP_SCRIPT_INFO_ENABLED_LOG 1					//����־�ļ��������Ƶscript��Ϣ
#define DUMP_SCRIPT_INFO_ENABLED_CONSOLE 1				//����̨�������Script Tag��ϸ��Ϣ

typedef struct RuntimeConfiguration
{
	UINT8 flvLogLevel;		// 0:Flv tag;	1:Video/Audio tag;	2:Video/Audio payload
	RuntimeConfiguration()
	{
		flvLogLevel = 0;
	}
} RuntimeConfiguration;

#endif // _CONFIGURATION_H_