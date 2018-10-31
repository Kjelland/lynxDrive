#include"LynxStructure.h"

// #include <exception>
// #include <iostream>

namespace LynxStructureSpace
{
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

			tempSize = checkLocalSize(structDefinition[i].dataType);

			if (tempSize > this->indexingSize)
			{
				this->indexingSize = tempSize;
			}

		}

		if (data)
		{
			delete[] data;
		}

		data = new char[this->size*this->indexingSize];
		_structDefinition = structDefinition;

		this->clear();

	}

	int LynxStructure::toBuffer(char *dataBuffer)
	{
		int tempTransferSize;
		int index = 0;

		//  Write ID
		dataBuffer[index] = char(this->lynxID.deviceID);
		index++;
		dataBuffer[index] = char(this->lynxID.structTypeID);
		index++;
		dataBuffer[index] = char(this->lynxID.structInstanceID);
		index++;

		// Write data
		for (int i = 0; i < this->size; i++)
		{
			tempTransferSize = checkTransferSize(this->_structDefinition[i].dataType);
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
		int index = 3;

		// Read data
		for (int i = 0; i < this->size; i++)
		{
			tempTransferSize = checkTransferSize(this->_structDefinition[i].dataType);
			switch (tempTransferSize)
			{
			case 1:
				setData<uint8_t>(i, uint8_t(dataBuffer[index]));
				index++;
				break;
			case 2:
			{
				uint16_t temp = 0;
				for (int n = 0; n < 2; n++)
				{
					temp |= ((uint16_t(dataBuffer[index]) << (n * 8)) & (uint16_t(0xFF) << (n * 8)));
					index++;
				}
				setData<uint16_t>(i, temp);
			}
			break;
			case 4:
			{
				uint32_t temp = 0;
				for (int n = 0; n < 4; n++)
				{
					temp |= ((uint32_t(dataBuffer[index]) << (n * 8)) & (uint32_t(0xFF) << (n * 8)));
					index++;
				}
				setData<uint32_t>(i, temp);
			}
			break;
			case 8:
			{
				uint64_t temp = 0;
				for (int n = 0; n < 4; n++)
				{
					temp |= ((uint64_t(dataBuffer[index]) << (n * 8)) & (uint64_t(0xFF) << (n * 8)));
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

	bool LynxStructure::dataChanged()
	{
		bool temp = this->_dataChanged;
		this->_dataChanged = false;
		return temp;
	}

	
	int LynxStructure::checkLocalSize(LynxDataType dataType)
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
		case eIQ:
			temp = sizeof(uint32_t);
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
		case eFloat:
			temp = 4;
			break;
		case eDouble:
			temp = 8;
			break;
		case eIQ:
			temp = 4;
			break;
		default:
			temp = -1;
			break;
		}

		return temp;
	}



	int LynxStructure::getOffset(int target)
	{
		if (this->size == 0)
		{
			return -1;
		}

		return (target * this->indexingSize);

	}

	void LynxHandler::init(uint8_t _deviceID, int nStructs)
	{
		deviceID = _deviceID;

		if (nStructs > 0)
		{
			if (_structures)
			{
				delete[] _structures;
			}
			_structures = new LynxStructure[nStructs];
			_size = 0;
			_reservedSize = nStructs;
		}
	}

	LynxID LynxHandler::addStructure(uint8_t _structType, uint8_t _structInstance, const LynxStructure::StructDefinition* _structDefinition, int nElements)
	{
		_size++;

		if (_size > _reservedSize )
		{
			_reservedSize++;

			LynxStructure* tempStructs = new LynxStructure[_reservedSize];
			
			if (this->_structures)
			{
				for (int i = 0; i < _reservedSize - 1; i++)
				{
					tempStructs[i].init(_structures[i].structDefinition(), _structures[i].lynxID, _structures[i].getSize());
				}
				
				delete[] _structures;
			}

			_structures = tempStructs;
			tempStructs = nullptr;
		}

		LynxID tempID = { this->deviceID, _structType, _structInstance };
		_structures[_size - 1].init(_structDefinition, tempID);
		return tempID;
	

	}

	int LynxHandler::toBuffer(LynxID _lynxID, char * dataBuffer)
	{
		int index = this->indexFromID(_lynxID);

		if (index >= 0)
		{
			return this->_structures[index].toBuffer(dataBuffer);
		}

		return -1;
	}

	int LynxHandler::fromBuffer(const char * dataBuffer)
	{
		LynxID tempID;

		tempID.deviceID = dataBuffer[0];
		tempID.structTypeID = dataBuffer[1];
		tempID.structInstanceID = dataBuffer[2];

		int index = this->indexFromID(tempID);

		if (index >= 0)
		{
			this->_structures[index].lynxID = tempID;

			return this->_structures[index].fromBuffer(dataBuffer);
		}

		return -1;
	}

	bool LynxHandler::dataChanged(LynxID _lynxID)
	{
		int index = indexFromID(_lynxID);

		if (index < 0)
		{
			return false;
		}

		return this->_structures[index].dataChanged();
	}

	int LynxHandler::indexFromID(LynxID _lynxID)
	{
		for (int i = 0; i < this->_size; i++)
		{
			if ((this->_structures[i].lynxID.structTypeID == _lynxID.structTypeID)
				&& (this->_structures[i].lynxID.structInstanceID == _lynxID.structInstanceID))
			{
				return i;
			}
		}

		return -1;
	}

}