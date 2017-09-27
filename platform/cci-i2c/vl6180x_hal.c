/**
 *
 *  @file vl6180x_hal.c
 *  @brief implementation of functions that provide access to I2C interface with vl6180x
 *  @author Rafael Dias Menezes <rdmeneze@gmail.com>
 *  @date 2017/05
 */

#include "vl6180x_inc.h"
#include "vl6180x_i2c.h"

/********************************************************************************/
/* sensor definitions                                                           */
/* this field will be used on VL6180xDev_t->i2c_dev_addr */
/********************************************************************************/

int VL6180x_I2CWrite(VL6180xDev_t dev, uint8_t *buff, uint8_t len)
{
    uint32_t dwRet = EFAULT;
    
    if ( NULL != dev )
    {
        (void)xI2C_VL6180_I2CMasterWriteBuf( dev->i2c_dev_addr, buff, len, xI2C_VL6180_I2C_MODE_COMPLETE_XFER );
        
        while (0u == (xI2C_VL6180_I2CMasterStatus() & xI2C_VL6180_I2C_MSTAT_WR_CMPLT));
        
        if (0u == (xI2C_VL6180_I2C_MSTAT_ERR_XFER & xI2C_VL6180_I2CMasterStatus()))
        {
            dwRet = EACCES;
            if (xI2C_VL6180_I2CMasterGetWriteBufSize() == len)
            {
                dwRet = 0;
            }
        }
    }
    
    return dwRet;
}

/******************************************************************************/

int VL6180x_I2CRead(VL6180xDev_t dev, uint8_t *buff, uint8_t len) 
{
    uint32 dwRet = EFAULT;    
    
    if ( NULL != dev )
    {
        (void)xI2C_VL6180_I2CMasterReadBuf( dev->i2c_dev_addr, buff, len, xI2C_VL6180_I2C_MODE_COMPLETE_XFER );

        /* Waits until master complete read transfer */
        while (0u == (xI2C_VL6180_I2CMasterStatus() & xI2C_VL6180_I2C_MSTAT_RD_CMPLT));
        
        if (0u == (xI2C_VL6180_I2C_MSTAT_ERR_XFER & xI2C_VL6180_I2CMasterStatus()))
        {
            if ( xI2C_VL6180_I2CMasterGetReadBufSize() == len )
            {
                dwRet = 0;
            }
        }
    }    

    return dwRet;
}

/******************************************************************************/

void OnErrLog(void)
{
    return;
}

/******************************************************************************/
