/**
 *	@file 	vl6180x_ext_def.h
 *	@brief	definitions for extensions functions VL6180x STMicroelectronics library
 *	@author	Rafael Dias at NCR <rd185189@ncr.com> <rdmeneze@gmail.com>
 *	@date 	july-2017
 */
 
 #ifndef _VL6180X_EXT_DEF_
 #define _VL6180X_EXT_DEF_
 
/**
 *  @def INTERLEAVED_MODE_ENABLE
 *  @brief Writing to this register will enable the interleaved modes
 *
 *  Interleaved mode enable: Write 0x1 to this register to select ALS+Range interleaved mode. 
 *		Use SYSALS_START and SYSALS_INTERMEASUREMENT_PERIOD to control this mode.
 * 		A range measurement is automatically performed immediately after each ALS measurement.
 *  @ingroup device_regdef
 */
 
#define INTERLEAVED_MODE_ENABLE				0x2A3
	#define INTERLEAVED_ENABLE 				0x01
	#define INTERLEAVED_DISABLE 			0x00
    
#endif