#pragma once
#include <stdint.h>

namespace LynxStructureSpace
{
	enum LynxDataType
	{
		eEndOfList = 0,
		eNoStorage,
		eInt8,
		eUint8,
		eInt16,
		eUint16,
		eInt32,
		eUint32,
		eInt64,
		eUint64,
		eFloat,
		eDouble,
		eIQ
	};

	struct LynxID
	{
		uint8_t deviceID;			// Identifies the current machine
		uint8_t structTypeID;		// Identifies the type of struct
		uint8_t structInstanceID;	// Identifies the instance of the struct
	};

	enum StandardStructIDs : uint8_t
	{
		invalidID = 0,
		structureRequest,
		structureResponse,
		scanRequest,
		scanResponse,
		endOfReserve
	};

	class LynxStructure
	{
	public:
		struct StructDefinition
		{
			int identifier;
			LynxDataType dataType;
		};

		LynxID lynxID; // identifies current datagram

		LynxStructure()
		{
			indexingSize = 0;
			size = 0;
			data = nullptr;
			_structDefinition = nullptr;
			_dataChanged = false;
		};

		~LynxStructure()
		{
			if (data)
			{
				delete[] data;
				data = nullptr;
			}
		};

		void init(const StructDefinition structDefinition[], LynxID _lynxID, int nElements = 0);

		int toBuffer(char *dataBuffer);			// Returns size of copied data if success, -1 if failure, -2 if checksum is wrong

		int fromBuffer(const char *dataBuffer); // Returns size of copied data if success, -1 if failure

		void clear();							// sets all elements to 0

		const StructDefinition* structDefinition() { return this->_structDefinition; };

		int getSize() { return this->size; };

		bool dataChanged();

		template <class T>
		T getData(int identifier)
		{	
			int offset = getOffset(identifier);
			if (offset < 0)
			{
				return 0;
			}

			this->_dataChanged = false;
			
			return *(T*)(data + offset);

		};

		template <class T>
		void setData(int identifier, T dataIn)
		{
			int offset = getOffset(identifier);
			if (offset < 0)
			{
				return;
			}

			T* temp = (T*)(data + offset);

			*temp = dataIn;

			this->_dataChanged = true;

		};

		template <class T>
		bool getBit(int identifier, T bitMask)
		{
			return ((getData<T>(identifier) & bitMask) != 0);
		};

		template <class T>
		void setBit(int identifier, T bitMask, bool state)
		{
			T temp = getData<T>(identifier);
			if (state)
			{
				setData(identifier, temp | bitMask);
			}
			else
			{
				setData(identifier, temp & ~bitMask);
			}
		};

	private:
		int getOffset(int identifier);

		int indexingSize;
		int size;

		char *data;

		bool _dataChanged;

		const StructDefinition* _structDefinition;

		int checkLocalSize(LynxDataType dataType);

		int checkTransferSize(LynxDataType dataType);


	};

	class LynxHandler
	{
	public:
		uint8_t deviceID;

		LynxHandler(uint8_t _deviceID, int nStructs = 0)
		{
			_size = 0;
			_reservedSize = 0;
			_structures = nullptr;

			this->init(_deviceID, nStructs);
		};

		~LynxHandler()
		{
			if (_structures)
			{
				delete[] _structures;
				_structures = nullptr;
			}
		};

		void init(uint8_t _deviceID, int nStructs = 0);

		LynxID addStructure(uint8_t _structType, uint8_t _structInstance, const LynxStructure::StructDefinition* _structDefinition, int nElements = 0);

		template <class T>
		T getData(LynxID _lynxID, int _identifier)
		{
			int index = this->indexFromID(_lynxID);

			if (index >= 0)
			{
				return this->_structures[index].getData<T>(_identifier);
			}
	
			return 0;
		};

		template <class T>
		T setData(LynxID _lynxID, int _identifier, T data) // Returns 0 if _lynxID was not found
		{
			int index = this->indexFromID(_lynxID);

			if (index >= 0)
			{
				this->_structures[index].setData<T>(_identifier, data);
				return data;
			}

			return 0;
		};

		int toBuffer(LynxID _lynxID, char* dataBuffer);

		int fromBuffer(const char* dataBuffer);

		bool dataChanged(LynxID _lynxID);

		int size() { return _size; };

	private:
		LynxStructure* _structures;
		int _size;
		int _reservedSize;

		int indexFromID(LynxID _lynxID);
	};


 }