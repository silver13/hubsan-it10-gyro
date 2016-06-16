/* 
Copyright 2013 Brad Quick

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gyro.h"
#include "lib_i2c.h"
#include "lib_timers.h"
#include "rx.h"
#include "lib_fp.h"
#include "bradwii.h"

extern globalstruct global;

// when adding gyros, the following functions need to be included:
// initgyro() // initializes the gyro
// readgyro() // loads global.gyrorate with gyro readings in fixedpointnum degrees per second
#define GYRO_TYPE HUBSAN_NEW_GYRO

#if ( GYRO_TYPE== HUBSAN_NEW_GYRO )

#define UNK_INVENSENSE_ADDRESS 0x69

#if (GYRO_LOW_PASS_FILTER<=6)
#define UNK_INVENSENSE_DLPF_CFG GYRO_LOW_PASS_FILTER
#else
#define UNK_INVENSENSE_DLPF_CFG   6
#endif

void initgyro(void)
{
lib_i2c_writereg(UNK_INVENSENSE_ADDRESS, 107, 128 );
lib_timers_delaymilliseconds(1);
	// gyro scale 2000 deg (FS =3)
lib_i2c_writereg(UNK_INVENSENSE_ADDRESS, 27, 24 );
	// Gyro DLPF
lib_i2c_writereg(UNK_INVENSENSE_ADDRESS, 26, UNK_INVENSENSE_DLPF_CFG ); 
}

void readgyro(void)
{
    unsigned char data[6];

    lib_i2c_readdata( 0x69 , 0x43, (unsigned char *) &data, 6);

    // convert to fixedpointnum, in degrees per second
    // the gyro puts out a 16 bit signed int where each count equals 0.0609756097561 degrees/second
    // So we have 15 bit fractional part, need to shift that to FIXEDPOINTSHIFT and
    // take 2000deg/s into account.
    // This only works if FIXEDPOINTSHIFT >= 15
 
	    GYRO_ORIENTATION(global.gyrorate,
        ((int16_t) ((data[0] << 8) | data[1]) )* 4000L,
        ((int16_t) ((data[2] << 8) | data[3]) )* 4000L ,
        ((int16_t) ((data[4] << 8) | data[5]) )* 4000L);

}
//#elif
// the other gyros here
#endif



