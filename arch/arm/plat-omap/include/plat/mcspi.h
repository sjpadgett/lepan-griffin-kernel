#ifndef _OMAP2_MCSPI_H
#define _OMAP2_MCSPI_H

#define OMAP2_MCSPI_REV 0
#define OMAP3_MCSPI_REV 1
#define OMAP4_MCSPI_REV 2

#define OMAP4_MCSPI_REG_OFFSET 0x100
#define OMAP2_MCSPI_MASTER		0
#define OMAP2_MCSPI_SLAVE		1

/**
 * struct omap2_mcspi_platform_config - McSPI controller configuration
 * @num_cs: Number of chip selects or channels supported
 * @mode: SPI is master or slave
 * @dma_mode: Use only DMA for data transfers
 * @force_cs_mode: Use force chip select mode or auto chip select mode
 * @fifo_depth: FIFO depth in bytes, max value 64
 *
 * @dma_mode when set to 1 uses only dma for data transfers
 * else the default behaviour is to use PIO mode for transfer
 * size of 8 bytes or less. This mode is useful when mcspi
 * is configured as slave
 *
 * @force_cs_mode when set to 1 allows continuous transfer of multiple
 * spi words without toggling the chip select line.
 *
 * @fifo_depth when set to non zero values enables FIFO. fifo_depth
 * should be set as a multiple of buffer size used for read/write.
 */

struct omap2_mcspi_platform_config {
	unsigned short	num_cs;
	unsigned int regs_offset;
	unsigned short	mode;
	unsigned short	dma_mode;
	unsigned short	force_cs_mode;
	unsigned short  fifo_depth;
};

struct omap2_mcspi_dev_attr {
	unsigned short num_chipselect;
	unsigned short	mode;
	unsigned short dma_mode;
	unsigned short force_cs_mode;
	unsigned short fifo_depth;
};

struct omap2_mcspi_device_config {
	unsigned turbo_mode:1;

	/* Do we want one channel enabled at the same time? */
	unsigned single_channel:1;
	/* Swap data lines */
	unsigned swap_datalines;
};

#endif
