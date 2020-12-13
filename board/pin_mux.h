/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK 0x200006U /*!< GPIO1 and GPIO6 share same IO MUX function, GPIO_MUX1 selects one GPIO function: affected bits mask */
#define BOARD_INITPINS_IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL_MASK 0x40000100U /*!< GPIO2 and GPIO7 share same IO MUX function, GPIO_MUX2 selects one GPIO function: affected bits mask */

/* GPIO_AD_B0_01 (coord H10), LED1 */
/* Routed pin properties */
#define BOARD_INITPINS_LED1_PERIPHERAL                                     GPIO1   /*!< Peripheral name */
#define BOARD_INITPINS_LED1_SIGNAL                                       gpio_io   /*!< Signal name */
#define BOARD_INITPINS_LED1_CHANNEL                                           1U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED1_GPIO                                           GPIO1   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_LED1_GPIO_PIN                                          1U   /*!< GPIO pin number */
#define BOARD_INITPINS_LED1_GPIO_PIN_MASK                             (1U << 1U)   /*!< GPIO pin mask */

/* GPIO_AD_B0_02 (coord M11), LED2 */
/* Routed pin properties */
#define BOARD_INITPINS_LED2_PERIPHERAL                                     GPIO1   /*!< Peripheral name */
#define BOARD_INITPINS_LED2_SIGNAL                                       gpio_io   /*!< Signal name */
#define BOARD_INITPINS_LED2_CHANNEL                                           2U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED2_GPIO                                           GPIO1   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_LED2_GPIO_PIN                                          2U   /*!< GPIO pin number */
#define BOARD_INITPINS_LED2_GPIO_PIN_MASK                             (1U << 2U)   /*!< GPIO pin mask */

/* GPIO_B1_14 (coord C14), LED3 */
/* Routed pin properties */
#define BOARD_INITPINS_LED3_PERIPHERAL                                     GPIO2   /*!< Peripheral name */
#define BOARD_INITPINS_LED3_SIGNAL                                       gpio_io   /*!< Signal name */
#define BOARD_INITPINS_LED3_CHANNEL                                          30U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED3_GPIO                                           GPIO2   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_LED3_GPIO_PIN                                         30U   /*!< GPIO pin number */
#define BOARD_INITPINS_LED3_GPIO_PIN_MASK                            (1U << 30U)   /*!< GPIO pin mask */

/* GPIO_AD_B1_05 (coord K12), ENET1_RST */
/* Routed pin properties */
#define BOARD_INITPINS_ENET1_RST_PERIPHERAL                                GPIO1   /*!< Peripheral name */
#define BOARD_INITPINS_ENET1_RST_SIGNAL                                  gpio_io   /*!< Signal name */
#define BOARD_INITPINS_ENET1_RST_CHANNEL                                     21U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_ENET1_RST_GPIO                                      GPIO1   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_ENET1_RST_GPIO_PIN                                    21U   /*!< GPIO pin number */
#define BOARD_INITPINS_ENET1_RST_GPIO_PIN_MASK                       (1U << 21U)   /*!< GPIO pin mask */

/* GPIO_B0_08 (coord B9), ENET2_RST */
/* Routed pin properties */
#define BOARD_INITPINS_ENET2_RST_PERIPHERAL                                GPIO2   /*!< Peripheral name */
#define BOARD_INITPINS_ENET2_RST_SIGNAL                                  gpio_io   /*!< Signal name */
#define BOARD_INITPINS_ENET2_RST_CHANNEL                                      8U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_ENET2_RST_GPIO                                      GPIO2   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_ENET2_RST_GPIO_PIN                                     8U   /*!< GPIO pin number */
#define BOARD_INITPINS_ENET2_RST_GPIO_PIN_MASK                        (1U << 8U)   /*!< GPIO pin mask */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
