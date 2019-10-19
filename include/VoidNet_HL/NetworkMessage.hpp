#pragma once

#include "VoidNet_LL/Net.hpp"
#include "VoidNet_HL/NetworkHeader.hpp"
#include "VoidNet_HL/ByteConverter.hpp"

namespace std::net
{
	enum class DistributionMode : uint8_t
	{
		ID,
		Others,
		OthersAndServer,
		All,
		AllAndServer,
		Server
	};

	class NetworkMessage
	{
	public:
		NetworkMessage()
		{
		}

		NetworkMessage(uint32_t sender, DistributionMode mode, uint32_t destinationId, uint32_t tag, void *data, uint32_t dataSize)
			: m_senderID(sender)
			, m_distributionMode(mode)
			, m_destinationID(destinationId)
			, m_tag(tag)
			, m_data(data)
			, m_dataSize(dataSize)
		{
		}

		uint32_t GetSenderID() const;
		DistributionMode GetDistributionMode() const;
		uint32_t GetDestinationID() const;
		uint32_t GetTag() const;

	private:
		uint32_t m_senderID = 0;
		DistributionMode m_distributionMode = DistributionMode::All;
		uint32_t m_destinationID = 0;
		uint32_t m_tag = 0;

		void *m_data = 0;
		uint32_t m_dataSize = 0;
	
	public:
		template<typename T>
		void SetData(T *data)
		{
			m_data = data;
			m_dataSize = sizeof(*data);
		}

		template<typename T>
		byte * SerializeData(uint32_t &size) const
		{
			int32_t sizeOfNetHeader = sizeof(NetworkHeader);

			NetworkHeader header;
			header.Size = 13 + sizeOfNetHeader + (m_data ? sizeof(T) : 0);

			byte *bytes = new byte[header.Size]();
			memcpy(bytes, &header, sizeOfNetHeader);

			byte *sender = ByteConverter::ToBytes<uint32_t>(m_senderID); // 4
			byte *destination = ByteConverter::ToBytes<uint32_t>(m_destinationID); // 4
			byte *tag = ByteConverter::ToBytes<uint32_t>(m_tag); // 4

			memcpy(bytes + sizeOfNetHeader, sender, 4);
			bytes[sizeOfNetHeader + 4] = (byte)m_distributionMode;
			memcpy(bytes + sizeOfNetHeader + 5, destination, 4);
			memcpy(bytes + sizeOfNetHeader + 9, tag, 4);

			if (m_data)
				memcpy(bytes + 13 + sizeOfNetHeader, m_data, sizeof(T));

			size = header.Size;
			return bytes;
		}

		byte *SerializeData(uint32_t &size) const;
		void Deserialize(byte *data, uint32_t size);
		void DeserializeWithoutHeader(byte* data, uint32_t size);

		template<typename T>
		T *GetData() const
		{
			return (T*)m_data;
		}
	};
}