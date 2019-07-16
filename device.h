/* =====================================================================*/
/* Program: template.h                                                  */
/*                                                                      */
/* Project: Template-Project                                            */
/*                                                                      */
/* Description: Template Description                                    */
/*                                                                      */
/* Last Edited: 2018-11-06                                              */
/*                                                                      */
/* Author: Falk Kyburz                                                  */
/*                                                                      */
/* ==================================================================== */
/*    (c) 2018 by Interstaatliche Hochschule f�r Technik Buchs NTB      */
/* ==================================================================== */
#ifndef DEVICE_H
#define DEVICE_H


/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */
#include <stdint.h>
#include "ringbuffer.h"

#include "defines.h"
#include "DSP2803x_Cla_typedefs.h"// DSP2803x CLA Type definitions
#include "DSP2803x_Device.h"      // DSP2803x Headerfile Include File

// Functions that will be run from RAM need to be assigned to
// a different section.  This section will then be mapped to a load and
// run address using the linker cmd file.
#pragma CODE_SECTION(InitFlash, "ramfuncs");
#define Device_cal (void   (*)(void))0x3D7C80


/* ==================================================================== */
/* ============================ constants ============================= */
/* ==================================================================== */
/* #define and enum statements go here */
#define CLARAM0_ENABLE  1
#define CLARAM1_ENABLE  1


/* ==================================================================== */
/* ========================== public data ============================= */
/* ==================================================================== */
/* Definition of public (external) data types go here */

/* ==================================================================== */
/* ======================= public functions =========================== */
/* ==================================================================== */
//CLA ISRs
__interrupt void cla1_isr2(void);

//Linker defined vars
extern Uint16 Cla1Prog_Start;
extern Uint16 Cla1funcsLoadStart;
extern Uint16 Cla1funcsLoadEnd;
extern Uint16 Cla1funcsRunStart;
extern Uint16 Cla1funcsLoadSize;
extern Uint16 Cla1mathTablesLoadStart;
extern Uint16 Cla1mathTablesRunStart;
extern Uint16 Cla1mathTablesLoadSize;

//microprocessor public functions
extern int32_t device_init(void);
extern void updateDutyEPwm(uint16_t duty);


/* ==================================================================== */
/* =================== public inline functions ======================== */
/* ==================================================================== */

/**
 * Macro to calculate a integral controller
 * @param cntl - I_CONTROLLER structure, ref - Reference Value, value - actual value to compare
 * @return I_CONTROLLER structure
 */
#define run_I_CONTROLLER_CLA_MACRO(cntl, ref, value)            \
        cntl.u = ref - value;                                   \
        cntl.e = cntl.ys - cntl.y;                              \
        cntl.y = cntl.y1 + cntl.Vi * cntl.u + cntl.Vr * cntl.e; \
        cntl.ys = __mmaxf32(cntl.y, cntl.sat_min);              \
        cntl.ys = __mminf32(cntl.ys, cntl.sat_max);             \
        cntl.y1 = cntl.y;

/* Enable gate drivers for Phase U */
extern inline int32_t device_driverEnableVLS(void)
{
    GpioDataRegs.GPADAT.bit.GPIO11 = 0;
    return 0;
}

extern inline int32_t device_driverEnableVHS(void)
{
    GpioDataRegs.GPBDAT.bit.GPIO43 = 0;
    return 0;
}

/* Enable gate drivers for Phase V */
extern inline int32_t device_driverEnableULS(void)
{
    GpioDataRegs.GPADAT.bit.GPIO4 = 0;
    return 0;
}

extern inline int32_t device_driverEnableUHS(void)
{
    GpioDataRegs.GPADAT.bit.GPIO5 = 0;
    return 0;
}

/* Enable gate drivers for Phase U */
extern inline int32_t device_driverDisableVLS(void)
{
    GpioDataRegs.GPADAT.bit.GPIO11 = 1;
    return 0;
}

extern inline int32_t device_driverDisableVHS(void)
{
    GpioDataRegs.GPBDAT.bit.GPIO43 = 1;
    return 0;
}

/* Enable gate drivers for Phase V */
extern inline int32_t device_driverDisableULS(void)
{
    GpioDataRegs.GPADAT.bit.GPIO4 = 1;
    return 0;
}

extern inline int32_t device_driverDisableUHS(void)
{
    GpioDataRegs.GPADAT.bit.GPIO5 = 1;
    return 0;
}

extern inline int32_t device_setALLDriverDisable(void)
{
    device_driverDisableULS();
    device_driverDisableVLS();
    device_driverDisableUHS();
    device_driverDisableVHS();

    return 0;
}

extern inline int32_t device_setALLDriverEnable(void)
{
    device_driverEnableULS();
    device_driverEnableVLS();
    device_driverEnableUHS();
    device_driverEnableVHS();

    return 0;
}


#endif // DEVICE_H


