add_library(device STATIC
  ./system/fsl/drivers/fsl_adc.c
  ./system/fsl/drivers/fsl_adc_etc.c
  ./system/fsl/drivers/fsl_aipstz.c
  ./system/fsl/drivers/fsl_aoi.c
  ./system/fsl/drivers/fsl_bee.c
  ./system/fsl/drivers/fsl_cache.c
  ./system/fsl/drivers/fsl_clock.c
  ./system/fsl/drivers/fsl_cmp.c
  ./system/fsl/drivers/fsl_common.c
  ./system/fsl/drivers/fsl_csi.c
  ./system/fsl/drivers/fsl_dcdc.c
  ./system/fsl/drivers/fsl_dcp.c
  ./system/fsl/drivers/fsl_dmamux.c
  ./system/fsl/drivers/fsl_edma.c
  ./system/fsl/drivers/fsl_elcdif.c
  ./system/fsl/drivers/fsl_enc.c
  ./system/fsl/drivers/fsl_enet.c
  ./system/fsl/drivers/fsl_ewm.c
  ./system/fsl/drivers/fsl_flexcan.c
  ./system/fsl/drivers/fsl_flexcan_edma.c
  ./system/fsl/drivers/fsl_flexio.c
  ./system/fsl/drivers/fsl_flexio_camera.c
  ./system/fsl/drivers/fsl_flexio_camera_edma.c
  ./system/fsl/drivers/fsl_flexio_i2c_master.c
  ./system/fsl/drivers/fsl_flexio_i2s.c
  ./system/fsl/drivers/fsl_flexio_i2s_edma.c
  ./system/fsl/drivers/fsl_flexio_mculcd.c
  ./system/fsl/drivers/fsl_flexio_mculcd_edma.c
  ./system/fsl/drivers/fsl_flexio_spi.c
  ./system/fsl/drivers/fsl_flexio_spi_edma.c
  ./system/fsl/drivers/fsl_flexio_uart.c
  ./system/fsl/drivers/fsl_flexio_uart_edma.c
  ./system/fsl/drivers/fsl_flexram_allocate.c
  ./system/fsl/drivers/fsl_flexram.c
  ./system/fsl/drivers/fsl_flexspi.c
  ./system/fsl/drivers/fsl_gpc.c
  ./system/fsl/drivers/fsl_gpio.c
  ./system/fsl/drivers/fsl_gpt.c
  ./system/fsl/drivers/fsl_kpp.c
  ./system/fsl/drivers/fsl_lpi2c.c
  ./system/fsl/drivers/fsl_lpi2c_edma.c
  ./system/fsl/drivers/fsl_lpi2c_freertos.c
  ./system/fsl/drivers/fsl_lpspi.c
  ./system/fsl/drivers/fsl_lpspi_edma.c
  ./system/fsl/drivers/fsl_lpspi_freertos.c
  ./system/fsl/drivers/fsl_lpuart.c
  ./system/fsl/drivers/fsl_lpuart_edma.c
  ./system/fsl/drivers/fsl_lpuart_freertos.c
  ./system/fsl/drivers/fsl_ocotp.c
  ./system/fsl/drivers/fsl_pit.c
  ./system/fsl/drivers/fsl_pmu.c
  ./system/fsl/drivers/fsl_pwm.c
  ./system/fsl/drivers/fsl_pxp.c
  ./system/fsl/drivers/fsl_qtmr.c
  ./system/fsl/drivers/fsl_rtwdog.c
  ./system/fsl/drivers/fsl_sai.c
  ./system/fsl/drivers/fsl_sai_edma.c
  ./system/fsl/drivers/fsl_semc.c
  ./system/fsl/drivers/fsl_snvs_hp.c
  ./system/fsl/drivers/fsl_snvs_lp.c
  ./system/fsl/drivers/fsl_spdif.c
  ./system/fsl/drivers/fsl_spdif_edma.c
  ./system/fsl/drivers/fsl_src.c
  ./system/fsl/drivers/fsl_tempmon.c
  ./system/fsl/drivers/fsl_trng.c
  ./system/fsl/drivers/fsl_tsc.c
  ./system/fsl/drivers/fsl_usdhc.c
  ./system/fsl/drivers/fsl_wdog.c
  ./system/fsl/drivers/fsl_xbara.c
  ./system/fsl/drivers/fsl_xbarb.c

  ./system/fsl/system_MIMXRT1062.c

  ./system/components/osa/fsl_os_abstraction_free_rtos.c
  ./system/components/lists/generic_list.c

  ./system/middleware/usb/device/usb_device_dci.c
  ./system/middleware/usb/device/usb_device_ehci.c
  ./system/middleware/usb/device/usb_device_khci.c
)

target_include_directories(device PUBLIC
  ./system/fsl/
  ./system/fsl/drivers
  ./system/fsl/utils
  ./system/fsl/include
  ./system/components/lists
  ./system/components/osa
  ./system/middleware/usb/include
  ./system/middleware/usb/device

#  ./system/fsl/utils/debug_console
#  ./system/fsl/utils/str
  ./modules/cmsis/CMSIS/Core/Include/
)

add_definitions(
  -DCORE_M7
  -DCPU_MIMXRT1062CVL5A
  -DFSL_RTOS_FREE_RTOS
  -DSDK_DEBUGCONSOLE=0
)

target_link_libraries(device
  FreeRTOS
)
