/**
 *  @file       OpticalInteface.h
 *  @brief      describe the interface and factory to optical sensors
 *  @author     Rafael Dias Menezes <rd185189@ncr.com>
 *  @date       09/2017
 */

#include "OpticalSensorInterface.h" 
#include "dev_VL6180x.h"
#include "vl6180x_platform.h"
#include "vl6180x_inc.h"
#include "vl6180x_sim.h"
#include "util.h"

/****************************************************************************/
/* static members                                                           */
/* dev_vl6180x:  vector with the description of optical drivers installed on*/ 
/*  HFD hardware                                                            */
/****************************************************************************/
static struct MyDev_t dev_vl6180x[] = 
{
    [0] = 
    {
        .i2c_dev_addr   = VL6180x_DEF_I2C_ADDR,     /*!< default I2C address */
        .xSensId        = SENS_0,                   /*!< sens ID             */
        .xManufac       = STMICRO,                  /*!< manufactor          */
        
        /*! device access functions */
        .write_cs       = vl6180_G00_Write,         /*!< write chip select pin      */
        .read_cs        = vl6180_G00_Read,          /*!< read chip select pin       */
        .read_st        = vl6180_G01_Read,          /*!< read the state of INT pin  */

        .set_cs         = vl6180_dev_en_set0,       /*!< set Chip Select                */
        .reset_cs       = vl6180_dev_en_reset0,     /*!< reset chip select              */
        .isr_init       = vl6180_dev_isr_init0,     /*!< initialize the interrupt GPIO  */
        
        .sleep          = vl6180_dev_sleep0,        /*!< enter in sleep mode            */
        .wakeup         = vl6180_dev_wakeup0,       /*!< wakeup                         */
        
    },
    [1] = 
    {
        .i2c_dev_addr   = VL6180x_DEF_I2C_ADDR,
        .xSensId        = SENS_1, 
        .xManufac       = SIMULATION, 
        
        .write_cs       = vl6180_sim_en_write,
        .read_cs        = vl6180_sim_en_read,
        .read_st        = vl6180_sim_st_read,
        
        .set_cs         = vl6180_sim_en_set,
        .reset_cs       = vl6180_sim_en_reset,
        .isr_init       = vl6180_sim_isr_init,

        .sleep          = vl6180_sim_sleep,
        .wakeup         = vl6180_sim_wakeup,        
        
    },
};


/****************************************************************************/
/*  oDev: class that give access to the optical sensor.                     */
/*      here, we have two objects: one related to the VL6180x and another   */
/*      related to a simulation instance                                    */
/****************************************************************************/
static STOpticalSensorInt oDev[] = 
{
    [0]=
    {
        .dev                = &dev_vl6180x[0],
        .init               = vl6180x_dev_init,
        .gettype            = vl6180x_dev_gettype,
        .deinit             = vl6180x_dev_deinit,
        .getSignalStrenght  = vl6180x_dev_getSignalStrenght,
        .getDistance        = vl6180x_dev_getDistance,
        .getAmbientLight    = vl6180x_dev_getAmbientLight,
        .getIsrCount        = vl6180_dev_isr_count0,
    },
    [1]=
    {
        .dev                = &dev_vl6180x[1],
        .init               = dev_vl6180x_sim_init,
        .gettype            = dev_vl6180x_sim_gettype,
        .deinit             = dev_vl6180x_sim_deinit,
        .getSignalStrenght  = dev_vl6180x_sim_getSignalStrenght,
        .getDistance        = dev_vl6180x_sim_getDistance,
        .getAmbientLight    = dev_vl6180x_sim_getAmbientLight,
        .getIsrCount        = vl6180_sim_isr_count, 
    },
};

/****************************************************************************/

STOpticalSensorInt* getOpticalSensorInstance( uint32_t uiSensID )
{
    STOpticalSensorInt* pSensInt = NULL;
    
    if ( uiSensID < GET_ARRAY_LEN(oDev) )
    {
        pSensInt = &oDev[uiSensID];
    }
    
    return pSensInt;
}

/****************************************************************************/
