/***************************************************************************//**
 *   @file   dummy_example.c
 *   @brief  DUMMY example for eval-adxl367z project
 *   @author Andrei Porumb (andrei.porumb@analog.com)
********************************************************************************
 * Copyright 2022(c) Analog Devices, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. “AS IS” AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ANALOG DEVICES, INC. BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "dummy_example.h"
#include "common_data.h"
#include "adxl367.h"
#include "no_os_print_log.h"
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/***************************************************************************//**
 * @brief Dummy example main execution.
 *
 * @return ret - Result of the example execution.
*******************************************************************************/
int dummy_example_main()
{
	int32_t ret;

	/* Device ADXL367 instance. */
	struct adxl367_dev *dev;

	ret = adxl367_init(&dev, init_param);
	if (ret)
		return -1;

	ret = adxl367_self_test(dev);
	if (ret)
		return -1;

	ret = adxl367_temp_read_en(dev, 1);
	if (ret)
		return -1;

	ret = adxl367_set_output_rate(dev, ADXL367_ODR_200HZ);
	if (ret)
		return -1;

	ret = adxl367_fifo_setup(dev, ADXL367_OLDEST_SAVED, ADXL367_FIFO_FORMAT_XYZT,
				 50);
	if (ret)
		return -1;

	//be sure to make all configs before switching to measure mode
	ret = adxl367_set_power_mode(dev, ADXL367_OP_MEASURE);
	if (ret)
		return -1;

	uint16_t entries;
	struct adxl367_fractional_val x[128], y[128], z[128], temp[128];

	ret = adxl367_read_converted_fifo(dev, x, y, z, temp, &entries);
	if (ret)
		return ret;

	pr_info("Number of read entries from the FIFO %d \n", entries);

	for (uint8_t i = 0; i < entries / 4; i ++) {
		pr_info("x=%d"".%09u m/s^2\n", (int)x[i].integer, (abs)(x[i].fractional));
		pr_info("y=%d"".%09u m/s^2\n", (int)y[i].integer, (abs)(y[i].fractional));
		pr_info("z=%d"".%09u m/s^2\n", (int)z[i].integer, (abs)(z[i].fractional));
		pr_info("temp=%d"".%09u C\n", (int)temp[i].integer, (abs)(temp[i].fractional));
		pr_info("\n");
	}

	return 0;
}
