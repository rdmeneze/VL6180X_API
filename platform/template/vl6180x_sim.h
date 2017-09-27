/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef _VL6180X_SIM_H_
#define _VL6180X_SIM_H_

#include "defs.h"
#include "OpticalSensorInterface.h" 
    
#define VL6180x_SIM_DEF_I2C_ADDR (0xFFu)    
    
/**
 */    
void vl6180_sim_en_write( uint8_t val );
void vl6180_sim_en_set( void );
void vl6180_sim_en_reset( void );
void vl6180_sim_isr_init( void );
void vl6180_sim_sleep(void);
void vl6180_sim_wakeup(void);

/**
 */    
uint8_t vl6180_sim_en_read(void);

/**
 */    
uint8_t vl6180_sim_st_read(void);    

/**
 *  @brief initialize the vl6180x device
 *  @param[in]  _this pointer to device configuration
 *  @returns    0 on success
 *  @returns    negative value: error
 */    
uint32_t    dev_vl6180x_sim_init(STOpticalSensorInt* _this);

/**
 *  @brief destroy the vl6180x device
 *  @param[in]  _this pointer to device configuration
 *  @returns    0 on success
 *  @returns    negative value: error
 */    
uint32_t    dev_vl6180x_sim_deinit(STOpticalSensorInt* _this);                                       

/**
 *  @brief get signal strenght
 *  @param[in]  _this pointer to device configuration
 *  @param[out]  pRet pointer to the return structure
 *  @param[in]  szLen pRet len
 *  @returns    0 on success
 *  @returns    negative value: error
 */    
uint32_t    dev_vl6180x_sim_getSignalStrenght(STOpticalSensorInt* _this, void* pRet, size_t szLen);  

/**
 *  @brief get distance
 *  @param[in]  _this pointer to device configuration
 *  @param[out]  pRet pointer to the return structure
 *  @param[in]  szLen pRet len
 *  @returns    0 on success
 *  @returns    negative value: error
 */
uint32_t    dev_vl6180x_sim_getDistance(STOpticalSensorInt*  _this, void* pRet, size_t szLen);

/**
 *  @brief get ambient light
 *  @param[in]  _this pointer to device configuration
 *  @param[out]  pRet pointer to the return structure
 *  @param[in]  szLen pRet len
 *  @returns    0 on success
 *  @returns    negative value: error
 */
uint32_t    dev_vl6180x_sim_getAmbientLight(STOpticalSensorInt*  _this, void* pRet, size_t szLen);

/**
 *  @brief get the sensor type
 *  @param[in]  _this pointer to device configuration
 *  @returns    >= 0  on success
 *  @returns    negative value: error
 */
uint32_t dev_vl6180x_sim_gettype(STOpticalSensorInt*  _this);

uint32_t vl6180_sim_isr_count( STOpticalSensorInt* _this );

#endif
