/**
 *	@file 	vl6180x_ext_api.h
 *	@brief	implementations of extensions functions for VL6180x STMicroelectronics library
 *	@author	Rafael Dias at NCR <rd185189@ncr.com> <rdmeneze@gmail.com>
 *	@date 	july-2017
 */
 

#include "vl6180x_ext_api.h"
#include "vl6180x_ext_def.h"
#include "defs.h"

/************************************************************************************************************************/
 
int VL6180x_AlsStartInterleavedMode(VL6180xDev_t dev)
{
    int status;

	LOG_FUNCTION_START("");
	
	status = VL6180x_WrByte(dev, INTERLEAVED_MODE_ENABLE, INTERLEAVED_ENABLE );
	if( status )
	{
		VL6180x_ErrLog("INTERLEAVED_MODE_ENABLE wr fail");
        goto VL6180x_end;
	}

    status = VL6180x_RangeStartContinuousMode(dev);
	if ( status ) 
	{
		VL6180x_ErrLog("VL6180x_RangeStartContinuousMode wr fail");
	}
	
VL6180x_end:    
    
    LOG_FUNCTION_END(status);
	
    return status;
}


/************************************************************************************************************************/

int VL6180x_AlsSetInterleavedMode(VL6180xDev_t dev, uint8_t mode)
{
    int status; 
    
    uint8_t user_mode;
    
    user_mode = (mode & INTERLEAVED_ENABLE) ? INTERLEAVED_ENABLE : INTERLEAVED_DISABLE;
    
    LOG_FUNCTION_START("");
	status = VL6180x_WrByte(dev, INTERLEAVED_MODE_ENABLE, user_mode );
	if( status )
	{
		VL6180x_ErrLog("INTERLEAVED_MODE_ENABLE wr fail");
	}
    
    return status;
}

/************************************************************************************************************************/


