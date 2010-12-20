#ifndef COMMANDS_H
#define COMMANDS_H

//! Command to force the openASC box into bootloader mode
#define COMPUTER_COMM_ENTER_BOOTLOADER		0x01
#define CTRL_REBOOT												0x02
#define CTRL_GET_FIRMWARE_REV							0x03

#define INT_COMM_PC_CTRL	0xC6

#define REMOTE_CONTROL_BUTTON_PRESSED		0x10
#define REMOTE_CONTROL_RX_ANT_TEXT			0x11
#define REMOTE_CONTROL_ANT_TEXT					0x12
#define REMOTE_CONTROL_GET_STATUS				0x13
#define REMOTE_CONTROL_CHANGE_BAND			0x14

/* EXTERNAL CTRL FUNCTIONS */
//! Ext ctrl - No function assigned
#define EXT_CTRL_SEL_NONE						0
//! Ext ctrl - Set RX antenna #1
#define EXT_CTRL_SEL_RX_ANT1					1
//! Ext ctrl - Set RX antenna #2
#define EXT_CTRL_SEL_RX_ANT2					2
//! Ext ctrl - Set RX antenna #3
#define EXT_CTRL_SEL_RX_ANT3					3
//! Ext ctrl - Set RX antenna #4
#define EXT_CTRL_SEL_RX_ANT4					4
//! Ext ctrl - Set RX antenna #5
#define EXT_CTRL_SEL_RX_ANT5					5
//! Ext ctrl - Set RX antenna #6
#define EXT_CTRL_SEL_RX_ANT6					6
//! Ext ctrl - Set RX antenna #7
#define EXT_CTRL_SEL_RX_ANT7					7
//! Ext ctrl - Set RX antenna #8
#define EXT_CTRL_SEL_RX_ANT8					8
//! Ext ctrl - Set RX antenna #9
#define EXT_CTRL_SEL_RX_ANT9					9
//! Ext ctrl - Set RX antenna #10
#define EXT_CTRL_SEL_RX_ANT10				10

//! Ext ctrl - Toggle TX antenna combination #1
#define EXT_CTRL_TOGGLE_TX_ANT1			11
//! Ext ctrl - Toggle TX antenna combination #2
#define EXT_CTRL_TOGGLE_TX_ANT2			12
//! Ext ctrl - Toggle TX antenna combination #3
#define EXT_CTRL_TOGGLE_TX_ANT3			13
//! Ext ctrl - Toggle TX antenna combination #4
#define EXT_CTRL_TOGGLE_TX_ANT4			14

//! Ext ctrl - Toggle RX antenna combination #1
#define EXT_CTRL_TOGGLE_RX_ANT1			15
//! Ext ctrl - Toggle RX antenna combination #2
#define EXT_CTRL_TOGGLE_RX_ANT2			16
//! Ext ctrl - Toggle RX antenna combination #3
#define EXT_CTRL_TOGGLE_RX_ANT3			17
//! Ext ctrl - Toggle RX antenna combination #4
#define EXT_CTRL_TOGGLE_RX_ANT4			18

//! Ext ctrl - Toggle RX antenna enabled
#define EXT_CTRL_TOGGLE_RX_ANT_MODE	19
//! Ext ctrl - Toggle TX/RX mode on/off
#define EXT_CTRL_TOGGLE_TXRX_MODE		20

//! Ext ctrl - Select array direction #1
#define EXT_CTRL_SET_ARRAY_DIR1			21
//! Ext ctrl - Select array direction #2
#define EXT_CTRL_SET_ARRAY_DIR2			22
//! Ext ctrl - Select array direction #3
#define EXT_CTRL_SET_ARRAY_DIR3			23
//! Ext ctrl - Select array direction #4
#define EXT_CTRL_SET_ARRAY_DIR4			24
//! Ext ctrl - Select array direction #5
#define EXT_CTRL_SET_ARRAY_DIR5			25
//! Ext ctrl - Select array direction #6
#define EXT_CTRL_SET_ARRAY_DIR6			26
//! Ext ctrl - Select array direction #7
#define EXT_CTRL_SET_ARRAY_DIR7			27
//! Ext ctrl - Select array direction #8
#define EXT_CTRL_SET_ARRAY_DIR8			28

//! Ext ctrl - Select stack combo #1
#define EXT_CTRL_SET_STACK_COMB1			29
//! Ext ctrl - Select stack combo #2
#define EXT_CTRL_SET_STACK_COMB2			30
//! Ext ctrl - Select stack combo #3
#define EXT_CTRL_SET_STACK_COMB3			31
//! Ext ctrl - Select stack combo #4
#define EXT_CTRL_SET_STACK_COMB4			32
//! Ext ctrl - Select stack combo #5
#define EXT_CTRL_SET_STACK_COMB5			33
//! Ext ctrl - Select stack combo #6
#define EXT_CTRL_SET_STACK_COMB6			34

//! Ext ctrl - Toggle the amplifier on/off
#define EXT_CTRL_AMPLIFIER_TOGGLE_ON_OFF	35
//! Ext ctrl - Toggle the amplifier standby
#define EXT_CTRL_AMPLIFIER_TOGGLE_STANDBY	36
//! Ext ctrl - Tune the amplifier to the correct band
#define EXT_CTRL_AMPLIFIER_TUNE	37
//! Ext ctrl - Reset the amplifier
#define EXT_CTRL_AMPLIFIER_RESET	38

//! Ext ctrl - Rotate the last antenna clock wise
#define EXT_CTRL_ROTATE_CW	39
//! Ext ctrl - Rotate the last antenna counter clock wise
#define EXT_CTRL_ROTATE_CCW 40

//! Ext ctrl - Select band portion
#define EXT_CTRL_SEL_BAND_PORTION 41
//! Ext ctrl - Change band up
#define EXT_CTRL_CHANGE_BAND_UP		42
//! Ext ctrl - Change band down
#define EXT_CTRL_CHANGE_BAND_DOWN 43

#endif // COMMANDS_H
