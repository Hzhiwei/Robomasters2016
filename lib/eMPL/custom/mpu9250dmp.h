#ifndef __DMP_H
#define __DMP_H


#include "stm32f4xx.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


/* Data requested by client. */
#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)

#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)
#define QUAT_ON         (0x04)

#define MOTION          (0)
#define NO_MOTION       (1)

/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (200)

#define FLASH_SIZE      (512)
#define FLASH_MEM_START ((void*)0x1800)
    
#define Acceleration_Of_Gravity     9.87F
    

#ifdef  __DMP_GLOBALS
#define __DMP_EXT
#else
#define __DMP_EXT extern
#endif


struct hal_s {
    unsigned char sensors;
    unsigned char dmp_on;
    unsigned char wait_for_tap;
    volatile unsigned char new_gyro;
    unsigned short report;
    unsigned short dmp_features;
    unsigned char motion_int_mode;
};


uint8_t MPU9250DMP_InitConfig(uint8_t mode);
uint8_t MPU9250DMP_GetEuler(float *Pitch, float *Roll, float *Yaw);
uint8_t MPU9250_GetGyro(float *X, float *Y, float *Z);
uint8_t MPU9250_GetAccel(float *X, float *Y, float *Z);

static inline unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
static inline unsigned short inv_row_2_scale(const signed char *row);
static uint8_t MPU9250DMPU_Selftest(uint8_t mode);

void mpu_delay_ms(int x);
void mpu_get_ms(unsigned long *p);
void mpu_accel_lpfset(void);



#endif
