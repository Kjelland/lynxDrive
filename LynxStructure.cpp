#include"LynxStructure.h"

namespace LynxStructureSpace
{
	int LynxStructure::toBuffer(char *dataBuffer)
	{
		int tempTransferSize;
		int index = 0;

		//  Write ID
		dataBuffer[index] = char(this->lynxID.deviceID);
		index++;
		dataBuffer[index] = char(this->lynxID.structID & 0xFF);
		index++;
		dataBuffer[index] = char((this->lynxID.structID >> 8) & 0xFF);
		index++;

		// Write data
		for (int i = 0; i < this->size; i++)
		{
			tempTransferSize = checkTransferSize(this->dataParams[i].dataType);
			switch (tempTransferSize)
			{
			case 1:
				dataBuffer[index] = (char)getData<uint8_t>(i);
				index++;
				break;
			case 2:
			{
				uint16_t temp = getData<uint16_t>(i);
				for (int n = 0; n < 2; n++)
				{
					dataBuffer[index] = (char)((temp >> (n * 8)) & 0xFF);
					index++;
				}
			}
			break;
			case 4:
			{
				uint32_t temp = getData<uint32_t>(i);
				for (int n = 0; n < 4; n++)
				{
					dataBuffer[index] = (char)((temp >> (n * 8)) & 0xFF);
					index++;
				}
			}
			break;
			case 8:
			{
				uint64_t temp = getData<uint64_t>(i);
				for (int n = 0; n < 8; n++)
				{
					dataBuffer[index] = (char)((temp >> (n * 8)) & 0xFF);
					index++;
				}
			}
			break;
			default:
				return -1;
			}
		}

		// Check data
		char checksum = 0;
		for (int i = 0; i < index; i++)
		{
			checksum += data[i];
		}
		dataBuffer[index] = checksum;

		return (index + 1);
	}

	int LynxStructure::fromBuffer(const char *dataBuffer)
	{
		int tempTransferSize;
		int index = 0;

		//  Read ID
		LynxID tempID;
		tempID.deviceID = uint8_t(dataBuffer[index]);
		index++;
		tempID.structID = uint16_t(dataBuffer[index] & 0xFF);
		index++;
		tempID.structID = tempID.structID | uint16_t((dataBuffer[index] << 8) & 0xFF00);
		index++;
		if (tempID.structID != this->lynxID.structID)
		{
			return -1;
		}

		// Read data
		for (int i = 0; i < this->size; i++)
		{
			tempTransferSize = checkTransferSize(this->dataParams[i].dataType);
			switch (tempTransferSize)
			{
			case 1:
				setData<uint8_t>(i, (uint8_t)dataBuffer[index]);
				index++;
				break;
			case 2:
			{
				uint16_t temp = 0;
				for (int n = 0; n < 2; n++)
				{
					temp |= (((uint16_t)dataBuffer[index] << (n * 8)) & (0xFF << (n * 8)));
					index++;
				}
				setData<uint16_t>(i, temp);
			}
			break;
			case 4:
			{
				uint32_t temp = 0; getData<uint32_t>(i);
				for (int n = 0; n < 4; n++)
				{
					temp |= (((uint32_t)dataBuffer[index] << (n * 8)) & (0xFF << (n * 8)));
					index++;
				}
				setData<uint32_t>(i, temp);
			}
			break;
			case 8:
			{
				uint64_t temp = 0; getData<uint64_t>(i);
				for (int n = 0; n < 4; n++)
				{
					temp |= (((uint64_t)dataBuffer[index] << (n * 8)) & (0xFF << (n * 8)));
					index++;
				}
				setData<uint64_t>(i, temp);
			}
			break;
			default:
				return -1;
			}
		}

		// Check data
		char checksum = 0;
		for (int i = 0; i < index; i++)
		{
			checksum += data[i];
		}

		if (checksum == dataBuffer[index])
		{
			return (index + 1);
		}
		else
		{
			return -2;
		}

	}


	void LynxStructure::clear()
	{
		for (int i = 0; i < this->size*this->indexingSize; i++)
		{
			data[i] = 0;
		}
	}


	int LynxStructure::checkSize(LynxDataType dataType)
	{
		int temp = 0;
		switch (dataType)
		{
		case eInt8:
			temp = sizeof(int8_t);
			break;
		case eUint8:
			temp = sizeof(uint8_t);
			break;
		case eInt16:
			temp = sizeof(int16_t);
			break;
		case eUint16:
			temp = sizeof(uint16_t);
			break;
		case eInt32:
			temp = sizeof(int32_t);
			break;
		case eUint32:
			temp = sizeof(uint32_t);
			break;
		case eInt64:
			temp = sizeof(int64_t);
			break;
		case eUint64:
			temp = sizeof(uint64_t);
			break;
        case eFloat:
            temp = sizeof(float);
            break;
        case eDouble:
            temp = sizeof(double);
            break;
		default:
			temp = -1;
			break;
		}

		return temp;
	}

	int LynxStructure::checkTransferSize(LynxDataType dataType)
	{
		int temp = 0;
		switch (dataType)
		{
		case eInt8:
			temp = 1;
			break;
		case eUint8:
			temp = 1;
			break;
		case eInt16:
			temp = 2;
			break;
		case eUint16:
			temp = 2;
			break;
		case eInt32:
			temp = 4;
			break;
		case eUint32:
			temp = 4;
			break;
		case eInt64:
			temp = 8;
			break;
		case eUint64:
			temp = 8;
			break;

		default:
			temp = -1;
			break;
		}

		return temp;
	}

	void LynxStructure::init(const StructDefinition* structDefinition, LynxID _lynxID, int nElements)
	{
		this->lynxID = _lynxID;

		if (nElements == 0)
		{
			this->size = 255;
		}
		else
		{
			this->size = nElements;
		}

		int tempSize;
		for (int i = 0; i < this->size; i++)
		{
			if (structDefinition[i].dataType == eEndOfList)
			{
				this->size = i;
				break;
			}

			tempSize = checkSize(structDefinition[i].dataType);

			if (tempSize > this->indexingSize)
			{
				this->indexingSize = tempSize;
			}

		}

		if (data)
		{
			delete data;
		}

		data = new char[this->size*this->indexingSize];
		dataParams = structDefinition;

		this->clear();

	}

	int LynxStructure::getOffset(int target)
	{
		if (this->size == 0)
		{
			return -1;
		}

		return (target * this->indexingSize);



		//for (int i = 0; i < this->size; i++)
		//{
		//	if (this->dataParams[i].identifier == target)
		//	{
		//		index = i * this->indexingSize;
		//		break;
		//	}
		//}

		// return index;
	}

 }
