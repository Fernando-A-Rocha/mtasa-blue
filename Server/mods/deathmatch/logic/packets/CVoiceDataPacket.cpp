/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        mods/deathmatch/logic/packets/CVoiceDataPacket.cpp
 *  PURPOSE:     Voice data packet class
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include "CVoiceDataPacket.h"
#include "CPlayer.h"

CVoiceDataPacket::CVoiceDataPacket()
{
    m_voiceBuffer.reserve(MAX_VOICE_BUFFER_LENGTH);
}

bool CVoiceDataPacket::Read(NetBitStreamInterface& BitStream)
{
    unsigned short voiceBufferLength{};

    // Read the length field
    if (!BitStream.Read(voiceBufferLength))
        return false;

    // Validate length is within acceptable bounds
    if (voiceBufferLength < MIN_VOICE_BUFFER_LENGTH || voiceBufferLength > MAX_VOICE_BUFFER_LENGTH)
        return false;

    // Verify that the BitStream has enough bytes available before attempting to read
    if (!BitStream.CanReadNumberOfBytes(voiceBufferLength))
        return false;

    // All validations passed, now resize buffer and read the data
    m_voiceBuffer.resize(voiceBufferLength);
    return BitStream.Read(reinterpret_cast<char*>(m_voiceBuffer.data()), m_voiceBuffer.size());
}

bool CVoiceDataPacket::Write(NetBitStreamInterface& BitStream) const
{
    if (!m_voiceBuffer.empty())
    {
        const auto voiceBuffer = reinterpret_cast<const char*>(m_voiceBuffer.data());
        const auto voiceBufferLength = static_cast<uint16_t>(m_voiceBuffer.size());

        // Write the source player
        BitStream.Write(m_pSourceElement->GetID());
        // Write the length as an unsigned short and then write the string
        BitStream.Write(voiceBufferLength);
        BitStream.Write(voiceBuffer, voiceBufferLength);
        return true;
    }

    return false;
}

void CVoiceDataPacket::SetVoiceData(const unsigned char* voiceBuffer, unsigned short voiceBufferLength)
{
    m_voiceBuffer.clear();

    // Validate input parameters and length bounds
    if (!voiceBuffer || voiceBufferLength < MIN_VOICE_BUFFER_LENGTH || voiceBufferLength > MAX_VOICE_BUFFER_LENGTH)
        return;

    m_voiceBuffer.resize(voiceBufferLength);
    std::copy_n(voiceBuffer, voiceBufferLength, m_voiceBuffer.data());
}
