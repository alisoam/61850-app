/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_PHY_H_
#define _FSL_PHY_H_

#include "fsl_enet.h"

/*!
 * @addtogroup phy_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PHY_LINK_STATE_MASK 0x7
#define PHY_LINK_STATE0    (1 << 0)
#define PHY_LINK_STATE1    (1 << 1)
#define PHY_LINK_STATE2    (1 << 2)
#define PHY_LINK_ERROR     (1 << 4)	/*!< PHY status bit for link error */
#define PHY_LINK_BUSY      (1 << 5)	/*!< PHY status bit for MII link busy */
#define PHY_LINK_CHANGED   (1 << 6)	/*!< PHY status bit for changed state (not persistent) */
#define PHY_LINK_CONNECTED (1 << 7)	/*!< PHY status bit for connected state */
#define PHY_LINK_SPEED100  (1 << 8)	/*!< PHY status bit for 100Mbps mode */
#define PHY_LINK_FULLDUPLX (1 << 9)	/*!< PHY status bit for full duplex mode */

enum _phy_status
{
    kStatus_PHY_SMIVisitTimeout   = MAKE_STATUS(kStatusGroup_PHY, 1), /*!< ENET PHY SMI visit timeout. */
    kStatus_PHY_AutoNegotiateFail = MAKE_STATUS(kStatusGroup_PHY, 2)  /*!< ENET PHY AutoNegotiate Fail. */
};

struct PhyState {
  ENET_Type* base;
  uint32_t phy_addr;
  uint32_t phyustate;
  uint32_t physts;
  uint32_t olddphysts;
  uint32_t sts;
};

#if defined(__cplusplus)
extern "C" {
#endif

status_t PHY_Init(struct PhyState* state, ENET_Type *base, uint32_t phyAddr, uint32_t srcClock_Hz);

uint32_t lpcPHYStsPoll(struct PhyState* state);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_PHY_H_ */
