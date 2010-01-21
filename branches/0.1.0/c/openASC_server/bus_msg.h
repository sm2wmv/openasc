#ifndef __BUS_MSG_H_
#define __BUS_MSG_H_
/*! BUS message structure */
typedef struct {
	//! From which address the bus message was sent
	unsigned char from_addr;
	//! To which address the bus message should be sent
	unsigned char to_addr;
	//! The checksum of the message
	unsigned char checksum;
	//! Different flags
	unsigned char flags;
	//! The command of the message
	unsigned char cmd;
	//! The length of the data sent in the message
	unsigned char length;
	//! The variables wanted to be sent
	unsigned char data[15];
} MSG;
//} __attribute__((packed)) MSG;
#endif
