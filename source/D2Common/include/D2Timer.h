#pragma once

#pragma pack(1)

struct D2TimerArgStrc
{
	void* pUnk;
	uint32_t dwUnitId;
};

struct D2TimerQueueStrc
{
	uint32_t dwQueueNo;
	D2TimerStrc* pActiveTimerList[5][256];
	D2TimerStrc* pExpiredTimerList[5][256];
	D2TimerStrc* pTimerListByType[5];
	void* pTimerList;
};

struct D2TimerStrc
{
	uint16_t wTimerType;						//0x00
	uint16_t wTimerId;							//0x02
	uint8_t nQueueNo;							//0x04
	uint8_t unk0x05[3];						//0x05
	D2UnitStrc* pUnit;						//0x08
	uint32_t dwUnitId;							//0x0C
	uint32_t dwUnitType;						//0x10
	uint32_t unk0x14;							//0x14
	D2TimerStrc* pPrevTimer;				//0x18
	D2TimerStrc* pNextTimer;				//0x1C
	D2TimerStrc* pPrevTimerOnUnit;			//0x20
	D2TimerStrc* pNextTimerOnUnit;			//0x24
	void* pTimerList;						//0x28
	void* fpTimerFunction;					//0x2C
};

#pragma pack()
