#include "VoidNet_HL/NetworkMessage.hpp"
#include <VoidNet_HL\NetworkHeader.hpp>

namespace std::net
{
	uint32_t NetworkMessage::GetSenderID() const
	{
		return m_senderID;
	}

	DistributionMode NetworkMessage::GetDistributionMode() const
	{
		return m_distributionMode;
	}

	uint32_t NetworkMessage::GetDestinationID() const
	{
		return m_destinationID;
	}

	uint32_t NetworkMessage::GetTag() const
	{
		return m_tag;
	}

	byte *NetworkMessage::SerializeData(uint32_t &size) const
	{
		int32_t sizeOfNetHeader = sizeof(NetworkHeader);

		NetworkHeader header;
		header.Size = 13 + sizeOfNetHeader + m_dataSize;

		byte *bytes = new byte[header.Size]();
		memcpy(bytes, &header, sizeOfNetHeader);

		byte *sender = ByteConverter::ToBytes<uint32_t>(m_senderID); // 4
		byte *destination = ByteConverter::ToBytes<uint32_t>(m_destinationID); // 4
		byte *tag = ByteConverter::ToBytes<uint32_t>(m_tag); // 4

		memcpy(bytes + sizeOfNetHeader, sender, 4);
		bytes[sizeOfNetHeader + 4] = (byte)m_distributionMode;
		memcpy(bytes + sizeOfNetHeader + 5, destination, 4);
		memcpy(bytes + sizeOfNetHeader + 9, tag, 4);

		memcpy(bytes + 13 + sizeOfNetHeader, m_data, m_dataSize);

		size = header.Size;
		return bytes;
	}

	void NetworkMessage::Deserialize(byte *data, uint32_t size)
	{
		NetworkHeader header;
		memcpy(&(header), data, sizeof(NetworkHeader));

		data += sizeof(NetworkHeader);

		DeserializeWithoutHeader(data, header.Size);
	}
	void NetworkMessage::DeserializeWithoutHeader(byte* data, uint32_t size)
	{
		memcpy(&(m_senderID), data, 4);
		m_distributionMode = (DistributionMode)data[4];
		memcpy(&(m_destinationID), data + 5, 4);
		memcpy(&(m_tag), data + 9, 4);

		if (size - 4 > 13)
		{
			m_data = data + 13;
			m_dataSize = size - 13 - 4;
		}
	}
}