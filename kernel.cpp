//
// kernel.cpp
//
#include "kernel.h"
#include <circle/logger.h>
#include <circle/cputhrottle.h>

LOGMODULE("Kernel");

CKernel::CKernel (void)
:	m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
	m_Timer (&m_Interrupt),
	m_Logger (m_Options.GetLogLevel (), &m_Timer)
	// TODO: add more member initializers here
{
}

CKernel::~CKernel (void)
{
}

boolean CKernel::Initialize (void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Screen.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Serial.Initialize (115200);
	}

	if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice (m_Options.GetLogDevice (), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}
        //pTarget = &m_Serial;

		bOK = m_Logger.Initialize (pTarget);
	}

	if (bOK)
	{
		bOK = m_Interrupt.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Timer.Initialize ();
	}

	// TODO: call Initialize () of added members here (if required)

	return bOK;
}

TShutdownMode CKernel::Run (void)
{
    CCPUThrottle cpuThrottle = CCPUThrottle(CPUSpeedMaximum);

    unsigned start = CTimer::Get()->GetClockTicks();
    LOGDBG("1M ReadAll() start");
    volatile u32 bits;
    for (u32 i = 0; i < 1000000; i++){
        bits = CGPIOPin::ReadAll();
    }
    unsigned now = CTimer::Get()->GetClockTicks();
    unsigned elapsed = now - start;
    LOGDBG("1M ReadAll() finished.");
    LOGDBG("  Elapsed time: %d ms", elapsed);
    LOGDBG("  %d I/O per sec.", 1000000 / (elapsed / 1000));
    LOGDBG("");

    start = CTimer::Get()->GetClockTicks();
    LOGDBG("1M WriteAll() start");
    for (u32 i = 0; i < 500000; i++){
        CGPIOPin::WriteAll(0xffffffff, 0xffffffff);
        CGPIOPin::WriteAll(0, 0xffffffff);
    }
    now = CTimer::Get()->GetClockTicks();
    elapsed = now - start;
    LOGDBG("1M WriteAll() finished.");
    LOGDBG("  Elapsed time: %d ms", elapsed);
    LOGDBG("  %d I/O per sec.", 1000000 / (elapsed / 1000));
    LOGDBG("");


    CGPIOPin pin = CGPIOPin(0, GPIOModeInput);

    start = CTimer::Get()->GetClockTicks();
    LOGDBG("1M SetMode() start");
    for (u32 i = 0; i < 500000; i++){
        pin.SetMode(GPIOModeOutput);
        pin.SetMode(GPIOModeInputPullDown);
    }
    now = CTimer::Get()->GetClockTicks();
    elapsed = now - start;
    LOGDBG("1M SetMode() finished.");
    LOGDBG("  Elapsed time: %d ms", elapsed);
    LOGDBG("  %d I/O per sec.", 1000000 / (elapsed / 1000));
    LOGDBG("");

    return ShutdownHalt;
}
