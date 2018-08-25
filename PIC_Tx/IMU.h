#ifndef IMU_H
#define	IMU_H


UBYTE sampling_counter[2] = {};

void readIMUsequence_ICM(unsigned char, unsigned char, unsigned char, int);
void readIMUsequence_adxl_ITG(unsigned char, unsigned char, unsigned char, int);


#endif	/* ADXL_H */