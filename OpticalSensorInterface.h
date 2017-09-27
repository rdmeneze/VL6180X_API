/**
 *  @file       OpticalInteface.h
 *  @brief      describe the interface and factory to optical sensors
 *  @author     Rafael Dias Menezes <rd185189@ncr.com>
 *  @date       05/2017
 */

#ifndef __OPTICALSENSORINTERFACE_H__
#define __OPTICALSENSORINTERFACE_H__ 
    
#include "defs.h"

    
/**
 *  @brief  sensor ID
            pay attention because this parameter is used by worker functions
 */    
typedef enum 
{
    SENS_0, 
    SENS_1, 
} TProxAlsID;

/**
 * @brief define the type of manufactor
 */
typedef enum
{
    STMICRO,
    SIMULATION,
} TManufac;
    
/**
 * @brief define the type of convertion that the VL6180X is performing now
 */
typedef enum 
{
    ST_ALS, 
    ST_RANGE, 
    ST_NONE,
} TCurrentConvertion;

    
//!  Interface type definition
typedef struct STOpticalSensorInt STOpticalSensorInt;

/**
 *  @struct STOpticalSensorInt
 *  @brief  a generic interface to optical sensor
 */
struct STOpticalSensorInt
{
    void*       dev;                                                                        /**! virtual device pointer     */
    uint32_t    (*init)(STOpticalSensorInt* _this);                                         /**! init function interface    */
    uint32_t    (*deinit)(STOpticalSensorInt* _this);                                       /**! init function interface    */
    
    uint32_t    (*getSignalStrenght)(STOpticalSensorInt* _this, void* pRet, size_t szLen);  /**! get signal strenght function interface */
    uint32_t    (*getDistance)(STOpticalSensorInt*  _this, void* pRet, size_t szLen);       /**! get distance function interface */
    uint32_t    (*getAmbientLight)(STOpticalSensorInt*  _this, void* pRet, size_t szLen);   /**! get ambient light function interface */

    uint32_t    (*gettype)(STOpticalSensorInt* _this);
    
    uint32_t    (*getIsrCount)(STOpticalSensorInt* _this);                                   /**!< get the ISR occurrence counter        */

};

/**
 *  @brief  get a instance of the optical sensor struct defined by uiSensID
 *  @param[in] uiSensID sensor ID
 *  @returns NULL  in case of errors
 *  @returns valid pointer in case of success
 */    
STOpticalSensorInt* getOpticalSensorInstance( uint32_t uiSensID );
    
#endif 
