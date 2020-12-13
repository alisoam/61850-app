/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_phy.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the timeout macro. */
#define PHY_TIMEOUT_COUNT 200
#define PHY_MII_TIMEOUT_COUNT 5

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the ENET instance from peripheral base address.
 *
 * @param base ENET peripheral base address.
 * @return ENET instance.
 */
extern uint32_t ENET_GetInstance(ENET_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to enet clocks for each instance. */
extern clock_ip_name_t s_enetClock[FSL_FEATURE_SOC_ENET_COUNT];
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
/* LAN8720 PHY register offsets */
#define LAN8_BCR_REG        0x0	/*!< Basic Control Register */
#define LAN8_BSR_REG        0x1	/*!< Basic Status Reg */
#define LAN8_PHYID1_REG     0x2	/*!< PHY ID 1 Reg  */
#define LAN8_PHYID2_REG     0x3	/*!< PHY ID 2 Reg */
#define LAN8_PHYSPLCTL_REG  0x1F/*!< PHY special control/status Reg */

/* LAN8720 BCR register definitions */
#define LAN8_RESET          (1 << 15)	/*!< 1= S/W Reset */
#define LAN8_LOOPBACK       (1 << 14)	/*!< 1=loopback Enabled */
#define LAN8_SPEED_SELECT   (1 << 13)	/*!< 1=Select 100MBps */
#define LAN8_AUTONEG        (1 << 12)	/*!< 1=Enable auto-negotiation */
#define LAN8_POWER_DOWN     (1 << 11)	/*!< 1=Power down PHY */
#define LAN8_ISOLATE        (1 << 10)	/*!< 1=Isolate PHY */
#define LAN8_RESTART_AUTONEG (1 << 9)	/*!< 1=Restart auto-negoatiation */
#define LAN8_DUPLEX_MODE    (1 << 8)	/*!< 1=Full duplex mode */

/* LAN8720 BSR register definitions */
#define LAN8_100BASE_T4     (1 << 15)	/*!< T4 mode */
#define LAN8_100BASE_TX_FD  (1 << 14)	/*!< 100MBps full duplex */
#define LAN8_100BASE_TX_HD  (1 << 13)	/*!< 100MBps half duplex */
#define LAN8_10BASE_T_FD    (1 << 12)	/*!< 100Bps full duplex */
#define LAN8_10BASE_T_HD    (1 << 11)	/*!< 10MBps half duplex */
#define LAN8_AUTONEG_COMP   (1 << 5)	/*!< Auto-negotation complete */
#define LAN8_RMT_FAULT      (1 << 4)	/*!< Fault */
#define LAN8_AUTONEG_ABILITY (1 << 3)	/*!< Auto-negotation supported */
#define LAN8_LINK_STATUS    (1 << 2)	/*!< 1=Link active */
#define LAN8_JABBER_DETECT  (1 << 1)	/*!< Jabber detect */
#define LAN8_EXTEND_CAPAB   (1 << 0)	/*!< Supports extended capabilities */

/* LAN8720 PHYSPLCTL status definitions */
#define LAN8_SPEEDMASK      (7 << 2)	/*!< Speed and duplex mask */
#define LAN8_SPEED100F      (6 << 2)	/*!< 100BT full duplex */
#define LAN8_SPEED10F       (5 << 2)	/*!< 10BT full duplex */
#define LAN8_SPEED100H      (2 << 2)	/*!< 100BT half duplex */
#define LAN8_SPEED10H       (1 << 2)	/*!< 10BT half duplex */

/* LAN8720 PHY ID 1/2 register definitions */
#define LAN8_PHYID1_OUI     0x0007		/*!< Expected PHY ID1 */
#define LAN8_PHYID2_OUI     0xC0F0		/*!< Expected PHY ID2, except last 4 bits */


/* Pointer to delay function used for this driver */
extern void pDelayMs(uint32_t ms);




status_t PHY_Write(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t data)
{
  uint32_t counter;

  /* Clear the SMI interrupt event. */
  ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

  /* Starts a SMI write command. */
  ENET_StartSMIWrite(base, phyAddr, phyReg, kENET_MiiWriteValidFrame, data);

  /* Wait for SMI complete. */
  for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
  {
    if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
    {
      break;
    }
    pDelayMs(1);
  }

  /* Check for timeout. */
  if (!counter)
  {
    return kStatus_PHY_SMIVisitTimeout;
  }

  /* Clear MII interrupt event. */
  ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

  return kStatus_Success;
}

status_t PHY_Read(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t *dataPtr)
{
  assert(dataPtr);

  uint32_t counter;

  /* Clear the MII interrupt event. */
  ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

  /* Starts a SMI read command operation. */
  ENET_StartSMIRead(base, phyAddr, phyReg, kENET_MiiReadValidFrame);

  /* Wait for MII complete. */
  for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
  {
    if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
    {
      break;
    }
    pDelayMs(1);
  }

  /* Check for timeout. */
  if (!counter)
  {
    return kStatus_PHY_SMIVisitTimeout;
  }

  /* Get data from MII register. */
  *dataPtr = ENET_ReadSMIData(base);

  /* Clear MII interrupt event. */
  ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

  return kStatus_Success;
}




/* Update PHY status from passed value */
static void smsc_update_phy_sts(struct PhyState* state, uint16_t sdsts)
{
  /* Update link active status */
  if (state->sts & LAN8_LINK_STATUS)
    state->physts |= PHY_LINK_CONNECTED;
  else
    state->physts &= ~PHY_LINK_CONNECTED;

  switch (sdsts & LAN8_SPEEDMASK) {
    case LAN8_SPEED100F:
    default:
      state->physts |= PHY_LINK_SPEED100;
      state->physts |= PHY_LINK_FULLDUPLX;
      break;

    case LAN8_SPEED10F:
      state->physts &= ~PHY_LINK_SPEED100;
      state->physts |= PHY_LINK_FULLDUPLX;
      break;

    case LAN8_SPEED100H:
      state->physts |= PHY_LINK_SPEED100;
      state->physts &= ~PHY_LINK_FULLDUPLX;
      break;

    case LAN8_SPEED10H:
      state->physts &= ~PHY_LINK_SPEED100;
      state->physts &= ~PHY_LINK_FULLDUPLX;
      break;
  }

  /* If the status has changed, indicate via change flag */
  if ((state->physts & (PHY_LINK_SPEED100 | PHY_LINK_FULLDUPLX | PHY_LINK_CONNECTED)) !=
      (state->olddphysts & (PHY_LINK_SPEED100 | PHY_LINK_FULLDUPLX | PHY_LINK_CONNECTED))) {
    state->olddphysts = state->physts;
    state->physts |= PHY_LINK_CHANGED;
  }
}


/* Initialize the SMSC 87x0 PHY */
status_t PHY_Init(struct PhyState* state, ENET_Type *base, uint32_t phy_addr, uint32_t srcClock_Hz)
{
  uint32_t instance = ENET_GetInstance(base);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
  /* Set SMI first. */
//  CLOCK_EnableClock(s_enetClock[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
  ENET_SetSMI(base, srcClock_Hz, false);
  uint32_t tmp;
  int32_t i;


  state->base = base;
  state->phy_addr = phy_addr;
  /* Initial states for PHY status and state machine */
  state->olddphysts = 0;
  state->physts = 0;
  state->phyustate = 0;

  /* Only first read and write are checked for failure */
  /* Put the DP83848C in reset mode and wait for completion */
  status_t result = PHY_Write(base, phy_addr, LAN8_BCR_REG, LAN8_RESET);
  if (result != kStatus_Success)
    return result;

  i = 400;
  while (i > 0) {
    pDelayMs(1);
    result = PHY_Read(base, phy_addr, LAN8_BCR_REG, &tmp);
    if (result != kStatus_Success)
      return result;

    if (!(tmp & (LAN8_RESET | LAN8_POWER_DOWN)))
      i = -1;
    else
      i--;
  }
  /* Timeout? */
  if (i == 0)
    return kStatus_PHY_SMIVisitTimeout;

  /* Setup link */
  result = PHY_Write(base, phy_addr, LAN8_BCR_REG, LAN8_AUTONEG); //| LAN8_LOOPBACK);
  if (result != kStatus_Success)
    return result;

  result = PHY_Read(base, phy_addr, LAN8_PHYID1_REG, &tmp);
  if (result != kStatus_Success)
    return result;
  if (tmp != LAN8_PHYID1_OUI)
    return kStatus_Fail;

  result = PHY_Read(base, phy_addr, LAN8_PHYID2_REG, &tmp);
  if (result != kStatus_Success)
    return result;
  if ((tmp & 0xfff0) != LAN8_PHYID2_OUI)
    return kStatus_Fail;

  /* The link is not set active at this point, but will be detected
     later */
  return kStatus_Success;
}

/* Phy status update state machine */
uint32_t lpcPHYStsPoll(struct PhyState* state) {
  switch (state->phyustate) {
    default:
    case 0:
      /* Read BMSR to clear faults */
      ENET_StartSMIRead(state->base, state->phy_addr, LAN8_BSR_REG, kENET_MiiReadValidFrame);
      state->physts &= ~PHY_LINK_CHANGED;
      state->physts = state->physts | PHY_LINK_BUSY;
      state->phyustate = 1;
      state->timeout_counter = PHY_MII_TIMEOUT_COUNT;
      break;

    case 1:
      /* Wait for read status state */
      if (ENET_GetInterruptStatus(state->base) & ENET_EIR_MII_MASK) {
        /* Get PHY status with link state */
        state->sts = ENET_ReadSMIData(state->base);
        ENET_StartSMIRead(state->base, state->phy_addr, LAN8_PHYSPLCTL_REG, kENET_MiiReadValidFrame);
        state->phyustate = 2;
        state->timeout_counter = PHY_MII_TIMEOUT_COUNT;
      }
      break;

    case 2:
      /* Wait for read status state */
      if (ENET_GetInterruptStatus(state->base) & ENET_EIR_MII_MASK) {
        /* Update PHY status */
        uint16_t new_sts = ENET_ReadSMIData(state->base) & 0xffff;
        state->physts &= ~PHY_LINK_BUSY;
        smsc_update_phy_sts(state, new_sts);
        state->phyustate = 0;
      }
      break;
  }

  if (state->timeout_counter > 0)
    state->timeout_counter -= 1;
  else if (state->phyustate != 0)
    state->phyustate = 0;

  return state->physts;
}
