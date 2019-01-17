#include "../../lvgl/lv_core/lv_vdb.h"
#include "stm32f769i_discovery_lcd.h"
#include "bsp.h"




extern DSI_HandleTypeDef hdsi_discovery;

static uint32_t * my_fb1 = (uint32_t *)LCD_FB_START_ADDRESS;

static uint32_t *my_fb2 = (uint32_t *)((uint32_t)(0xC0000000 + (1024*1024*4)));
static uint32_t *my_gui = (uint32_t *)((uint32_t)(0xC0000000 + (1024*1024*8)));

static uint8_t buff1State = 0 ;


static void CopyBuffer(const uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{
	uint32_t row;
	for(row = y; row < y + ysize; row++ ) {
		memcpy(&pDst[row * 800 + x], pSrc, xsize * 4);
		pSrc += xsize;
	}
}

/**
 * Flush a color buffer
 * @param x1 left coordinate of the rectangle
 * @param x2 right coordinate of the rectangle
 * @param y1 top coordinate of the rectangle
 * @param y2 bottom coordinate of the rectangle
 * @param color_p pointer to an array of colors
 */
void tft_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
  uint32_t *fbAddr ;
    /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > TFT_HOR_RES - 1) return;
    if(y1 > TFT_VER_RES - 1) return;

    CopyBuffer((const uint32_t *)color_p, my_fb1, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
    //fbAddr = my_fb1 + (800*y1+x1);
    //DMA2D_CopyBuffer(0,(void *)color_p,fbAddr, x2 - x1 + 1, y2 - y1 + 1,0,800-(x2-x1+1));
    //DMA2D_CopyBufferWithAlpha(0,(void *)color_p,fbAddr, x2 - x1 + 1, y2 - y1 + 1,0,800-(x2-x1+1));

    HAL_DSI_Refresh(&hdsi_discovery);
    lv_flush_ready();
}

void tft_copy2fb(void)
{
//  if ( buff1State == 0 )
//  {
//    memcpy( my_fb2, my_gui, 800*480*4);
//  }
//  else
//  {
//    memcpy( my_fb1, my_gui, 800*480*4);
//  }
//  buff1State = !buff1State;
}



/**
  * @brief  Line Event callback.
  * @param  hltdc pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{
  /* Prevent unused argument(s) compilation warning */
  //UNUSED(hltdc);
  
//  if ( buff1State == 0 )
//  {
//    HAL_LTDC_SetAddress(hltdc, (uint32_t)my_fb1, 0);
//  }
//  else
//  {
//    HAL_LTDC_SetAddress(hltdc, (uint32_t)my_fb2, 0);
//  }
//  HAL_DSI_Refresh(&hdsi_discovery);
//  __HAL_LTDC_RELOAD_CONFIG(hltdc);
//  
//  HAL_LTDC_ProgramLineEvent(hltdc,0);

}
