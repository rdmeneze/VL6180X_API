#ifndef _DEV_VL6180X_H_
#define _DEV_VL6180X_H_

#include "defs.h"
#include "OpticalSensorInterface.h" 
    
#define VL6180x_DEF_I2C_ADDR (0x29u)
    
/**
 *  @brief initialize the vl6180x device
 *  @param[in]  _this pointer to device configuration
 *  @returns    0 on success
 *  @returns    negative value: error
 */    
uint32_t    vl6180x_dev_init(STOpticalSensorInt* _this);

/**
 *  @brief destroy the vl6180x device
 *  @param[in]  _this pointer to device configuration
 *  @returns    0 on success
 *  @returns    negative value: error
 */    
uint32_t    vl6180x_dev_deinit(STOpticalSensorInt* _this);                                       

/**
 *  @brief get signal strenght
 *  @param[in]  _this pointer to device configuration
 *  @param[out]  pRet pointer to the return structure
 *  @param[in]  szLen pRet len
 *  @returns    0 on success
 *  @returns    negative value: error
 */    
uint32_t    vl6180x_dev_getSignalStrenght(STOpticalSensorInt* _this, void* pRet, size_t szLen);  

/**
 *  @brief get distance
 *  @param[in]  _this pointer to device configuration
 *  @param[out]  pRet pointer to the return structure
 *  @param[in]  szLen pRet len
 *  @returns    0 on success
 *  @returns    negative value: error
 */
uint32_t    vl6180x_dev_getDistance(STOpticalSensorInt*  _this, void* pRet, size_t szLen);

/**
 *  @brief get ambient light
 *  @param[in]  _this pointer to device configuration
 *  @param[out]  pRet pointer to the return structure
 *  @param[in]  szLen pRet len
 *  @returns    0 on success
 *  @returns    negative value: error
 */
uint32_t    vl6180x_dev_getAmbientLight(STOpticalSensorInt*  _this, void* pRet, size_t szLen);

/**
 *  @brief get the sensor type
 *  @param[in]  _this pointer to device configuration
 *  @returns    >= 0  on success
 *  @returns    negative value: error
 */
uint32_t vl6180x_dev_gettype(STOpticalSensorInt*  _this);


/**
 *  @brief set the EN (GPIO0) pin of sensor 0
 */
void vl6180_dev_en_set0( void );

/**
 *  @brief reset the EN (GPIO0) pin of sensor 0
 */
void vl6180_dev_en_reset0( void );

/**
 *  @brief set the EN (GPIO0) pin of sensor 1
 */
void dev_vl6180_en_set1( void );

/**
 *  @brief reset the EN (GPIO0) pin of sensor 1
 */
void dev_vl6180_en_reset1( void );

/**
 * @brief initialize the interrupt service routine associated with GPIO1 pin from sensor 0
 */
void vl6180_dev_isr_init0( void );

/**
 * @brief initialize the interrupt service routine associated with GPIO1 pin from sensor 1
 */
void dev_vl6180_isr_init1( void );

/**
 * @brief sleep routine associated with sensor 0
 */
void vl6180_dev_sleep0(void);

/**
 * @brief wakeup routine associated with sensor 0
 */
void vl6180_dev_wakeup0(void);

/**
 * @brief sleep routine associated with sensor 1
 */
void vl6180_dev_sleep1(void);

/**
 * @brief wakeup routine associated with sensor 1
 */
void vl6180_dev_wakeup1(void);


uint32_t vl6180_dev_isr_count0( STOpticalSensorInt* _this );

uint32_t vl6180_dev_isr_count1( STOpticalSensorInt* _this );

#endif //~_DEV_VL6180X_H_
