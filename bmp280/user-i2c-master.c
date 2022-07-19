/* 
 * IMPORTANT: because of the use of inline functions, you have 
 * to use ‘-O’ or some variation when you compile your program!
 */


/* ###### headers to access an I2C adapter ###### */

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * i2c_smbus_read_word_data - SMBus "read word" protocol
 * @client: Handle to slave device
 * @command: Byte interpreted by slave
 *
 * This executes the SMBus "read word" protocol, returning negative errno
 * else a 16-bit unsigned "word" received from the device.
 */
__s32 i2c_smbus_read_word_data(const struct i2c_client *client, __u8 command)
{
	union i2c_smbus_data data;
	int status;

	status = i2c_smbus_xfer(client->adapter, client->addr, client->flags,
				I2C_SMBUS_READ, command,
				I2C_SMBUS_WORD_DATA, &data);
	return (status < 0) ? status : data.word;
}

int main (int argc, char *argv[]){

	/* ###### open the device file ###### */
	int file;
	int adapter_nr = 0; /* probably dynamically determined */
	char filename[20];

	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) {
	  /* ERROR HANDLING; you can check errno to see what went wrong */
	  exit(1);
	}

	/* ###### specify with what device address you want to communicate ###### */
	int addr = 0x76; /* The I2C address */

	if (ioctl(file, I2C_SLAVE, addr) < 0) {
	  /* ERROR HANDLING; you can check errno to see what went wrong */
	  exit(1);
	}	  
	
	/* ###### SMBus commands ###### */

	__u8 reg = 0xd0; /* Device register to access */
	__s32 res;
	char buf[10];

	/* Using SMBus commands */
	res = i2c_smbus_read_word_data(file, reg);
	if (res < 0) {
	  /* ERROR HANDLING: I2C transaction failed */
	} else {
	  /* res contains the read word */
	  printf("Word read: %d\n", res);
	}

	/*
	 * Using I2C Write, equivalent of
	 * i2c_smbus_write_word_data(file, reg, 0x6543)
	 */
	buf[0] = reg;
	buf[1] = 0x43;
	buf[2] = 0x65;
	if (write(file, buf, 3) != 3) {
	  /* ERROR HANDLING: I2C transaction failed */
	}

	/* Using I2C Read, equivalent of i2c_smbus_read_byte(file) */
	if (read(file, buf, 1) != 1) {
	  /* ERROR HANDLING: I2C transaction failed */
	} else {
	  /* buf[0] contains the read byte */
	}
	  
	return 0;

}



