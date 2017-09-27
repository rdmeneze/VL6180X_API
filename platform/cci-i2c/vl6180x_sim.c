#include "vl6180x_sim.h"
#include <stdlib.h>
#include "TTimer.h"
#include "vl6180x_platform.h"
#include <CyLib.h>

/******************************************************************************/
#define VL6180X_SIM_DELAY (100 * TTIMER_1MS_INTERVAL)

/******************************************************************************/
/* static variables */
static uint8_t  bvl6180_sim_en = 0;
static uint8_t  bvl6180_sim_st = 0;
static uint32_t uivl6180_sim_st_timer_handle = 0;
static uint32_t uivl6180_sim_st_signal_strenght = 0;
static uint32_t uivl6180_sim_st_distance = 0;
static uint32_t uivl6180_sim_st_ambient_light = 0;

/******************************************************************************/
/* worker thread  */
static DWORD vl6180_sim_task( void* param );

/******************************************************************************/

void vl6180_sim_en_set( void )
{
    vl6180_sim_en_write(1);
}

/******************************************************************************/

void vl6180_sim_en_reset( void )
{
    vl6180_sim_en_write(0);
}

/******************************************************************************/

void vl6180_sim_isr_init( void )
{
    return;
}

/******************************************************************************/

void vl6180_sim_sleep(void)
{
    return;
}

/******************************************************************************/

void vl6180_sim_wakeup(void)
{
    return;
}

/******************************************************************************/

void vl6180_sim_en_write( uint8_t val )
{
    if ( val != bvl6180_sim_en )
    {
        if ( val )
        {
            TTimerRestart( uivl6180_sim_st_timer_handle );
            srand( TTimerGetTickCount() );
        }
        else
        {
            TTimerStop( uivl6180_sim_st_timer_handle );
        }
        
        bvl6180_sim_en = val;
    }

    return;
}

/******************************************************************************/

uint8_t vl6180_sim_en_read(void)
{
    return bvl6180_sim_en;
}

/******************************************************************************/

uint8_t vl6180_sim_st_read(void)
{
    uint8_t bsim_tmp = bvl6180_sim_st;
    bvl6180_sim_st = 0;
    return bsim_tmp;
}

/******************************************************************************/

uint32_t dev_vl6180x_sim_init(STOpticalSensorInt* _this)
{
    uint32_t dwRet = EFAULT;
    if ( NULL != _this )
    {
        if ( NULL != _this->dev )
        {
            struct MyDev_t* dev = (struct MyDev_t*)_this->dev;
            
            dwRet = TTimerRegisterCallBack( VL6180X_SIM_DELAY, 
                                            TimerPeriodic, 
                                            vl6180_sim_task, 
                                            NULL, 
                                            &uivl6180_sim_st_timer_handle );
            if ( !dwRet )
            {            
                dwRet = ENOMEDIUM;
                int gpio1 = dev->read_st( );
                if ( !gpio1 )
                {
                    dev->write_cs( 0 );
                    
                    CyDelay( 10 );  /* wait for 10 ms */

                    dev->write_cs( 1 );

                    CyDelayUs( 1000 );  /* wait for 1 ms */ 
                    
                    CyDelayUs( 1000 );
                    
                    dwRet = 0;
                    
                }
            }
        }
    }
    return dwRet;
}

/******************************************************************************/

uint32_t dev_vl6180x_sim_deinit(STOpticalSensorInt* _this)
{
    (void)_this;
    
    if ( uivl6180_sim_st_timer_handle )
    {
        TTimerUnregisterCallBack( uivl6180_sim_st_timer_handle );
        uivl6180_sim_st_timer_handle = 0;
    }
    
    return 0;
}

/******************************************************************************/

uint32_t dev_vl6180x_sim_getSignalStrenght(STOpticalSensorInt* _this, void* pRet, size_t szLen)
{
    uint32_t dwRet = EFAULT;
    if ( NULL != _this )
    {
        if ( NULL != _this->dev )
        {
            //struct MyDev_t* dev = (struct MyDev_t*)_this->dev;
            
            dwRet = 0;
            //if ( dev->read_st() )
            {
                dwRet = EINVAL;
                
                if ( sizeof( uivl6180_sim_st_signal_strenght ) ==  szLen )
                {
                    pRet = &uivl6180_sim_st_signal_strenght;
                    dwRet = 0;
                }
            }
        }
    }
    return dwRet;     
}

/******************************************************************************/

uint32_t dev_vl6180x_sim_getDistance(STOpticalSensorInt*  _this, void* pRet, size_t szLen)
{
    uint32_t dwRet = EFAULT;
    if ( NULL != _this )
    {
        if ( NULL != _this->dev )
        {
            //struct MyDev_t* dev = (struct MyDev_t*)_this->dev;
            
            dwRet = 0;
            //if ( dev->read_st() )
            {
                dwRet = EINVAL;

                if ( sizeof( uint32_t ) ==  szLen )
                {
                    *(uint32_t*)pRet = uivl6180_sim_st_distance;
                    dwRet = 0;
                }
            }
        }
    }
    return dwRet;    
}

/******************************************************************************/

uint32_t dev_vl6180x_sim_getAmbientLight(STOpticalSensorInt*  _this, void* pRet, size_t szLen)
{
    uint32_t dwRet = EFAULT;
    if ( NULL != _this )
    {
        if ( NULL != _this->dev )
        {
            //struct MyDev_t* dev = (struct MyDev_t*)_this->dev;
            
            dwRet = 0;
            //if ( dev->read_st() )
            {
                dwRet = EINVAL;
            
                if ( sizeof( uint32_t ) ==  szLen )
                {
                    *(uint32_t*)pRet = uivl6180_sim_st_ambient_light;
                    dwRet = 0;
                }
            }
        }
    }
    return dwRet;    
    
}

/******************************************************************************/

static uint32_t vl6180_sim_scale( uint32_t x, const uint32_t min, const uint32_t max )
{
    uint32_t ret = 0;
    
    ret = (uint32_t)((((float)(x / (float)RAND_MAX)) * (max - min)) + min);
    
    return ret;
    
}

/******************************************************************************/

static DWORD vl6180_sim_task( void* param )
{
    static uint8_t uiSignal2Update = 0;
    uint32_t vrand;
    
    vrand = rand();
    
    (void)param;
    
    if ( uiSignal2Update & 1 )
    {
        uivl6180_sim_st_ambient_light = vl6180_sim_scale( vrand, 0, 350 );
    }
    else
    {
        uivl6180_sim_st_distance = vl6180_sim_scale( vrand, 0, 254 );
    }
    
    uiSignal2Update++;
    
    bvl6180_sim_st = 1;

    return 0;
}

/******************************************************************************/

uint32_t dev_vl6180x_sim_gettype(STOpticalSensorInt*  _this)
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

/******************************************************************************/

uint32_t vl6180_sim_isr_count( STOpticalSensorInt* _this )
{
    (void)_this;
    return 0;
}

/******************************************************************************/


