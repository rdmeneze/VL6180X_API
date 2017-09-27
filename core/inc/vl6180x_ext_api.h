/**
 *	@file 	vl6180x_ext_api.h
 *	@brief	declarations of extensions functios for VL6180x STMicroelectronics library
 *	@author	Rafael Dias at NCR <rd185189@ncr.com> <rdmeneze@gmail.com>
 *	@date 	july-2017
 */


#ifndef _VL6180X_EXT_API_H_
#define _VL6180X_EXT_API_H_
    
#include "vl6180x_api.h"
    
#ifdef __cplusplus
extern "C" {
#endif
 
/**
 * @brief Start interleaved mode
 *
 * @details End user should ensure device is in idle state and not already running
 */
int VL6180x_AlsStartInterleavedMode(VL6180xDev_t dev);

/**
 * @brief Stop interleaved mode
 *
 * @details End user should ensure device is in idle state and not already running
 */
int VL6180x_AlsStopInterleavedMode(VL6180xDev_t dev);

/**
 * @brief Set interleaved mode
 * @param[in]   dev device to be controlled
 * @param[in]   mode operation mode. Can be one of these values: INTERLEAVED_ENABLE, INTERLEAVED_DISABLE 
 *
 * @details End user should ensure device is in idle state and not already running
 */
int VL6180x_AlsSetInterleavedMode(VL6180xDev_t dev, uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif