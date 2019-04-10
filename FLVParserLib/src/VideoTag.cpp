#include "FLVParserLib.h"
#include "VideoTag.h"

using namespace std;

CVideoTag::CVideoTag(UINT32 idx, UINT8 *buf, UINT8 type) : CFlvTag(idx, buf, type)
{
	m_frameType = 0;
	m_codecID = 0;
	m_AVCPacketType = 0;
	m_CompositionTime = 0;

	m_decCfgRcrd = NULL;
	m_nalu = NULL;
}

CVideoTag::~CVideoTag()
{
	if (m_decCfgRcrd)
	{
		delete m_decCfgRcrd;
		m_decCfgRcrd = NULL;
	}

	if (m_nalu)
	{
		NALUnit *currentUnit = m_nalu, *nextUnit = currentUnit->nextNalu;
		do 
		{
			delete currentUnit;
			currentUnit = nextUnit;
			if (nextUnit)
			{
				nextUnit = nextUnit->nextNalu;
			}
		} 
		while (currentUnit);
	}
}

int CVideoTag::Parse_video_tag_detail()
{
	int err = 0;

	m_frameType = m_tagBuffer[11] >> 4;
	if (m_frameType < 1 || m_frameType > 5)
	{
		return kFlvParserError_IllegalVideoFrameType;
	}
	m_codecID = m_tagBuffer[11] & 0x0f;
	if (m_codecID != 7)
	{
		return kFlvParserError_UnsupportedVideoCodecID;
	}
	m_AVCPacketType = m_tagBuffer[12];
	Read_data_lsb(&m_CompositionTime, m_tagBuffer + 13, 3);

	if (m_AVCPacketType == 0)
	{
		m_decCfgRcrd = new AVCDecoderConfigurationRecord;
		err = m_decCfgRcrd->Parse_as_configuration_record(m_tagBuffer + 16);
		if (err < 0)
		{
			return err;
		}
	}
	else if (m_AVCPacketType == 1)
	{
		err = parse_nal_units();
		if (err < 0)
		{
			return err;
		}
	}

	return kFlvParserError_NoError;
}

void CVideoTag::Dump_video_tag_info()
{
#if DUMP_TAG_INFO_ENABLED_LOG

#if DUMP_VIDEO_TAG_INFO_ENABLED_LOG
	if (g_flvConfig.flvLogLevel >= 1)
	{
		g_logoutFile << "Frame Type: " << (m_frameType == 1 ? "Key Frame" : "Inter Frame") << endl;
		g_logoutFile << "Codec ID: " << to_string(m_codecID) << endl;
		g_logoutFile << "AVCPacketType: " << to_string(m_AVCPacketType) << endl;
		g_logoutFile << "CompositionTime: " << to_string(m_CompositionTime) << endl;
	}	
#endif

#if DUMP_VIDEO_TAG_PAYLOAD_INFO
	dump_video_payload_info();
#endif

#endif
}

void CVideoTag::dump_video_payload_info()
{
	if (g_flvConfig.flvLogLevel == 2)
	{
		if (m_AVCPacketType == 0)
		{
#if DUMP_DECODER_CONFIG_RECORD_LOG
			g_logoutFile << "Configuration Version: " << to_string(m_decCfgRcrd->configurationVersion) << endl;
			g_logoutFile << "AVC Profile: " << to_string(m_decCfgRcrd->AVCProfileIndication) << endl;
			g_logoutFile << "Profile Compatibility: " << to_string(m_decCfgRcrd->profile_compatibility) << endl;
			g_logoutFile << "AVC Level: " << to_string(m_decCfgRcrd->AVCLevelIndication) << endl;
			g_logoutFile << "NALU Length Size: " << to_string(m_decCfgRcrd->lengthSize) << endl;
			g_logoutFile << "Number of SPS: " << to_string(m_decCfgRcrd->numSPS) << endl;
			g_logoutFile << "Number of PPS: " << to_string(m_decCfgRcrd->numPPS) << endl;
#endif
		}
		else
		{
#if DUMP_NAL_UNIT_INFO_LOG
			dump_nal_units_info();
#endif
		}
	}	
}

int CVideoTag::parse_nal_units()
{
	int err = 0;
	BYTE *nalsBuffer = m_tagBuffer + 16;
	UINT32 totalNalLength = 0, naluSize = 0;
	NALUnit *currentUnit = NULL;

	while (totalNalLength < m_dataSize - 5)
	{
		NALUnit *tempUnit = new NALUnit;
		err = tempUnit->Get_nal_unit(nalsBuffer + totalNalLength, false,m_tagMaxSize);
		if (err < 0)
		{
			return err;
		}
		tempUnit->Parse_nal();
		totalNalLength += (tempUnit->naluLength + 4);

		if (!m_nalu)
		{
			m_nalu = tempUnit;
			currentUnit = m_nalu;
		}
		else
		{
			currentUnit->nextNalu = tempUnit;
			currentUnit = tempUnit;
		}
	}

	return kFlvParserError_NoError;
}

void CVideoTag::dump_nal_units_info()
{
	NALUnit *unit = m_nalu;
	const char *sliceTypeString[5] = 
	{
		"P Slice",
		"B Slice",
		"I Slice",
		"SP Slice",
		"SI Slice"
	};

	while (unit)
	{
		g_logoutFile << "NAL Unit Type: " << to_string(unit->nalType) << endl;
		if (unit->nalType == 5 || unit->nalType == 1)
		{
			g_logoutFile << "Slice Type: " << sliceTypeString[unit->sliceType % 5] << endl;
		}
		unit = unit->nextNalu;
	}
}
