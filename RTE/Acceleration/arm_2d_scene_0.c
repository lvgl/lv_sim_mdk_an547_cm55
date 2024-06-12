/*
 * Copyright (c) 2009-2022 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*============================ INCLUDES ======================================*/

#include "arm_2d.h"

#ifdef RTE_Acceleration_Arm_2D_Scene0

#define __USER_SCENE0_IMPLEMENT__
#include "arm_2d_scene_0.h"

#include "arm_2d_helper.h"
#include "arm_2d_example_controls.h"

#include <stdlib.h>
#include <string.h>

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wmissing-declarations"  
#elif __IS_COMPILER_ARM_COMPILER_5__
#elif __IS_COMPILER_IAR__
#   pragma diag_suppress=Pa089,Pe188,Pe177,Pe174
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#   pragma GCC diagnostic ignored "-Wunused-function"
#   pragma GCC diagnostic ignored "-Wunused-variable"
#   pragma GCC diagnostic ignored "-Wunused-value"
#endif

/*============================ MACROS ========================================*/

#if __GLCD_CFG_COLOUR_DEPTH__ == 8

#   define c_tileBuff               c_tileBuffGRAY8
#   define c_tileTestImageCuisine   c_tileTestImageCuisineGRAY8
#   define c_tileCMSISLogo          c_tileCMSISLogoGRAY8
#   define c_tileBackground         c_tileBackgroundGRAY8

#elif __GLCD_CFG_COLOUR_DEPTH__ == 16

#   define c_tileBuff               c_tileBuffRGB565
#   define c_tileTestImageCuisine   c_tileTestImageCuisineRGB565
#   define c_tileCMSISLogo          c_tileCMSISLogoRGB565
#   define c_tileBackground         c_tileBackgroundRGB565

#elif __GLCD_CFG_COLOUR_DEPTH__ == 32

#   define c_tileBuff               c_tileBuffCCCA8888
#   define c_tileTestImageCuisine   c_tileTestImageCuisineCCCN888
#   define c_tileCMSISLogo          c_tileCMSISLogoCCCA8888
#   define c_tileBackground         c_tileBackgroundCCCA8888

#else
#   error Unsupported colour depth!
#endif

#define DEMO_IMAGE                    c_tileTestImageCuisine //c_tileBuff

#if __DISP0_CFG_PFB_BLOCK_WIDTH__ == __DISP0_CFG_SCEEN_WIDTH__                  \
 && __DISP0_CFG_PFB_BLOCK_HEIGHT__ == __DISP0_CFG_SCEEN_HEIGHT__

#   define __DISP_ADPATER_USE_FULL_FRAMEBUFFER__   1
#else
#   define __DISP_ADPATER_USE_FULL_FRAMEBUFFER__   0
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
#undef this
#define this (*ptThis)

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/

extern const arm_2d_tile_t c_tileTestImageCuisine;

extern const arm_2d_tile_t c_tileBuff;

extern const arm_2d_tile_t c_tileCMSISLogo;
extern const arm_2d_tile_t c_tileCMSISLogoMask;
extern const arm_2d_tile_t c_tileCMSISLogoA2Mask;
extern const arm_2d_tile_t c_tileCMSISLogoA4Mask;
extern const arm_2d_tile_t c_tileCMSISLogoMask2;

extern const arm_2d_tile_t c_tileBackground;
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/


static void __on_scene0_depose(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
    ptScene->ptPlayer = NULL;
    
    /* reset timestamp */
    arm_foreach(int64_t,this.lTimestamp, ptItem) {
        *ptItem = 0;
    }

    arm_foreach(arm_2d_filter_iir_blur_descriptor_t,
                this.tUserOPCODETemplate, 
                ptOP) {
        ARM_2D_OP_DEPOSE(*ptOP);
    }

    if (!this.bUserAllocated) {
        __arm_2d_free_scratch_memory(ARM_2D_MEM_TYPE_UNSPECIFIED, ptScene);
    }
}

/*----------------------------------------------------------------------------*
 * Scene 0                                                                    *
 *----------------------------------------------------------------------------*/

static void __on_scene0_background_start(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene0_background_complete(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}


static void __on_scene0_frame_start(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    do {
        int32_t nResult;
        arm_2d_helper_time_cos_slider(128, 255-16, 3000, 0, &nResult, &this.lTimestamp[1]);
        this.chBlurDegree = nResult;
    } while(0);

#if 0
    this.chBlurDegree--;
    if (this.chBlurDegree < 16) {
        this.chBlurDegree = 128;
    }
#endif
}

static void __on_scene0_frame_complete(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
#if 0
    /* switch to next scene after 3s */
    if (arm_2d_helper_is_time_out(3000, &this.lTimestamp[0])) {
        arm_2d_scene_player_switch_to_next_scene(ptScene->ptPlayer);
    }
#endif

    arm_foreach(arm_2d_filter_iir_blur_descriptor_t,
                this.tUserOPCODETemplate, 
                ptOP) {
        arm_2dp_filter_iir_blur_depose(ptOP);
    }
}

static void __before_scene0_switching_out(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static
IMPL_PFB_ON_DRAW(__pfb_draw_scene0_handler)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)pTarget;
    ARM_2D_UNUSED(ptTile);
    ARM_2D_UNUSED(bIsNewFrame);
    
    arm_2d_canvas(ptTile, __top_canvas) {
    /*-----------------------draw the foreground begin-----------------------*/
        
        /* following code is just a demo, you can remove them */
        
        arm_2d_fill_colour(ptTile, &__top_canvas, GLCD_COLOR_BLACK);

        /* on picture plus label */
        arm_2d_size_t tCell = DEMO_IMAGE.tRegion.tSize;
        tCell.iHeight += 10;


        /* start layout */
        arm_2d_layout(__top_canvas) {

#if 0
            /* cell 1, the original Lenna */
            __item_horizontal(tCell, 2,2,2,2) {
                
                arm_2d_region_t __inner_canvas = __item_region;

                arm_2d_layout(__inner_canvas) {

                    __item_line_dock_vertical(10) {
                        arm_2d_fill_colour(ptTile, &__item_region, GLCD_COLOR_DARK_GREY);


                        arm_lcd_text_set_colour(GLCD_COLOR_GREEN, GLCD_COLOR_WHITE);
                        arm_lcd_print_banner("Original", __item_region);
                    }

                    __item_line_dock_vertical(DEMO_IMAGE.tRegion.tSize.iHeight) {

                        arm_2d_tile_copy_only(  &DEMO_IMAGE,
                                                ptTile,
                                                &__item_region);

                    }
                }
            }

            /* cell 2, algorithm 1 */
            __item_horizontal(tCell, 2,2,2,2) {
                
                arm_2d_region_t __inner_canvas = __item_region;

                arm_2d_layout(__inner_canvas) {

                    __item_line_dock_vertical(10) {
                        arm_2d_fill_colour(ptTile, &__item_region, GLCD_COLOR_DARK_GREY);


                        arm_lcd_text_set_colour(GLCD_COLOR_GREEN, GLCD_COLOR_WHITE);
                        arm_lcd_print_banner("Blur Degree: 128", __item_region);
                    }

                    __item_line_dock_vertical(DEMO_IMAGE.tRegion.tSize.iHeight) {

                        arm_2d_tile_copy_only(  &DEMO_IMAGE,
                                                ptTile,
                                                &__item_region);

                        if (bIsNewFrame && !__DISP_ADPATER_USE_FULL_FRAMEBUFFER__) {
                            if (NULL == arm_2d_scratch_memory_new(  
                                        &this.tUserOPCODETemplate[0].tScratchMemory,
                                        sizeof(__arm_2d_iir_blur_acc_t),
                                        (   __item_region.tSize.iHeight 
                                        +   __item_region.tSize.iWidth),
                                        __alignof__(__arm_2d_iir_blur_acc_t),
                                        ARM_2D_MEM_TYPE_FAST)) {
                                assert(false);  /* insufficient memory */
                            }
                        }

                        arm_2dp_filter_iir_blur(&this.tUserOPCODETemplate[0],
                                                ptTile,
                                                &__item_region,
                                                128);

                    }
                }
            }


            /* cell 3, algorithm 2 */
            __item_horizontal(tCell, 2,2,2,2) {
                
                arm_2d_region_t __inner_canvas = __item_region;

                arm_2d_layout(__inner_canvas) {

                    __item_line_dock_vertical(10) {
                        arm_2d_fill_colour(ptTile, &__item_region, GLCD_COLOR_DARK_GREY);


                        arm_lcd_text_set_colour(GLCD_COLOR_GREEN, GLCD_COLOR_WHITE);
                        arm_lcd_print_banner("Blur Degree: 239", __item_region);
                    }

                    __item_line_dock_vertical(DEMO_IMAGE.tRegion.tSize.iHeight) {

                        arm_2d_tile_copy_only(  &DEMO_IMAGE,
                                                ptTile,
                                                &__item_region);

                    if (bIsNewFrame && !__DISP_ADPATER_USE_FULL_FRAMEBUFFER__) {
                            if (NULL == arm_2d_scratch_memory_new(  
                                        &this.tUserOPCODETemplate[1].tScratchMemory,
                                        sizeof(__arm_2d_iir_blur_acc_t),
                                        (   __item_region.tSize.iHeight 
                                        +   __item_region.tSize.iWidth),
                                        __alignof__(__arm_2d_iir_blur_acc_t),
                                        ARM_2D_MEM_TYPE_FAST)) {
                                assert(false);  /* insufficient memory */
                            }
                        }

                        arm_2dp_filter_iir_blur(&this.tUserOPCODETemplate[1],
                                                ptTile,
                                                &__item_region,
                                                255-16);

                    }
                }
            }

#endif
            /* cell 3, algorithm 3 */
            __item_horizontal(tCell, 2,2,2,2) {
                this.chBlurDegree = 255-16;
                arm_2d_region_t __inner_canvas = __item_region;

                arm_2d_layout(__inner_canvas) {

                    __item_line_dock_vertical(10) {
                        arm_2d_fill_colour(ptTile, &__item_region, GLCD_COLOR_DARK_GREY);


                        arm_lcd_text_set_colour(GLCD_COLOR_GREEN, GLCD_COLOR_WHITE);
                        //arm_lcd_print_banner("Blur Degree %d", __item_region);
                        do {
                            arm_lcd_text_set_draw_region(&__item_region);
                            arm_lcd_printf("Blur Degree: %d", (int)this.chBlurDegree);
                            
                        } while(0);
                    }

                    __item_line_dock_vertical(DEMO_IMAGE.tRegion.tSize.iHeight) {

                        arm_2d_tile_copy_only(  &DEMO_IMAGE,
                                                ptTile,
                                                &__item_region);

                        arm_2dp_filter_iir_blur(&this.tUserOPCODETemplate[2],
                                                ptTile,
                                                &__item_region,
                                                this.chBlurDegree);
                    }
                }
            }

        }

    /*-----------------------draw the foreground end  -----------------------*/
    }
    arm_2d_op_wait_async(NULL);

    return arm_fsm_rt_cpl;
}

ARM_NONNULL(1)
user_scene_0_t *__arm_2d_scene0_init(   arm_2d_scene_player_t *ptDispAdapter, 
                                        user_scene_0_t *ptThis)
{
    bool bUserAllocated = false;
    assert(NULL != ptDispAdapter);

#if 0
    /*! define dirty regions */
    IMPL_ARM_2D_REGION_LIST(s_tDirtyRegions, static)

        /* a dirty region to be specified at runtime*/
        ADD_REGION_TO_LIST(s_tDirtyRegions,
            0  /* initialize at runtime later */
        ),
        
        /* add the last region:
         * it is the top left corner for text display 
         */
        ADD_LAST_REGION_TO_LIST(s_tDirtyRegions,
            .tLocation = {
                .iX = 0,
                .iY = 0,
            },
            .tSize = {
                .iWidth = __GLCD_CFG_SCEEN_WIDTH__,
                .iHeight = 8,
            },
        ),

    END_IMPL_ARM_2D_REGION_LIST(s_tDirtyRegions)
    
    s_tDirtyRegions[dimof(s_tDirtyRegions)-1].ptNext = NULL;

    /* get the screen region */
    arm_2d_region_t tScreen
        = arm_2d_helper_pfb_get_display_area(
            &ptDispAdapter->use_as__arm_2d_helper_pfb_t);
    
    /* initialise dirty region 0 at runtime
     * this demo shows that we create a region in the centre of a screen(320*240)
     * for a image stored in the tile c_tileCMSISLogoMask
     */
    arm_2d_align_centre(tScreen, c_tileCMSISLogoMask.tRegion.tSize) {
        s_tDirtyRegions[0].tRegion = __centre_region;
    }
#endif

    if (NULL == ptThis) {
        ptThis = (user_scene_0_t *)
                    __arm_2d_allocate_scratch_memory(   sizeof(user_scene_0_t),
                                                        __alignof__(user_scene_0_t),
                                                        ARM_2D_MEM_TYPE_UNSPECIFIED);
        assert(NULL != ptThis);
        if (NULL == ptThis) {
            return NULL;
        }
    } else {
        bUserAllocated = true;
    }
    memset(ptThis, 0, sizeof(user_scene_0_t));

    *ptThis = (user_scene_0_t){
        .use_as__arm_2d_scene_t = {
            /* Please uncommon the callbacks if you need them
             */
            .fnScene        = &__pfb_draw_scene0_handler,
            //.ptDirtyRegion  = (arm_2d_region_list_item_t *)s_tDirtyRegions,
            

            //.fnOnBGStart    = &__on_scene0_background_start,
            //.fnOnBGComplete = &__on_scene0_background_complete,
            .fnOnFrameStart = &__on_scene0_frame_start,
            //.fnBeforeSwitchOut = &__before_scene0_switching_out,
            .fnOnFrameCPL   = &__on_scene0_frame_complete,
            .fnDepose       = &__on_scene0_depose,
        },
        .bUserAllocated = bUserAllocated,
    };
    
    arm_foreach(arm_2d_filter_iir_blur_descriptor_t,
                this.tUserOPCODETemplate, 
                ptOP) {
        ARM_2D_OP_INIT(*ptOP);
    }

    this.chBlurDegree = 128;

    arm_2d_scene_player_append_scenes(  ptDispAdapter, 
                                        &this.use_as__arm_2d_scene_t, 
                                        1);

    return ptThis;
}




#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#endif

