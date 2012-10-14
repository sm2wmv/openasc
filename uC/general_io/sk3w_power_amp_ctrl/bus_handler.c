/*! \file     general_io/sk3w_power_amp_ctrl/bus_handler.c
 *  \ingroup  general_io_group
 *  \brief    Handle bus communications for this application
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-05-23
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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/wdt.h>

#include <delay.h>

/* Include the bus headers */
#include <wmv_bus/bus.h>
#include <wmv_bus/bus_ping.h>
#include <wmv_bus/bus_commands.h>
#include <queue.h>
#include <global.h>

#include "pa.h"
#include "bsp.h"
#include "controller.h"
#include "bus_handler.h"
#include "version.h"


#define MAX_ASCII_CMD_ARGS      5
#define AMP_STATUS_INTERVAL     5000/6  /* ~6 bands per five second */


//! Counter to keep track of the time elapsed since the last sync message was sent
static uint16_t counter_sync = 0;
//! Counter to keep track of when to send a ping out on the bus
static uint16_t counter_ping_interval = 0;
//! Counter to keep track of when ot send amplifier status updates
static uint16_t counter_send_amp_status_interval = 0;

static uint8_t cur_tx_active = 0;
static uint8_t next_amp_status_band = 0;

static const uint8_t amp_status_band[] = {
  BAND_160M, BAND_80M, BAND_40M, BAND_20M, BAND_15M, BAND_10M
};

static uint8_t decode_ctrlr(char ctrlr);
static void parse_ascii_cmd(BUS_MESSAGE *bus_message);
static void bus_parse_message(BUS_MESSAGE *bus_message);
static unsigned char read_ext_addr(void);
static void send_pa_status(uint8_t band);
static void send_help(uint8_t addr);
static int8_t wl2band(uint8_t wl);


void bus_handler_init() {
    /* Read the external address of the device */
  bus_set_address(BUS_BASE_ADDR + read_ext_addr());

    /* This delay is simply so that if you have the devices connected to the same power supply
     * all units should not send their status messages at the same time. Therefor we insert a delay
     * that is based on the external address of the device */
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
  ++counter_send_amp_status_interval;

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

  uint8_t tx_active = bsp_get_tx_active();
  if (tx_active != cur_tx_active) {
    uint8_t diff = tx_active ^ cur_tx_active;
    cur_tx_active = tx_active;
    for (int i = 0; i < 6; ++i) {
      if (diff & 0x01) {
        controller_set_tx_active(i, !(tx_active & 0x01));
      }
      diff >>= 1;
      tx_active >>= 1;
    }
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

    if (counter_send_amp_status_interval >= AMP_STATUS_INTERVAL) {
      uint8_t band = amp_status_band[next_amp_status_band++];
      if (next_amp_status_band ==
          (sizeof(amp_status_band) / sizeof(*amp_status_band))) {
        next_amp_status_band = 0;
      }
      send_pa_status(band);
      counter_send_amp_status_interval = 0;
    }
  }

  const Controller *ctrlrs;
  uint8_t ctrlr_cnt = controller_band_list(&ctrlrs);
  for (int i = 0; i < ctrlr_cnt; ++i) {
    if ((ctrlrs[i].addr != 0) && !bus_ping_device_is_present(ctrlrs[i].addr)) {
      controller_set_band(i, 0, BAND_UNDEFINED);
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


static uint8_t decode_ctrlr(char ctrlr) {
  if ((ctrlr >= '0') && (ctrlr <= '5')) {
    ctrlr -= '0';
  }
  else if ((ctrlr >= 'A') && (ctrlr <= 'F')) {
    ctrlr -= 'A';
  }
  else if ((ctrlr >= 'a') && (ctrlr <= 'f')) {
    ctrlr -= 'a';
  }
  else {
    return 0xff;
  }
  return (uint8_t)ctrlr;
}


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

  if (argc > 0) {
    if (strcmp(argv[0], "help") == 0) {
      send_help(bus_message->from_addr);
    }
    else if (strcmp(argv[0], "ver") == 0) {
      send_ascii_data(bus_message->from_addr, "Ver: " VERSION "\r\n");
    }
    else if (strcmp(argv[0], "ptton") == 0) {
      if (argc != 2) {
        send_help(bus_message->from_addr);
        return;
      }
      uint8_t ctrlr = decode_ctrlr(argv[1][0]);
      if (ctrlr == 0xff) {
        send_help(bus_message->from_addr);
        return;
      }
      controller_set_tx_active(ctrlr, 1);
      send_ascii_data(bus_message->from_addr, "OK\r\n");
    }
    else if (strcmp(argv[0], "pttoff") == 0) {
      if (argc != 2) {
        send_help(bus_message->from_addr);
        return;
      }
      uint8_t ctrlr = decode_ctrlr(argv[1][0]);
      if (ctrlr == 0xff) {
        send_help(bus_message->from_addr);
        return;
      }
      controller_set_tx_active(ctrlr, 0);
      send_ascii_data(bus_message->from_addr, "OK\r\n");
    }
    else if (strcmp(argv[0], "togglemains") == 0) {
      if (argc != 2) {
        send_help(bus_message->from_addr);
        return;
      }
      uint8_t ctrlr = decode_ctrlr(argv[1][0]);
      if (ctrlr == 0xff) {
        send_help(bus_message->from_addr);
        return;
      }
      controller_toggle_mains(ctrlr);
      send_ascii_data(bus_message->from_addr, "OK\r\n");
    }
    else if (strcmp(argv[0], "mainsoff") == 0) {
      int8_t ret = 0;
      if (argc == 2) {
        int8_t band = wl2band(atoi(argv[1]));
        if (band == -1) {
          send_ascii_data(bus_message->from_addr, "*** Invalid band\r\n");
          send_help(bus_message->from_addr);
          return;
        }
        ret = pa_mains_off(band);
      }
      else if (argc == 1) {
        ret = pa_mains_all_off();
      }
      else {
        send_help(bus_message->from_addr);
        return;
      }
      if (ret == -1) {
        send_ascii_data(bus_message->from_addr, "*** Error\r\n");
        return;
      }
      send_ascii_data(bus_message->from_addr, "OK\r\n");
    }
    else if (strcmp(argv[0], "mainson") == 0) {
      int8_t ret = 0;
      if (argc == 2) {
        int8_t band = wl2band(atoi(argv[1]));
        if (band == -1) {
          send_ascii_data(bus_message->from_addr, "*** Invalid band\r\n");
          send_help(bus_message->from_addr);
          return;
        }
        ret = pa_mains_on(band);
      }
      else if (argc == 1) {
        ret = pa_mains_all_on();
      }
      else {
        send_help(bus_message->from_addr);
        return;
      }
      if (ret == -1) {
        send_ascii_data(bus_message->from_addr, "*** Error\r\n");
        return;
      }
      send_ascii_data(bus_message->from_addr, "OK\r\n");
    }
    else if (strcmp(argv[0], "warmup") == 0) {
      if (argc == 2) {
        pa_set_warmup_timeout(atoi(argv[1]));
      }
      else if (argc > 2) {
        send_help(bus_message->from_addr);
        return;
      }
      send_ascii_data(bus_message->from_addr, "warmup=%d\r\n",
                      pa_warmup_timeout());
    }
    else if (strcmp(argv[0], "unused") == 0) {
      if (argc == 2) {
        pa_set_unused_timeout(atoi(argv[1]));
      }
      else if (argc > 2) {
        send_help(bus_message->from_addr);
        return;
      }
      send_ascii_data(bus_message->from_addr, "unused=%d\r\n",
                      pa_unused_timeout());
    }
    else if (strcmp(argv[0], "cooldown") == 0) {
      if (argc == 2) {
        pa_set_cooldown_timeout(atoi(argv[1]));
      }
      else if (argc > 2) {
        send_help(bus_message->from_addr);
        return;
      }
      send_ascii_data(bus_message->from_addr, "cooldown=%d\r\n",
                      pa_cooldown_timeout());
    }
    else if (strcmp(argv[0], "defaults") == 0) {
      pa_set_default_config();
      send_ascii_data(bus_message->from_addr, "warmup=%d\r\n",
                      pa_warmup_timeout());
      send_ascii_data(bus_message->from_addr, "unused=%d\r\n",
                      pa_unused_timeout());
      send_ascii_data(bus_message->from_addr, "cooldown=%d\r\n",
                      pa_cooldown_timeout());
    }
    else if (strcmp(argv[0], "reset") == 0) {
        /* Enable the watchdog timer and wait for it to reset
         * in about one second */
      wdt_enable(WDTO_1S);
      send_ascii_data(bus_message->from_addr, "RESETTING\r\n");
    }
    else {
      send_help(bus_message->from_addr);
    }
  }
}


/*! \brief Parse a message and exectute the proper commands
 * This function is used to parse a message that was receieved on the bus that is located
 * in the RX queue. */
static void bus_parse_message(BUS_MESSAGE *bus_message) {
  switch (bus_message->cmd) {
    case BUS_CMD_ACK:{
      bus_message_acked(bus_message->from_addr);
      break;
    }
    case BUS_CMD_NACK:{
      bus_message_nacked(bus_message->from_addr, bus_message->data[0]);
      break;
    }
    case BUS_CMD_PING:{
      if (bus_message->length > 1) {
        bus_ping_new_stamp(bus_message->from_addr,
                           bus_message->data[0],
                           bus_message->length - 1,
                           (unsigned char *)(bus_message->data + 1));
      }
      else {
        bus_ping_new_stamp(bus_message->from_addr, bus_message->data[0], 0,
                           0);
      }
      break;
    }
    case BUS_CMD_ASCII_DATA:{
      parse_ascii_cmd(bus_message);
      break;
    }
    case BUS_CMD_AMPLIFIER_TOGGLE_MAINS_STATUS:{
      unsigned char subaddr = bus_message->data[0];
      if (controller_toggle_mains(subaddr) == -1) {
        uint8_t msg[] = {
          subaddr,
          controller_band(subaddr)
        };
        bus_add_tx_message(bus_get_address(),
                           BUS_BROADCAST_ADDR,
                           0, BUS_CMD_AMPLIFIER_ERROR, sizeof(msg), msg);
      }
      break;
    }
    case BUS_CMD_AMPLIFIER_BAND_CHANGE:{
      unsigned char subaddr = bus_message->data[0];
      unsigned char current_band = bus_message->data[1];
      /* unsigned char current_segment = bus_message->data[2]; */
      if (controller_set_band(subaddr, bus_message->from_addr,
                              current_band) == -1) {
        uint8_t msg[] = {
          subaddr,
          controller_band(subaddr)
        };
        bus_add_tx_message(bus_get_address(),
                           BUS_BROADCAST_ADDR,
                           0, BUS_CMD_AMPLIFIER_ERROR, sizeof(msg), msg);
      }
      break;
    }
    case BUS_CMD_AMPLIFIER_RESET:{
      /* FIXME: Implement when alarms have been implemented */
      break;
    }
    case BUS_CMD_AMPLIFIER_TUNE:
    case BUS_CMD_AMPLIFIER_TOGGLE_OPERATE_STBY_STATUS:{
      /* Not used by this PA control logic */
      break;
    }
    default:{
      /* FIXME: What should we do when receiving an unrecognized message? */
      break;
    }
  }
}


/*!
 * \brief   Read the external DIP-switch.
 * \return  The address of the external DIP-switch
 * 
 * This function is used to read the external offset address on the General I/O card
 */
static unsigned char read_ext_addr(void) {
  return (~(PINE >> 2) & 0x0F);
}


static void send_pa_status(uint8_t band) {
  uint8_t op_status = pa_op_status(band);
  uint8_t msg[5];
  msg[0] = pa_controller(band);
  msg[1] = ((op_status != AMP_OP_STATUS_OFF) << AMP_STATUS_MAINS)
         | ((op_status == AMP_OP_STATUS_READY) << AMP_STATUS_OPR_STBY);
  msg[2] = op_status;
  msg[3] = band;
  msg[4] = 0;
  bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0,
                     BUS_CMD_AMPLIFIER_GET_STATUS, 5, msg);
}


static void send_help(uint8_t addr) {
  send_ascii_data(addr, "ptton <ctrlr>\r\n"
                        "pttoff <ctrlr>\r\n");
  send_ascii_data(addr, "mainson [band]\r\n"
                        "mainsoff [band]\r\n");
  send_ascii_data(addr, "togglemains <ctrlr>\r\n"
                        "warmup [tmo sec]\r\n");
  send_ascii_data(addr, "unused [tmo sec]\r\n"
                        "cooldown [tmo sec]\r\n");
  send_ascii_data(addr, "defaults\r\n"
                        "reset\r\n"
                        "ver\r\n");
}


/*!
 * \brief Convert wavelength to a band enum
 * \param wl The wavelength as and integer (160, 80, 40, 20, 15 or 10)
 * \returns Return the corresponding band enum or -1 or error
 */
static int8_t wl2band(uint8_t wl)
{
  switch (wl) {
    case 160:
      return BAND_160M;
    case 80:
      return BAND_80M;
    case 40:
      return BAND_40M;
    case 20:
      return BAND_20M;
    case 15:
      return BAND_15M;
    case 10:
      return BAND_10M;
    default:
      return -1;
  }
}


/*!
 * \brief Called by the PA state machine when the op status changes
 * \param band The affected band
 */
void pa_op_status_changed(uint8_t band) {
  if (bus_allowed_to_send()) {
    send_pa_status(band);
  }
}

/*!
 * \brief Called by the PA state machine when the controller changes
 * \param band The affected band
 */
void pa_ctrlr_changed(uint8_t band) {
  if (bus_allowed_to_send()) {
    send_pa_status(band);
  }
}
