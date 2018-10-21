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
		uint8_t deviceID;	// Identifies the current machine
		uint16_t structID;	// Must be the same on both sides
	};

	enum StandardStructIDs : uint16_t
	{
		invalidID = 0,
		structureRequest,
		structureResponse,
		scanRequest,
		scanResponse,
		endOfReserve
	};

	/*
	enum E_Endianness
	{
		eNotChecked,
		eLittle,
		eBig
	};
	*/
	/*
	static void memcpyEndian(const void* src, void* dst, int size, E_Endianness srcEndian, E_Endianness dstEndian)
	{
		const char* charSrc = (char*)src;
		char* charDst = (char*)dst;

		if (srcEndian == dstEndian)
		{
			for (int i = 0; i < size; i++)
			{
				charDst[i] = charSrc[i];
			}
		}
		else
		{
			for (int i = 0; i < size; i++)
			{
				charDst[size - i - 1] = charSrc[i];
			}
		}
	}
	*/

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
			dataParams = nullptr;
		};

		~LynxStructure()
		{
			if (data)
			{
				delete data;
				data = nullptr;
			}
		}

		void init(const StructDefinition initParams[], LynxID _lynxID, int nElements = 0);

		int toBuffer(char *dataBuffer);			// Returns size of copied data if success, -1 if failure, -2 if checksum is wrong

		int fromBuffer(const char *dataBuffer); // Returns size of copied data if success, -1 if failure

		void clear();							// sets all elements to 0

		template <class T>
		T getData(int target)
		{	
			int offset = getOffset(target);
			if (offset < 0)
			{
				return 0;
			}

			return *(T*)(data + offset);

			/*
			int index = getOffset(target);

			T temp = 0;
			T temp2 = 0;

			if (index >= 0)
			{
				int dataDiff = sizeof(T) - sizeof(char);

				for (int i = 0; i <= dataDiff; i++)
				{
					temp2 = ((T)data[index + i] << (i * 8)) & (0xFF << (i*8));
					temp = temp | temp2;
				}
			}
			
			return temp;
			*/
		};

		template <class T>
		void setData(int target, T dataIn)
		{
			int offset = getOffset(target);
			if (offset < 0)
			{
				return;
			}

			T* temp = (T*)(data + offset);

			*temp = dataIn;

			/*
			int index = getOffset(target);

			if (index >= 0)
			{
				int dataDiff = sizeof(T) - sizeof(char);

				T temp = dataIn;

				for (int i = 0; i <= dataDiff; i++)
				{
					data[index + i] = (char)(temp & 0xFF);
					temp >>= 8;
				}
			}
			*/
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
		//template <class T>
		//T& getReference(int target)
		//{
		//	// T& temp = (T&)0;

		//	int index = getOffset(target);
		//	if (index > -1)
		//	{
		//		T& temp = (T&)*(data + index);
		//		return temp;
		//	}

		//	// return temp;
		//} does not work

	private:
		int getOffset(int target);

		int indexingSize;
		int size;

		char *data;

		const StructDefinition *dataParams;

		int checkSize(LynxDataType dataType);

		int checkTransferSize(LynxDataType dataType);

		/*
		template <class T>
		static void toArray(const T srcValue, void* dstArray, E_Endianness srcEndian, E_Endianness dstEndian, int size = 0) // Use size to force copy sumber of chars, 8 bit char is then assumed
		{
			int copySize = 0;
			if (size == 0)
			{
				if (srcEndian == dstEndian)
				{
					*(T*)dstArray = srcValue;
					return;
				}
				else
				{
					copySize = sizeof(T) - sizeof(char) + 1;
				}
			}
			else if (size > 0)
			{
				copySize = size;
			}
			else
			{
				return;
			}

			memcpyEndian(&srcValue, dstArray, copySize, srcEndian, dstEndian);

		}
		*/
	};

	/*
	class StaticProperties
	{
	public:
		static E_Endianness localEndian;
		static E_Endianness init();
	private:
		StaticProperties() {};
		~StaticProperties() {};
	};

	E_Endianness StaticProperties::localEndian { eNotChecked };

	E_Endianness StaticProperties::init()
	{
		union
		{
			uint32_t testVar;
			char testArray[sizeof(testVar)];
		}test;

		test.testVar = 1;

		if (test.testArray[0] == 1)
		{
			StaticProperties::localEndian = eLittle;
			return eLittle;
		}
		else
		{
			StaticProperties::localEndian = eBig;
			return eBig;
		}

	}
	*/

	// static const LynxStructure::InitParam InitStructureRequest{ 0, eNoStorage };

 }
