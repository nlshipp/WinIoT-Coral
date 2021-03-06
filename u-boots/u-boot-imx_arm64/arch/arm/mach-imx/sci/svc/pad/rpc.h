/*
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

/*!
 * Header file for the PAD RPC implementation.
 *
 * @addtogroup PAD_SVC
 * @{
 */

#ifndef SC_PAD_RPC_H
#define SC_PAD_RPC_H

/* Includes */

/* Defines */

/*!
 * @name Defines for RPC PAD function calls
 */
/*@{*/
#define PAD_FUNC_UNKNOWN 0 /*!< Unknown function */
#define PAD_FUNC_SET_MUX 1U /*!< Index for pad_set_mux() RPC call */
#define PAD_FUNC_GET_MUX 6U /*!< Index for pad_get_mux() RPC call */
#define PAD_FUNC_SET_GP 2U /*!< Index for pad_set_gp() RPC call */
#define PAD_FUNC_GET_GP 7U /*!< Index for pad_get_gp() RPC call */
#define PAD_FUNC_SET_WAKEUP 4U /*!< Index for pad_set_wakeup() RPC call */
#define PAD_FUNC_GET_WAKEUP 9U /*!< Index for pad_get_wakeup() RPC call */
#define PAD_FUNC_SET_ALL 5U /*!< Index for pad_set_all() RPC call */
#define PAD_FUNC_GET_ALL 10U /*!< Index for pad_get_all() RPC call */
#define PAD_FUNC_SET 15U /*!< Index for pad_set() RPC call */
#define PAD_FUNC_GET 16U /*!< Index for pad_get() RPC call */
#define PAD_FUNC_SET_GP_28FDSOI 11U /*!< Index for pad_set_gp_28fdsoi() RPC call */
#define PAD_FUNC_GET_GP_28FDSOI 12U /*!< Index for pad_get_gp_28fdsoi() RPC call */
#define PAD_FUNC_SET_GP_28FDSOI_HSIC 3U /*!< Index for pad_set_gp_28fdsoi_hsic() RPC call */
#define PAD_FUNC_GET_GP_28FDSOI_HSIC 8U /*!< Index for pad_get_gp_28fdsoi_hsic() RPC call */
#define PAD_FUNC_SET_GP_28FDSOI_COMP 13U /*!< Index for pad_set_gp_28fdsoi_comp() RPC call */
#define PAD_FUNC_GET_GP_28FDSOI_COMP 14U /*!< Index for pad_get_gp_28fdsoi_comp() RPC call */
/*@}*/

/* Types */

/* Functions */

/*!
 * This function dispatches an incoming PAD RPC request.
 *
 * @param[in]     caller_pt   caller partition
 * @param[in]     msg         pointer to RPC message
 */
void pad_dispatch(sc_rm_pt_t caller_pt, sc_rsrc_t mu, sc_rpc_msg_t *msg);

#endif /* SC_PAD_RPC_H */

/**@}*/
