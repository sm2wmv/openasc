/*! \file     general_io/sk3w_rotator_control/bus_handler.h
 *  \ingroup  general_io_group
 *  \brief    Handle bus communications for this application
 *  \author   Tobias Blomberg, SN0SVX
 *  \date     2012-06-16
 *
 * Contains functions for handling application specific bus communications.
 */
//    Copyright (C) 2012  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.


/******************************************************************************
 *
 * Standard library includes
 *
 *****************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <avr/io.h>


/******************************************************************************
 *
 * Project includes
 *
 *****************************************************************************/

#include <delay.h>
#include <wmv_bus/bus.h>
#include <wmv_bus/bus_ping.h>
#include <wmv_bus/bus_commands.h>
#include <queue.h>
#include <global.h>


/******************************************************************************
 *
 * Local includes
 *
 *****************************************************************************/

#include "rotator.h"
#include "bsp.h"
#include "bus_handler.h"


/******************************************************************************
 *
 * Macros and type definitions
 *
 *****************************************************************************/

#define MAX_ASCII_CMD_ARGS      5


/******************************************************************************
 *
 * Global variable declarations
 *
 *****************************************************************************/

//! Counter to keep track of the time elapsed since the last sync message was sent
static uint16_t counter_sync = 0;
//! Counter to keep track of when to send a ping out on the bus
static uint16_t counter_ping_interval = 0;


/******************************************************************************
 *
 * Function prototypes for private functions
 *
 *****************************************************************************/

static int16_t range_adjust_heading(int16_t heading);
static void parse_ascii_cmd(BUS_MESSAGE *bus_message);
static void bus_parse_message(BUS_MESSAGE *bus_message);
static unsigned char read_ext_addr(void);
static void send_help(uint8_t addr);


/******************************************************************************
 *
 * Public functions
 *
 *****************************************************************************/

void bus_handler_init() {
    /* Read the external address of the device */
  bus_set_address(BUS_BASE_ADDR + read_ext_addr());

    /* This delay is simply so that if you have the devices connected to the
     * same power supply all units should not send their status messages at the
     * same time. Therefor we insert a delay that is based on the external
     * address of the device */
  for (unsigned char i = 0; i < bus_get_address(); i++) {
    delay_ms(DEFAULT_STARTUP_DELAY);
  }

    /* Initialize the communication bus */
  bus_init();
  bus_ping_init();

  if ((BUS_BASE_ADDR + read_ext_addr()) == 0x01) {
    bus_set_is_master(1, DEF_NR_DEVICES);
  }
  else {
    bus_set_is_master(0, 0);
  }

    /* Timer used for the communication bus. The interrupt is caught in bus.c */
  bsp_init_timer_2();
}


void bus_handler_tick(void) {
  ++counter_sync;
  ++counter_ping_interval;

  bus_ping_tick();
}


void bus_handler_poll_core() {
  unsigned char device_count = bus_get_device_count();

  bus_check_tx_status();

    /* If this device should act as master it should send out a SYNC command
     * and also the number of devices (for the time slots) that are active on the bus */
  if (bus_is_master()) {
    if (counter_sync >= BUS_MASTER_SYNC_INTERVAL) {
      bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0,
                         BUS_CMD_SYNC, 1, &device_count);
      counter_sync = 0;
    }
  }
}


void bus_handler_poll() {
  bus_handler_poll_core();

  BUS_MESSAGE msg;
  if (bus_check_rx_status(&msg)) {
    bus_parse_message(&msg);
  }

  if (bus_allowed_to_send()) {
      /* Check if a ping message should be sent out on the bus */
    if (counter_ping_interval >= BUS_DEVICE_STATUS_MESSAGE_INTERVAL) {
        /* Check if the device is a POS or NEG driver module */
      unsigned char device_id = DEVICE_ID_GENERAL_IO;
      bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0,
                         BUS_CMD_PING, 1, &device_id);
      counter_ping_interval = 0;
    }
  }
}


void send_ascii_data(unsigned char to_addr, const char *fmt, ...) {
  char str[46];

  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(str, sizeof(str), fmt, ap);
  va_end(ap);

  if (len >= sizeof(str) - 1) {
    strcpy(str + sizeof(str) - 6, "...\r\n");
    len = sizeof(str) - 1;
  }

  char *ptr = str;
  while (len > 0) {
    unsigned char len_to_send = len < 15 ? len : 15;
    bus_add_tx_message(bus_get_address(),
                       to_addr,
                       (1 << BUS_MESSAGE_FLAGS_NEED_ACK),
                       BUS_CMD_ASCII_DATA,
                       len_to_send, (unsigned char *)ptr);
    len -= len_to_send;
    ptr += len_to_send;
  }
}


/******************************************************************************
 *
 * Externally declared functions
 *
 *****************************************************************************/

/**
 * \brief Called by the rotator code when a new heading measurement is available
 * \param rot_idx The rotator index
 * \param dir The heading in degrees
 *
 * This function is called from the rotator handling code when a new heading
 * value is available for a specific rotator.
 */
extern void rotator_direction_updated(uint8_t rot_idx, int16_t dir) {
  if (bus_allowed_to_send()) {
    unsigned char data[6];
    data[0] = DEVICE_ID_ROTATOR_UNIT;
    uint16_t current_heading = range_adjust_heading(dir);
    int16_t target_heading =
        range_adjust_heading(rotator_target_heading(rot_idx));
    if (target_heading == INT16_MAX) {
      target_heading = current_heading;
    }

    data[1] = rot_idx;                   /* Sub address */
    data[2] = (current_heading >> 8);    /* Hi current heading */
    data[3] = (current_heading & 0xff);  /* Lo current heading */
    data[4] = (target_heading >> 8);     /* Hi target heading */
    data[5] = (target_heading & 0xff);   /* Lo target heading */
    bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0,
                       BUS_CMD_ROTATOR_STATUS_UPDATE, sizeof(data), data);
  }
}


/**
 * \brief   Called by the rotator code when an error condition occurrs
 * \param   rot_idx The rotator index
 * \param   error The error code
 */
extern void rotator_error(uint8_t rot_idx, RotatorError error) {
  if (bus_allowed_to_send()) {
    uint8_t data[3];
    data[0] = rot_idx;
    data[2] = error;
    if (error != ROTATOR_ERROR_OK) {
      data[1] = 1;
      send_ascii_data(0, "ROTATOR #%d ERROR: %s\r\n",
                      rot_idx, rotator_strerror(error));
    }
    else {
      data[1] = 0;
      send_ascii_data(0, "ROTATOR #%d ERROR CLEARED\r\n", rot_idx);
    }
    bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0,
                        BUS_CMD_ROTATOR_ERROR, sizeof(data), data);

  }  
}


/******************************************************************************
 *
 * Private functions
 *
 *****************************************************************************/

/**
 * \brief   Range adjust the given heading so that it lies within 0-359 degrees
 * \param   me The state machine object
 * \returns Returns the range adjusted heading
 */
static int16_t range_adjust_heading(int16_t heading) {
  while (heading < 0) {
    heading += 360;
  }
  while (heading > 359) {
    heading -= 360;
  }
  return heading;
}


/**
 * \brief Parse a received ASCII message
 * \param bus_message The received message
 *
 * This function will parse a received ASCII message and execute the command
 * it contains. The command name should be a single word followed by space
 * separated command arguments. The total command length must not exceed
 * 15 characters since that is the maximum length for a bus message.
 */
static void parse_ascii_cmd(BUS_MESSAGE *bus_message) {
  char data[16];
  memcpy(data, bus_message->data, bus_message->length);
  data[bus_message->length] = '\0';

    /* Get the command and its arguments */
  unsigned char argc = 0;
  char *argv[MAX_ASCII_CMD_ARGS];
  argv[0] = NULL;
  unsigned char pos = 0;
  for (pos = 0; pos < bus_message->length; ++pos) {
    if (argv[argc] == NULL) {
      if (data[pos] != ' ') {
        argv[argc] = (char *) (data + pos);
      }
    }
    else {
      if (data[pos] == ' ') {
        data[pos] = '\0';
        if (argc >= MAX_ASCII_CMD_ARGS - 1) {
          break;
        }
        ++argc;
        argv[argc] = NULL;
      }
    }
  }
  if (argv[argc] != NULL) {
    ++argc;
  }

    /* The argc variable now contain the number of parts in the command.
     * The command name is also included in the count. The argv variable
     * contain the command and the arguments. argv[0] is the command name and
     * argv[1] is the first argument etc.
     */
  if (argc > 0) {
    if (strcmp(argv[0], "help") == 0) {
      send_help(bus_message->from_addr);
    }
    else if (strcmp(argv[0], "calon") == 0) {
      if (argc != 2) {
        send_help(bus_message->from_addr);
        return;
      }
      if (rotator_cal_on(argv[1][0]-'0') == -1) {
        send_help(bus_message->from_addr);
        return;
      }
    }
    else if (strcmp(argv[0], "caloff") == 0) {
      if (argc != 2) {
        send_help(bus_message->from_addr);
        return;
      }
      if (rotator_cal_off(argv[1][0]-'0') == -1) {
        send_help(bus_message->from_addr);
        return;
      }
    }
    else if (strcmp(argv[0], "ccwlim") == 0) {
      if (argc != 3) {
        send_help(bus_message->from_addr);
        return;
      }
      if (rotator_set_ccw_limit(argv[1][0]-'0', atoi(argv[2])) == -1) {
        send_help(bus_message->from_addr);
        return;
      }
    }
    else if (strcmp(argv[0], "cwlim") == 0) {
      if (argc != 3) {
        send_help(bus_message->from_addr);
        return;
      }
      if (rotator_set_cw_limit(argv[1][0]-'0', atoi(argv[2])) == -1) {
        send_help(bus_message->from_addr);
        return;
      }
    }
    else if (strcmp(argv[0], "dir") == 0) {
      if (argc != 2) {
        send_help(bus_message->from_addr);
        return;
      }
      uint8_t rot_idx = argv[1][0]-'0';
      int16_t dir = rotator_current_heading_raw(rot_idx);
      int16_t dir_deg = rotator_current_heading(rot_idx);
      if ((dir == -1) || (dir_deg == -1)) {
        send_help(bus_message->from_addr);
        return;
      }
      send_ascii_data(bus_message->from_addr, "%d (%d, %.1fV)\r\n",
                      dir_deg, dir, 5.0f * ((float)dir + 0x4000) / 0x7fe0);
    }
    else if (strcmp(argv[0], "defaults") == 0) {
      rotator_set_default_config();
#if 0
      send_ascii_data(bus_message->from_addr, "warmup=%d\r\n",
                      pa_warmup_timeout());
      send_ascii_data(bus_message->from_addr, "unused=%d\r\n",
                      pa_unused_timeout());
      send_ascii_data(bus_message->from_addr, "cooldown=%d\r\n",
                      pa_cooldown_timeout());
#endif
    }
    else if (strcmp(argv[0], "reset") == 0) {
      bsp_failsafe_setup();
      bsp_reset();
      send_ascii_data(bus_message->from_addr, "RESETTING\r\n");
    }
    else if (strcmp(argv[0], "status") == 0) {
      if (argc > 1) {
        if (strcmp(argv[1], "clear") == 0) {
          bsp_last_assertion_clear();
        }
        else {
          send_help(bus_message->from_addr);
          return;
        }
      }
      for (int i=0; i<ROTATOR_COUNT; ++i) {
        send_ascii_data(bus_message->from_addr, "%d: %s\r\n",
                        i, rotator_strerror(rotator_current_error(i)));
      }
      
      char last_assertion_str[LAST_ASSERTION_SIZE];
      bsp_last_assertion(last_assertion_str);
      if (last_assertion_str[0] != 0) {
        send_ascii_data(bus_message->from_addr, "\r\nASSERT[%s]\r\n",
                        last_assertion_str);
      }
    }
    else {
      send_help(bus_message->from_addr);
    }
  }
}


/*! \brief Parse a message and exectute the proper commands
 * 
 * This function is used to parse a message that was receieved on the bus that
 * is located in the RX queue.
 */
static void bus_parse_message(BUS_MESSAGE *bus_message) {
  switch (bus_message->cmd) {
    case BUS_CMD_ACK: {
      bus_message_acked(bus_message->from_addr);
      break;
    }
    case BUS_CMD_NACK: {
      bus_message_nacked(bus_message->from_addr, bus_message->data[0]);
      break;
    }
    case BUS_CMD_PING: {
      if (bus_message->length > 1) {
        bus_ping_new_stamp(bus_message->from_addr,
                           bus_message->data[0],
                           bus_message->length - 1,
                           (unsigned char *)(bus_message->data + 1));
      }
      else {
        bus_ping_new_stamp(bus_message->from_addr, bus_message->data[0], 0, 0);
      }
      break;
    }
    case BUS_CMD_ASCII_DATA: {
      parse_ascii_cmd(bus_message);
      break;
    }
    case BUS_CMD_ROTATOR_SET_ANGLE: {
      uint8_t rot_idx = bus_message->data[0];
      uint16_t angle = bus_message->data[1] << 8;
      angle |= bus_message->data[2];
      rotator_set_target_heading(rot_idx, angle);
      break;
    }
    case BUS_CMD_ROTATOR_ROTATE_CW: {
      uint8_t rot_idx = bus_message->data[0];
      rotator_rotate_cw(rot_idx);
      break;
    }
    case BUS_CMD_ROTATOR_ROTATE_CCW: {
      uint8_t rot_idx = bus_message->data[0];
      rotator_rotate_ccw(rot_idx);
      break;
    }
    case BUS_CMD_ROTATOR_STOP: {
      uint8_t rot_idx = bus_message->data[0];
      rotator_stop(rot_idx);
      break;
    }
    default: {
      /* FIXME: What should we do when receiving an unrecognized message? */
      break;
    }
  }
}


/*!
 * \brief   Read the external DIP-switch.
 * \return  The address of the external DIP-switch
 * 
 * This function is used to read the external offset address on the
 * General I/O card
 */
static unsigned char read_ext_addr(void) {
  return (~(PINE >> 2) & 0x0F);
}


/**
 * \brief Send a ASCII command help text to the given address
 */
static void send_help(uint8_t addr) {
  send_ascii_data(addr, "defaults\r\n"
                        "reset\r\n"
                        "calon <idx>\r\n");
  send_ascii_data(addr, "caloff <idx>\r\n"
                        "ccwlim <idx> <deg>\r\n");
  send_ascii_data(addr, "cwlim <idx> <deg>\r\n"
                        "dir <idx>\r\n");
  send_ascii_data(addr, "status [clear]\r\n");
}

