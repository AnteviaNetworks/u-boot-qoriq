/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2021 NXP
 */

#ifndef __LX2_HPCSOM_H
#define __LX2_HPCSOM_H

#include "lx2160a_common.h"

/* Qixis */
#define QIXIS_XMAP_MASK			0x01
#define QIXIS_XMAP_SHIFT		5
#define QIXIS_LBMAP_DFLTBANK		0x00
#define QIXIS_LBMAP_ALTBANK		QIXIS_XMAP_MASK << QIXIS_XMAP_SHIFT
#define QIXIS_LBMAP_QSPI		0x00
#define QIXIS_RCW_SRC_QSPI		0xff
#define QIXIS_RST_CTL_RESET		0x31
#define QIXIS_RCFG_CTL_RECONFIG_IDLE	0x20
#define QIXIS_RCFG_CTL_RECONFIG_START	0x21
#define QIXIS_RCFG_CTL_WATCHDOG_ENBLE	0x08
#define QIXIS_LBMAP_MASK		0x0f
#define QIXIS_LBMAP_SD
#define QIXIS_LBMAP_EMMC
#define QIXIS_RCW_SRC_SD           0x08
#define QIXIS_RCW_SRC_EMMC         0x09
#define NON_EXTENDED_DUTCFG

/* VID */

#define I2C_MUX_CH_VOL_MONITOR		0xA
/* Voltage monitor LTC3882 on channel 2 */
#define I2C_VOL_MONITOR_ADDR		0x63
#define I2C_VOL_MONITOR_BUS_V_OFFSET	0x2
#define I2C_VOL_MONITOR_BUS_V_OVF	0x1
#define I2C_VOL_MONITOR_BUS_V_SHIFT	3
#define CONFIG_VID_FLS_ENV		"lx2160hpcsom_vdd_mv"
#define CONFIG_VID

/* The lowest and highest voltage allowed */
#define VDD_MV_MIN			775
#define VDD_MV_MAX			855

/* PM Bus commands code for LTC3882 */
#define PMBUS_CMD_PAGE                  0x0
#define PMBUS_CMD_READ_VOUT             0x8B
#define PMBUS_CMD_PAGE_PLUS_WRITE       0x05
#define PMBUS_CMD_VOUT_COMMAND          0x21
#define PWM_CHANNEL0                    0x0

#define CONFIG_VOL_MONITOR_LTC3882_SET
#define CONFIG_VOL_MONITOR_LTC3882_READ

/* RTC */
#define CONFIG_SYS_RTC_BUS_NUM		0   /* Channel 4 I2C bus 0*/
#define I2C_MUX_CH_RTC			0xC /* Channel 4*/

/* MAC/PHY configuration */
#if defined(CONFIG_FSL_MC_ENET)
#define CONFIG_MII
#define CONFIG_ETHPRIME		"DPMAC17@rgmii-id"
// TODO - Update AQR PHY Address
#define AQR113_PHY_ADDR1	0x07
#define AQR113_PHY_ADDR2	0x0F
#define AQR113_PHY_ADDR3	0x07
#define AQR113_PHY_ADDR4	0x0F
#define AQR113_IRQ_MASK		0x0C

#define RGMII_PHY_ADDR1		0x01
#define RGMII_PHY_ADDR2		0x02

#endif

/* EMC2305 */
#define I2C_MUX_CH_EMC2305	0x9
#define I2C_EMC2305_ADDR	0x4D
#define I2C_EMC2305_CMD		0x40
#define I2C_EMC2305_PWM		0x80

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM	           0
#define CONFIG_SYS_I2C_EEPROM_ADDR	           0x57
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	    1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS     3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS 5

#undef SD_BOOTCOMMAND
#define SD_BOOTCOMMAND						\
		"env exists mcinitcmd && mmcinfo; "		\
		"mmc read 0x80d00000 0x6800 0x800; "		\
		"env exists mcinitcmd && env exists secureboot "	\
		" && mmc read 0x806C0000 0x3600 0x20 "		\
		"&& esbc_validate 0x806C0000;env exists mcinitcmd "	\
		"&& fsl_mc lazyapply dpl 0x80d00000;"		\
		"run sd_bootcmd;"		\
		"env exists secureboot && esbc_halt;"

#undef SD2_BOOTCOMMAND
#define SD2_BOOTCOMMAND						\
		"mmc dev 1; env exists mcinitcmd && mmcinfo; "	\
		"mmc read 0x80d00000 0x6800 0x800; "		\
		"env exists mcinitcmd && env exists secureboot "	\
		" && mmc read 0x806C0000 0x3600 0x20 "		\
		"&& esbc_validate 0x806C0000;env exists mcinitcmd "	\
		"&& fsl_mc lazyapply dpl 0x80d00000;"		\
		"run sd2_bootcmd;"		\
		"env exists secureboot && esbc_halt;"


/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	EXTRA_ENV_SETTINGS			\
	"boot_scripts=lx2160hpcsom_boot.scr\0"	\
	"boot_script_hdr=hdr_lx2160hpcsom_bs.out\0"	\
	"BOARD=lx2160ahpcsom\0"			\
	"sd_bootcmd=echo Trying load from sd card..;"		\
		"mmcinfo; mmc read $load_addr "			\
		"$kernel_addr_sd $kernel_size_sd ;"		\
		"env exists secureboot && mmc read $kernelheader_addr_r "\
		"$kernelhdr_addr_sd $kernelhdr_size_sd "	\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$BOARD\0"			\
	"sd2_bootcmd=echo Trying load from emmc card..;"	\
		"mmc dev 1; mmcinfo; mmc read $load_addr "	\
		"$kernel_addr_sd $kernel_size_sd ;"		\
		"env exists secureboot && mmc read $kernelheader_addr_r "\
		"$kernelhdr_addr_sd $kernelhdr_size_sd "	\
		" && esbc_validate ${kernelheader_addr_r};"	\
		"bootm $load_addr#$BOARD\0"

#include <asm/fsl_secure_boot.h>

#endif /* __LX2_HPCSOM_H */