/**
 *  @file       dev_VL6180x.c
 *  @brief      VL6180x device interface
 *  @author     Rafael Dias Menezes <rd185189@ncr.com>
 *  @date       05/2015
 *  @reference  
 *              [1] <a href="DM00112632">http://www.st.com/content/ccc/resource/technical/document/datasheet/c4/11/28/86/e6/26/44/b3/DM00112632.pdf/files/DM00112632.pdf/jcr:content/translations/en.DM00112632.pdf</a>
 */

#include "dev_VL6180x.h"
#include "errno.h"
#include <CyLib.h>
#include "vl6180x_platform.h"
#include "vl6180x_api.h"
#include "vl6180x_ext_api.h"
#include "vl6180x_inc.h"
#include "isr_G10.h"
#include "isr_G11.h"

/****************************************************************************/
/* module defines                                                           */
#define VL6180x_CS_HIGH 1
#define VL6180x_CS_LOW  0

#define VL6180X_DEFAULT_PRIORITY 3L

#define VL6180x_DEVICE_IDENTIFICATION   (0xB4)

#define ALS_INTER_MEASUREMENT_TIME      (100L)
#define ALS_INTEGRATION_TIME            (50L)
#define RANGE_MAX_CONV_TIME             (30L)
#define RANGE_INTER_MEASUREMENT_TIME    (100L)

/****************************************************************************/
/* local defines                                                            */
static      BOOL bInitI2c           = FALSE;
volatile    BOOL bIsrOccurred[2]    = {FALSE};
volatile    uint32_t bIsrCounter[2] = {0};

/****************************************************************************/

void vl6180_dev_en_set0( void )
{
    vl6180_G00_Write( VL6180x_CS_HIGH );
}

/****************************************************************************/

void vl6180_dev_en_reset0( void )
{
    vl6180_G00_Write( VL6180x_CS_LOW );
}

/****************************************************************************/

void vl6180_dev_en_set1( void )
{
    vl6180_G10_Write( VL6180x_CS_HIGH );
}

/****************************************************************************/

void vl6180_dev_en_reset1( void )
{
    vl6180_G10_Write( VL6180x_CS_LOW );
}

/****************************************************************************/

static void isr_gpio10( void )
{
    bIsrOccurred[0] = TRUE;
    bIsrCounter[0]++;
    
    isr_G10_ClearPending();
    
    vl6180_G10_ClearInterrupt();    
}

/****************************************************************************/

static void isr_gpio11( void )
{
    bIsrOccurred[1] = TRUE;
    bIsrCounter[1]++;
    
    isr_G11_ClearPending();
    
    vl6180_G11_ClearInterrupt();        
}

/****************************************************************************/

void vl6180_dev_isr_init0( void )
{
    isr_G10_StartEx( isr_gpio10 );
    
    isr_G10_SetPriority( VL6180X_DEFAULT_PRIORITY );
}

/****************************************************************************/

void vl6180_dev_isr_init1( void )
{
    isr_G11_StartEx( isr_gpio11 );
    
    isr_G11_SetPriority( VL6180X_DEFAULT_PRIORITY );    
}

/****************************************************************************/

uint32_t vl6180_dev_isr_count0( STOpticalSensorInt* _this )
{
    (void)_this;
    return (uint32_t)bIsrCounter[0];
}

/****************************************************************************/

uint32_t vl6180_dev_isr_count1( STOpticalSensorInt* _this )
{
    (void)_this;
    return (uint32_t)bIsrCounter[1];
}

/****************************************************************************/

void vl6180_dev_sleep0(void)
{
    vl6180_dev_en_reset0();

    xI2C_VL6180_sda_SetDriveMode( xI2C_VL6180_sda_DM_DIG_HIZ );
    xI2C_VL6180_scl_SetDriveMode( xI2C_VL6180_scl_DM_DIG_HIZ );
    
    xI2C_VL6180_Sleep();
    
    vl6180x_dev_deinit( NULL );
    
    return;
}

/****************************************************************************/

void vl6180_dev_wakeup0(void)
{
    
    /* to be implemented */
    
    return;
}

/****************************************************************************/

void vl6180_dev_sleep1(void)
{
    vl6180_dev_en_reset1();

    xI2C_VL6180_sda_SetDriveMode( xI2C_VL6180_sda_DM_DIG_HIZ );
    xI2C_VL6180_scl_SetDriveMode( xI2C_VL6180_scl_DM_DIG_HIZ );
    
    xI2C_VL6180_Sleep();
    
    vl6180x_dev_deinit( NULL );
    
    return;
}

/****************************************************************************/

void vl6180_dev_wakeup1(void)
{
    return;
}

/****************************************************************************/

uint32_t vl6180x_dev_init_interleaved( struct MyDev_t* dev )
{
    uint32_t dwRet = 0;
    int iRet;
    
    if ( NULL == dev )
    {
        dwRet = ENOMEM;
        goto VL6180X_init_interleaved_error;
    }
    
    
    /* next commands are related to the design tip DT0017 */
    /*                                                    */
    /*                                                    */   
    dwRet = EIO;
    iRet = VL6180x_AlsSetIntegrationPeriod( dev, ALS_INTEGRATION_TIME );
    
    dwRet = EIO;                    
    iRet = VL6180x_RangeSetMaxConvergenceTime( dev, RANGE_MAX_CONV_TIME );
    if ( iRet ) goto VL6180X_init_interleaved_error;    
    
    dwRet = EIO;                    
    iRet = VL6180x_RangeSetInterMeasPeriod( dev, RANGE_INTER_MEASUREMENT_TIME );
    if ( iRet ) goto VL6180X_init_interleaved_error;    
  
    dwRet = EIO;                    
    iRet = VL6180x_AlsSetInterMeasurementPeriod( dev, ALS_INTER_MEASUREMENT_TIME );
    if ( iRet ) goto VL6180X_init_interleaved_error;     
    
    dwRet = EIO;                    
    iRet = VL6180x_AlsSetInterleavedMode( dev, INTERLEAVED_ENABLE );
    if ( iRet ) goto VL6180X_init_interleaved_error;  
    
//    dwRet = EIO;                    
//    iRet = VL6180x_AlsSetInterMeasurementPeriod( dev, ALS_INTER_MEASUREMENT_TIME );
//    if ( iRet ) goto VL6180X_init_interleaved_error;     
    
    dwRet = EIO;
    iRet = VL6180x_AlsSetSystemMode( dev, MODE_CONTINUOUS | MODE_START_STOP );
    if ( iRet ) goto VL6180X_init_interleaved_error;
    
    dwRet = 0;
    
    goto VL6180X_init_interleaved_OK;
    
VL6180X_init_interleaved_error:

VL6180X_init_interleaved_OK: 
    return dwRet;
    
}

/****************************************************************************/

uint32_t    vl6180x_dev_init(STOpticalSensorInt* _this)
{
    uint32_t dwRet = EFAULT;
    if ( NULL != _this )
    {
        if ( NULL != _this->dev )
        {
            struct MyDev_t* dev = (struct MyDev_t*)_this->dev;
            
            if ( FALSE == dev->bInit )
            {
                int iRet;
                uint8_t bIdModel;                

                
                /* VL6180x initialization process */
                dev->xCurConv = ST_ALS;
                
                dev->reset_cs( );
                
                CyDelayUs( 500 );  /* wait for 500 us */
                
                dev->set_cs( );
                
                CyDelay( 10 );  /* wait for 10 ms */

                VL6180x_FilterSetState( dev, 0 );
                
                if ( FALSE == bInitI2c )
                {
                    // initialize the PSoC I2C port
                    xI2C_VL6180_Start();
                    bInitI2c = TRUE;
                }
                
                dwRet = ENOMEDIUM;
                VL6180x_RdByte( dev, IDENTIFICATION_MODEL_ID, &bIdModel );
                if ( bIdModel != VL6180x_DEVICE_IDENTIFICATION ) goto VL6180X_init_error;
                
                CyDelayUs( 1000 );
                
                dwRet = EIO;
                iRet = VL6180x_WrByte( dev, SYSTEM_FRESH_OUT_OF_RESET, 1 );
                if (iRet) goto VL6180X_init_error;                
                
                VL6180x_WaitDeviceBooted( dev );                       

                dwRet = EDEADLK;
                iRet = VL6180x_InitData( dev );
                if (iRet) goto VL6180X_init_error;
                
                dwRet = ENOSPC;
                iRet = VL6180x_Prepare( dev );
                if (iRet) goto VL6180X_init_error;
                
                dwRet = EIO;                    
                iRet = VL6180x_AlsConfigInterrupt( dev, CONFIG_GPIO_INTERRUPT_NEW_SAMPLE_READY );
                if (iRet) goto VL6180X_init_error;
                
                dwRet = EIO;
                iRet = VL6180x_RangeConfigInterrupt( dev, CONFIG_GPIO_INTERRUPT_NEW_SAMPLE_READY );
                if (iRet) goto VL6180X_init_error;
                
                dwRet = EIO;                    
                iRet = VL6180x_SetupGPIO1( dev, GPIOx_SELECT_GPIO_INTERRUPT_OUTPUT, INTR_POL_HIGH ); 
                if ( iRet ) goto VL6180X_init_error;        
                
                /* next commands are related to the design tip DT0017 */
                /*                                                    */
                /*                                                    */
                dwRet = EIO;
                iRet = vl6180x_dev_init_interleaved( dev );
                if ( iRet ) goto VL6180X_init_error;     
                
                dwRet = EIO;
                iRet = VL6180x_AlsSetAnalogueGain( dev, 7 );
                if ( iRet ) goto VL6180X_init_error;     
                
                dev->isr_init( );
                
                /*                                                    */
                /*                                                    */
                /******************************************************/
                /* initialization OK */
                dev->bInit = TRUE;
                dwRet = 0;   
                
                goto VL6180X_init_OK;
            }
        }
    }
    
VL6180X_init_error:
    
VL6180X_init_OK:
    return dwRet;
}

/****************************************************************************/

uint32_t    vl6180x_dev_deinit(STOpticalSensorInt* _this)
{
    (void)_this;
    
    bInitI2c = FALSE;
    return 0;
}

/****************************************************************************/

uint32_t    vl6180x_dev_getSignalStrenght(STOpticalSensorInt* _this, void* pRet, size_t szLen)
{
    (void)szLen;
    uint32_t dwRet = EFAULT;
    
    if ( NULL != _this )
    {
        if ( NULL != pRet )
        {
            struct MyDev_t* dev = (struct MyDev_t*)_this->dev;
            /*if ( ST_STRENGHT == dev->xCurConv )*/
            {
                if ( bIsrOccurred[dev->xSensId] )
                {
                }
            }
        }
    }
    
    return dwRet;
}

/****************************************************************************/

uint32_t    vl6180x_dev_getDistance(STOpticalSensorInt*  _this, void* pRet, size_t szLen)
{
    (void)szLen;
    uint32_t dwRet = EFAULT;
    int iRet;
    static VL6180x_RangeData_t xRangeData;
    uint8_t xRangeData_intStatus;
    
    if ( NULL != _this )
    {
        if ( NULL != pRet )
        {
            dwRet = 0;            
            struct MyDev_t* dev = (struct MyDev_t*)_this->dev;
            if ( ST_RANGE == dev->xCurConv )
            {
                //if ( TRUE == bIsrOccurred[dev->xSensId] )
                {
                    (void)VL6180x_RangeGetInterruptStatus( dev, &xRangeData_intStatus );
                    if ( (RESULT_INTERRUPT_STATUS_GPIO & xRangeData_intStatus) == RES_INT_STAT_GPIO_NEW_SAMPLE_READY )
                    {
                        dwRet = EFAULT;
                        iRet = VL6180x_RangeGetMeasurement(dev, &xRangeData);
                        if ( 0 == iRet ) 
                        {                
                            do
                            {
                                VL6180x_ClearAllInterrupt( dev );
                                (void)VL6180x_RangeGetInterruptStatus( dev, &xRangeData_intStatus );
                            }while( (RESULT_INTERRUPT_STATUS_GPIO & xRangeData_intStatus) == RES_INT_STAT_GPIO_NEW_SAMPLE_READY );
                            
                            {
                                if ( sizeof( uint32_t) == szLen )
                                {
                                    if ( NULL != pRet )
                                    {
                                        *(uint32_t*)pRet = xRangeData.FilteredData.rawRange_mm;
                                    }
                                }                                                                                            
                            }                            
                        }
                    }
                    
                    bIsrOccurred[dev->xSensId] = FALSE;
                    dev->xCurConv = ST_ALS;
                    dwRet = 0;                                                                                
                }
            }
            dwRet = 0;
        }
    }
    
    return dwRet;
}

/****************************************************************************/

uint32_t    vl6180x_dev_getAmbientLight(STOpticalSensorInt*  _this, void* pRet, size_t szLen)
{
    (void)szLen;
    int iRet;    
    uint32_t dwRet = EFAULT;
    uint8_t xAlsData_intStatus;
    static VL6180x_AlsData_t xVL6180x_AlsData;
    
    if ( NULL != _this )
    {
        if ( NULL != pRet )
        {
            dwRet = 0;
            struct MyDev_t* dev = (struct MyDev_t*)_this->dev;
            if ( ST_ALS == dev->xCurConv )
            {
                //if ( TRUE == bIsrOccurred[dev->xSensId] ) 
                {                   
                    (void)VL6180x_AlsGetInterruptStatus( dev, &xAlsData_intStatus ); 
                    if ( (RESULT_INTERRUPT_STATUS_GPIO & xAlsData_intStatus) == RES_INT_STAT_GPIO_NEW_SAMPLE_READY )
                    {
                        dwRet = EFAULT;
                        iRet = VL6180x_AlsGetMeasurement( dev, &xVL6180x_AlsData );
                        if ( !iRet )
                        {
                            do
                            {
                                VL6180x_ClearAllInterrupt( dev );
                                (void)VL6180x_AlsGetInterruptStatus( dev, &xAlsData_intStatus );
                            }while( (RESULT_INTERRUPT_STATUS_GPIO & xAlsData_intStatus) == RES_INT_STAT_GPIO_NEW_SAMPLE_READY );
                            
                            {
                                if ( sizeof( uint32_t) == szLen )
                                {
                                    if ( NULL != pRet )
                                    {
                                        *(uint32_t*)pRet = xVL6180x_AlsData.lux;
                                    }
                                }                                                                                            
                            }
                        }  
                    }
                    
                    bIsrOccurred[dev->xSensId] = FALSE;
                    dev->xCurConv = ST_RANGE;
                    dwRet = 0;                    
                }                
            }
        }
    }
    
    return dwRet;
}

/****************************************************************************/

uint32_t vl6180x_dev_gettype(STOpticalSensorInt*  _this)
{
    uint32_t dwRet = -1;
    if ( NULL != _this )
    {
        struct MyDev_t* dev = (struct MyDev_t*)_this->dev;
        if ( NULL != dev )
        {
            dwRet = (uint32_t)dev->xManufac;
        }
    }
    
    return dwRet;
}

/****************************************************************************/


