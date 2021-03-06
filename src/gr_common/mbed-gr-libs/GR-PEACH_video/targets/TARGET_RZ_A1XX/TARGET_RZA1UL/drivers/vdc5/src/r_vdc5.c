/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         r_vdc5.c
* @version      0.06
* $Rev: 18 $
* $Date:: 2014-05-29 11:11:38 +0900#$
* @brief        RZ/A1L VDC5 driver API function
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    "r_vdc5.h"
#include    "r_vdc5_user.h"
#include    "r_vdc5_register.h"
#include    "r_vdc5_shared_param.h"
#include    "r_vdc5_check_parameter.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static vdc5_graphics_type_t ConvertRwProcId2GrId(const vdc5_layer_id_t layer_id);


/**************************************************************************//**
 * @brief       VDC5 driver initialization
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Initializes the VDC5 driver's internal variables.
 *              - Calls the user-defined function specified in init_func.
 *              - Sets up and enables the VDC5's panel clock.
 *              - Disables all the VDC5 interrupts.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   param                   : Initialization parameter
 * @param[in]   init_func               : Pointer to a user-defined function
 * @param[in]   user_num                : User defined number
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_Initialize (
    const vdc5_channel_t        ch,
    const vdc5_init_t   * const param,
    void               (* const init_func)(uint32_t),
    const uint32_t              user_num)
{
    vdc5_error_t            ret;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_InitializeCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmInit();

        /* Callback function */
        if (init_func != 0)
        {
            init_func(user_num);
        }

        VDC5_Initialize(param);

        /* Disable all VDC5 interrupts */
        VDC5_Int_Disable();

        VDC5_ShrdPrmSetResource(VDC5_RESOURCE_PANEL_CLK, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_Initialize() */

/**************************************************************************//**
 * @brief       VDC5 driver termination
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Disables all the VDC5 interrupts.
 *              - Disables the VDC5 panel clock.
 *              - Calls the user-defined function specified in quit_func.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   quit_func               : Pointer to a user-defined function
 * @param[in]   user_num                : User defined number
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_Terminate (const vdc5_channel_t ch, void (* const quit_func)(uint32_t), const uint32_t user_num)
{
    vdc5_error_t    ret;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_TerminateCheckPrm(ch);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Disable all VDC5 interrupts */
        VDC5_Int_Disable();

        VDC5_Terminate();

        VDC5_ShrdPrmSetResource(VDC5_RESOURCE_PANEL_CLK, VDC5_RESOURCE_ST_INVALID);

        /* Callback function */
        if (quit_func != 0)
        {
            quit_func(user_num);
        }
    }
    return ret;
}   /* End of function R_VDC5_Terminate() */

/**************************************************************************//**
 * @brief       Video input setup
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Sets up the phase timing of the input signals.
 *              - Performs delay control on the sync signal for the video inputs.
 *              - Sets up the parameters for the external input video signals.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   param                   : Video input setup parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_VideoInput (const vdc5_channel_t ch, const vdc5_input_t * const param)
{
    vdc5_error_t ret;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_VideoInputCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetInputParam(param);

        /* Setting VDC5 registers */
        VDC5_VideoInput(param);

        VDC5_ShrdPrmSetResource(VDC5_RESOURCE_VIDEO_IN, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_VideoInput() */

/**************************************************************************//**
 * @brief       Synchronization control setup
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Selects the vertical sync signal.
 *              - Sets up the period of the sync signal.
 *              - Sets up the delay of the vertical sync signal.
 *              - Sets up the full-screen enable signal.
 *              - Sets up the compensation for the vertical sync signal.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   param                   : Synchronization control parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_SyncControl (const vdc5_channel_t ch, const vdc5_sync_ctrl_t * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_SyncControlCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(VDC5_RESOURCE_PANEL_CLK);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_CLK;
        }
    }
    if (ret == VDC5_OK)
    {
        if (param->res_vs_sel == VDC5_OFF)
        {   /* External input Vsync signal is selected. */
            rsrc_state = VDC5_ShrdPrmGetResource(VDC5_RESOURCE_VIDEO_IN);
            if (rsrc_state != VDC5_RESOURCE_ST_VALID)
            {
                ret = VDC5_ERR_RESOURCE_INPUT;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_SyncControl(param);

        VDC5_ShrdPrmSetResource(VDC5_RESOURCE_VSYNC, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_SyncControl() */

/**************************************************************************//**
 * @brief       Display output setup
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Sets up the timing signals for driving the LCD panel.
 *              - Sets up the phase, data sequence, and format of the LCD panel output data.
 *              - Sets up the background color.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   param                   : Display output configuration parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_DisplayOutput (const vdc5_channel_t ch, const vdc5_output_t * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_DisplayOutputCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(VDC5_RESOURCE_PANEL_CLK);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_CLK;
        }
        else
        {
            rsrc_state = VDC5_ShrdPrmGetResource(VDC5_RESOURCE_VSYNC);
            if (rsrc_state != VDC5_RESOURCE_ST_VALID)
            {
                ret = VDC5_ERR_RESOURCE_VSYNC;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetOutputParam(param);

        /* Setting VDC5 registers */
        VDC5_DisplayOutput(param);

        VDC5_ShrdPrmSetResource(VDC5_RESOURCE_LCD_PANEL, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_DisplayOutput() */

/**************************************************************************//**
 * @brief       Interrupt callback setup
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Enables the interrupt when the pointer to the corresponding interrupt callback function is specified.
 *              - Registers the specified interrupt callback function.
 *              - Disables the interrupt when the pointer to the corresponding interrupt callback function is not
 *                specified.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   param                   : Interrupt callback setup parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_CallbackISR (const vdc5_channel_t ch, const vdc5_int_t * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_CallbackISRCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(VDC5_RESOURCE_PANEL_CLK);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_CLK;
        }
        else
        {
            rsrc_state = VDC5_ShrdPrmGetResource(VDC5_RESOURCE_VSYNC);
            if (rsrc_state != VDC5_RESOURCE_ST_VALID)
            {
                ret = VDC5_ERR_RESOURCE_VSYNC;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_Int_SetInterrupt(param);
    }
    return ret;
}   /* End of function R_VDC5_CallbackISR() */

/**************************************************************************//**
 * @brief       Data write control processing
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Sets up the input image area to be captured.
 *              - Makes input image scale-down/rotation control settings.
 *              - Makes frame buffer write control settings.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_0_WR
 * @param[in]   param                   : Data write control parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_WriteDataControl (
    const vdc5_channel_t        ch,
    const vdc5_layer_id_t       layer_id,
    const vdc5_write_t  * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_WriteDataControlCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(layer_id);
        if (rsrc_state != VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is already used. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
        else
        {
            rsrc_state = VDC5_ShrdPrmGetResource(VDC5_RESOURCE_VIDEO_IN);
            if (rsrc_state != VDC5_RESOURCE_ST_VALID)
            {
                ret = VDC5_ERR_RESOURCE_INPUT;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetWriteParam(param);

        /* Setting VDC5 registers */
        VDC5_WriteDataControl(param);

        VDC5_ShrdPrmSetLayerResource(layer_id, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_WriteDataControl() */

/**************************************************************************//**
 * @brief       Data write change processing
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Changes the input image area to be captured.
 *              - Makes changes with respect to scaling-down/rotation control of the input image.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_0_WR
 * @param[in]   param                   : Data write change parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ChangeWriteProcess (
    const vdc5_channel_t            ch,
    const vdc5_layer_id_t           layer_id,
    const vdc5_write_chg_t   * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ChangeWriteProcessCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetRwProcEnabled(layer_id);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetChgWriteParam(param);

        /* Setting VDC5 registers */
        VDC5_ChangeWriteProcess(param);
    }
    return ret;
}   /* End of function R_VDC5_ChangeWriteProcess() */

/**************************************************************************//**
 * @brief       Data read control processing
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Sets up the display area for graphics images.
 *              - Makes image scale-up control settings (only layer 0).
 *              - Makes frame buffer read control settings.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_0_RD
 *                                        - VDC5_LAYER_ID_2_RD
 *                                        - VDC5_LAYER_ID_3_RD
 * @param[in]   param                   : Data read control parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ReadDataControl (
    const vdc5_channel_t        ch,
    const vdc5_layer_id_t       layer_id,
    const vdc5_read_t   * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ReadDataControlCheckPrm(ch, layer_id, graphics_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(layer_id);
        if (rsrc_state != VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is already used. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetReadParam(graphics_id, param);

        /* Setting VDC5 registers */
        VDC5_ReadDataControl(graphics_id, param);

        VDC5_ShrdPrmSetLayerResource(layer_id, VDC5_RESOURCE_ST_VALID);
    }
    return ret;
}   /* End of function R_VDC5_ReadDataControl() */

/**************************************************************************//**
 * @brief       Data read change processing
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Changes the frame buffer base address.
 *              - Changes the frame buffer read size (image scale-up control, only layer 0).
 *              - Changes the display area for graphics images.
 *              - Changes the graphics display mode.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_0_RD
 *                                        - VDC5_LAYER_ID_2_RD
 *                                        - VDC5_LAYER_ID_3_RD
 * @param[in]   param                   : Data read change parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ChangeReadProcess (
    const vdc5_channel_t            ch,
    const vdc5_layer_id_t           layer_id,
    const vdc5_read_chg_t   * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ChangeReadProcessCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetRwProcEnabled(layer_id);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetChgReadParam(graphics_id, param);

        /* Setting VDC5 registers */
        VDC5_ChangeReadProcess(graphics_id, param);
    }
    return ret;
}   /* End of function R_VDC5_ChangeReadProcess() */

/**************************************************************************//**
 * @brief       Data write/read start processing
 *
 *              Description:<br>
 *              This function performs layer start processing. If the layer ID specified in layer_id
 *              is VDC5_LAYER_ID_ALL, the function starts all the layers that are in the stopped state
 *              and also enabled. If the layer ID is not VDC5_LAYER_ID_ALL, the function starts only
 *              the specified layer.<br>
 *              When performing start processing for write, the function starts a write to the frame buffer.
 *              When performing start processing for read, the function starts a read from the frame buffer
 *              and sets the graphics display mode to the specified values for each layer.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_ALL
 *                                        - VDC5_LAYER_ID_0_WR
 *                                        - VDC5_LAYER_ID_0_RD
 *                                        - VDC5_LAYER_ID_2_RD
 *                                        - VDC5_LAYER_ID_3_RD
 * @param[in]   param                   : Data write/read start parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_StartProcess (
    const vdc5_channel_t        ch,
    const vdc5_layer_id_t       layer_id,
    const vdc5_start_t  * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_StartProcessCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        if (layer_id != VDC5_LAYER_ID_ALL)
        {
            rsrc_state = VDC5_ShrdPrmGetRwProcReady(layer_id);
            if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
            {   /* The specified layer is invalid. */
                ret = VDC5_ERR_RESOURCE_LAYER;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_StartProcess(layer_id, param);
    }
    return ret;
}   /* End of function R_VDC5_StartProcess() */

/**************************************************************************//**
 * @brief       Data write/read stop processing
 *
 *              Description:<br>
 *              This function performs layer stop processing. If the layer ID specified in layer_id is
 *              VDC5_LAYER_ID_ALL, the function stops all the layers that are enabled and running.
 *              If the layer ID is not VDC5_LAYER_ID_ALL, the function stops only the specified layer.<br>
 *              When performing stop processing for write, the function stops the write to the frame buffer.
 *              When performing stop processing for read, the function stops the read from the frame buffer
 *              and resets the graphics display mode to the initial values for each of the layers.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_ALL
 *                                        - VDC5_LAYER_ID_0_WR
 *                                        - VDC5_LAYER_ID_0_RD
 *                                        - VDC5_LAYER_ID_2_RD
 *                                        - VDC5_LAYER_ID_3_RD
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_StopProcess (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_StopProcessCheckPrm(ch, layer_id);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        if (layer_id != VDC5_LAYER_ID_ALL)
        {
            rsrc_state = VDC5_ShrdPrmGetRwProcEnabled(layer_id);
            if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
            {   /* The specified layer is invalid. */
                ret = VDC5_ERR_RESOURCE_LAYER;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_StopProcess(layer_id);
    }
    return ret;
}   /* End of function R_VDC5_StopSurface() */

/**************************************************************************//**
 * @brief       Data write/read control release processing
 *
 *              Description:<br>
 *              If the layer ID specified in layer_id is VDC5_LAYER_ID_ALL, the function disables all the layers
 *              that are not running and also enabled. If the layer ID is not VDC5_LAYER_ID_ALL, the function
 *              disables only the specified layers.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_ALL
 *                                        - VDC5_LAYER_ID_0_WR
 *                                        - VDC5_LAYER_ID_0_RD
 *                                        - VDC5_LAYER_ID_2_RD
 *                                        - VDC5_LAYER_ID_3_RD
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ReleaseDataControl (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ReleaseDataControlCheckPrm(ch, layer_id);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        if (layer_id != VDC5_LAYER_ID_ALL)
        {
            rsrc_state = VDC5_ShrdPrmGetRwProcReady(layer_id);
            if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
            {   /* The specified layer is invalid. */
                ret = VDC5_ERR_RESOURCE_LAYER;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        VDC5_ShrdPrmSetLayerResource(layer_id, VDC5_RESOURCE_ST_INVALID);
    }
    return ret;
}   /* End of function R_VDC5_ReleaseDataControl() */

/**************************************************************************//**
 * @brief       Noise reduction setup
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Turns on and off noise reduction processing.
 *              - Sets up the noise reduction parameters for the Y/G, Cb/B, and Cr/R signals.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   nr1d_on                 : Noise reduction ON/OFF setting
 * @param[in]   param                   : Noise reduction setup parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_VideoNoiseReduction (
    const vdc5_channel_t                    ch,
    const vdc5_onoff_t                      nr1d_on,
    const vdc5_noise_reduction_t    * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_VideoNoiseReductCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(VDC5_RESOURCE_VIDEO_IN);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_INPUT;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_VideoNoiseReduction(nr1d_on, param);
    }
    return ret;
}   /* End of function R_VDC5_VideoNoiseReduction() */

/**************************************************************************//**
 * @brief       Color matrix setup
 *
 *              Description:<br>
 *              This function sets up the specified color matrix.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   param                   : Color matrix setup parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ImageColorMatrix (const vdc5_channel_t ch, const vdc5_color_matrix_t * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ImageColorMatrixCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        if (param->module == VDC5_COLORMTX_IMGCNT)
        {
            rsrc_state = VDC5_ShrdPrmGetLayerResource(VDC5_LAYER_ID_0_WR);
        }
        else    /* VDC5_COLORMTX_ADJ_0 */
        {
            rsrc_state = VDC5_ShrdPrmGetLayerResource(VDC5_LAYER_ID_0_RD);
        }

        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_ImageColorMatrix(param);
    }
    return ret;
}   /* End of function R_VDC5_ImageColorMatrix() */

/**************************************************************************//**
 * @brief       Image enhancement processing
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Turns on and off sharpness processing.
 *              - Sets up the sharpness parameter.
 *              - Turns on and off LTI processing.
 *              - Sets up the LTI parameter.
 *              - Sets up the enhancer-enabled area to be subjected to sharpness and LTI processing.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   imgimprv_id             : Image quality improver ID
 *                                        - VDC5_IMG_IMPRV_0
 * @param[in]   shp_h_on                : Sharpness ON/OFF setting
 * @param[in]   sharp_param             : Sharpness setup parameter
 * @param[in]   lti_h_on                : LTI ON/OFF setting
 * @param[in]   lti_param               : LTI setup parameter
 * @param[in]   enh_area                : Enhancer-enabled area setup parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ImageEnhancement (
    const vdc5_channel_t                ch,
    const vdc5_imgimprv_id_t            imgimprv_id,
    const vdc5_onoff_t                  shp_h_on,
    const vdc5_enhance_sharp_t  * const sharp_param,
    const vdc5_onoff_t                  lti_h_on,
    const vdc5_enhance_lti_t    * const lti_param,
    const vdc5_period_rect_t    * const enh_area)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;
    vdc5_color_space_t      color_space;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ImageEnhancementCheckPrm(ch, imgimprv_id, sharp_param, enh_area);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(VDC5_LAYER_ID_0_RD);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
        else
        {
            /* Condition checks */
            color_space = VDC5_ShrdPrmGetColorSpaceFbRd(VDC5_GR_TYPE_GR0);
            if (color_space == VDC5_COLOR_SPACE_GBR)
            {   /* The image quality improver does not act on RGB signals. */
                ret = VDC5_ERR_IF_CONDITION;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_ImageEnhancement(shp_h_on, sharp_param, lti_h_on, lti_param, enh_area);
    }
    return ret;
}   /* End of function R_VDC5_ImageEnhancement() */

/**************************************************************************//**
 * @brief       Black stretch setup
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Turns on and off black stretch processing.
 *              - Sets up the black stretch parameters.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   imgimprv_id             : Image quality improver ID
 *                                        - VDC5_IMG_IMPRV_0
 * @param[in]   bkstr_on                : Black stretch ON/OFF setting
 * @param[in]   param                   : Black stretch setup parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_ImageBlackStretch (
    const vdc5_channel_t        ch,
    const vdc5_imgimprv_id_t    imgimprv_id,
    const vdc5_onoff_t          bkstr_on,
    const vdc5_black_t  * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;
    vdc5_color_space_t      color_space;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ImageBlackStretchCheckPrm(ch, imgimprv_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(VDC5_LAYER_ID_0_RD);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
        else
        {
            /* Condition checks */
            color_space = VDC5_ShrdPrmGetColorSpaceFbRd(VDC5_GR_TYPE_GR0);
            if (color_space == VDC5_COLOR_SPACE_GBR)
            {   /* The image quality improver does not act on RGB signals. */
                ret = VDC5_ERR_IF_CONDITION;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_ImageBlackStretch(bkstr_on, param);
    }
    return ret;
}   /* End of function R_VDC5_ImageBlackStretch() */

/**************************************************************************//**
 * @brief       Alpha blending setup
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Sets up the alpha value of the ARGB1555/RGBA5551 formats.
 *              - Make settings for premultiplication processing at alpha blending in one-pixel.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_2_RD
 *                                        - VDC5_LAYER_ID_3_RD
 * @param[in]   param                   : Alpha blending setup parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_AlphaBlending (
    const vdc5_channel_t                ch,
    const vdc5_layer_id_t               layer_id,
    const vdc5_alpha_blending_t * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_AlphaBlendingCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(layer_id);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_AlphaBlending(graphics_id, param);
    }
    return ret;
}   /* End of function R_VDC5_AlphaBlending() */

/**************************************************************************//**
 * @brief       Rectangle alpha blending setup
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Turns on and off alpha blending in a rectangular area.
 *              - Sets up the rectangular area subjected to alpha blending.
 *              - Sets up the alpha value for alpha blending in a rectangular area.
 *              - Makes fade-in/-out settings to be applied to rectangle alpha blending.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_2_RD
 *                                        - VDC5_LAYER_ID_3_RD
 * @param[in]   gr_arc_on               : ON/OFF setting for alpha blending in a rectangular area
 * @param[in]   param                   : Setup parameter for alpha blending in a rectangular area
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_AlphaBlendingRect (
    const vdc5_channel_t                        ch,
    const vdc5_layer_id_t                       layer_id,
    const vdc5_onoff_t                          gr_arc_on,
    const vdc5_alpha_blending_rect_t    * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_AlphaBlendingRectCheckPrm(ch, layer_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(layer_id);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_AlphaBlendingRect(graphics_id, gr_arc_on, param);
    }
    return ret;
}   /* End of function R_VDC5_AlphaBlendingRect() */

/**************************************************************************//**
 * @brief       Chroma-key setup
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Turns on and off the chroma-key processing.
 *              - Sets up the color signals to be subject to chroma-key processing and
 *                the color signals after replacement.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_0_RD
 *                                        - VDC5_LAYER_ID_2_RD
 *                                        - VDC5_LAYER_ID_3_RD
 * @param[in]   gr_ck_on                : Chroma-key ON/OFF setting
 * @param[in]   param                   : Chroma-key setup parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_Chromakey (
    const vdc5_channel_t            ch,
    const vdc5_layer_id_t           layer_id,
    const vdc5_onoff_t              gr_ck_on,
    const vdc5_chromakey_t  * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_color_space_t      color_space;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_ChromakeyCheckPrm(ch, layer_id, graphics_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(layer_id);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
        else
        {
            /* Condition checks */
            color_space = VDC5_ShrdPrmGetColorSpaceFbRd(graphics_id);
            if (color_space == VDC5_COLOR_SPACE_YCBCR)
            {   /* Chroma-key processing cannot be used
                   when the format of the signal read from the frame buffer is YCbCr422 or YCbCr444. */
                ret = VDC5_ERR_IF_CONDITION;
            }
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_Chromakey(graphics_id, gr_ck_on, param);
    }
    return ret;
}   /* End of function R_VDC5_Chromakey() */

/**************************************************************************//**
 * @brief       CLUT setup
 *
 *              Description:<br>
 *              This function sets up CLUT for the specified layer.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   layer_id                : Layer ID
 *                                        - VDC5_LAYER_ID_0_RD
 *                                        - VDC5_LAYER_ID_2_RD
 *                                        - VDC5_LAYER_ID_3_RD
 * @param[in]   param                   : CLUT setup parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_CLUT (const vdc5_channel_t ch, const vdc5_layer_id_t layer_id, const vdc5_clut_t * const param)
{
    vdc5_error_t            ret;
    vdc5_graphics_type_t    graphics_id;
    vdc5_resource_state_t   rsrc_state;

    graphics_id = ConvertRwProcId2GrId(layer_id);

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_CLUTCheckPrm(ch, layer_id, graphics_id, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetLayerResource(layer_id);
        if (rsrc_state == VDC5_RESOURCE_ST_INVALID)
        {   /* The specified layer is invalid. */
            ret = VDC5_ERR_RESOURCE_LAYER;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_CLUT(graphics_id, param);
    }
    return ret;
}   /* End of function R_VDC5_SetCLUT() */

/**************************************************************************//**
 * @brief       Display calibration processing
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Sets up panel brightness adjustment.
 *              - Sets up contrast adjustment.
 *              - Sets up panel dithering.
 *              - Makes control settings for the correction circuit sequence.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   param                   : Display calibration parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_DisplayCalibration (const vdc5_channel_t ch, const vdc5_disp_calibration_t * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_DisplayCalibrationCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(VDC5_RESOURCE_LCD_PANEL);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_OUTPUT;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_DisplayCalibration(param);
    }
    return ret;
}   /* End of function R_VDC5_DisplayCalibration() */

/**************************************************************************//**
 * @brief       Gamma correction setup
 *
 *              Description:<br>
 *              This function performs the following processing:
 *              - Turns on and off gamma correction processing.
 *              - Sets up the gamma correction gain adjustment values for the G/B/R signals.
 *              - Sets up the gamma correction start threshold values for the G/B/R signals.
 * @param[in]   ch                      : Channel
 *                                        - VDC5_CHANNEL_0
 * @param[in]   gam_on                  : Gamma correction ON/OFF setting
 * @param[in]   param                   : Gamma correction setup parameter
 * @retval      Error code
 *****************************************************************************/
vdc5_error_t R_VDC5_GammaCorrection (
    const vdc5_channel_t                    ch,
    const vdc5_onoff_t                      gam_on,
    const vdc5_gamma_correction_t   * const param)
{
    vdc5_error_t            ret;
    vdc5_resource_state_t   rsrc_state;

    ret = VDC5_OK;
#ifdef  R_VDC5_CHECK_PARAMETERS
    ret = VDC5_GammaCorrectionCheckPrm(ch, param);
#endif  /* R_VDC5_CHECK_PARAMETERS */
    if (ret == VDC5_OK)
    {
        /* Resource checks */
        rsrc_state = VDC5_ShrdPrmGetResource(VDC5_RESOURCE_LCD_PANEL);
        if (rsrc_state != VDC5_RESOURCE_ST_VALID)
        {
            ret = VDC5_ERR_RESOURCE_OUTPUT;
        }
    }
    if (ret == VDC5_OK)
    {
        /* Setting VDC5 registers */
        VDC5_GammaCorrection(gam_on, param);
    }
    return ret;
}   /* End of function R_VDC5_GammaCorrection() */

/******************************************************************************
Local Functions
******************************************************************************/
/**************************************************************************//**
 * @brief       Convert read/write process ID into graphics type ID
 *
 * @param[in]   layer_id              : Layer ID
 * @retval      Graphics type ID
 *****************************************************************************/
static vdc5_graphics_type_t ConvertRwProcId2GrId (const vdc5_layer_id_t layer_id)
{
    vdc5_graphics_type_t graphics_id;

    switch (layer_id)
    {
        case VDC5_LAYER_ID_0_RD:                /* Layer 0, read process */
            graphics_id = VDC5_GR_TYPE_GR0;     /* Graphics 0 */
        break;
        case VDC5_LAYER_ID_2_RD:                /* Layer 2, read process */
            graphics_id = VDC5_GR_TYPE_GR2;     /* Graphics 2 */
        break;
        case VDC5_LAYER_ID_3_RD:                /* Layer 3, read process */
            graphics_id = VDC5_GR_TYPE_GR3;     /* Graphics 3 */
        break;
        default:                                /* Error */
            graphics_id = VDC5_GR_TYPE_NUM;
        break;
    }
    return graphics_id;
}   /* End of function ConvertRwProcId2GrId() */

