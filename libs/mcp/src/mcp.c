#include "mcp/mcp.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

static const uint8_t MCP_DEFAULT_ADDR = 0x25;

void mcp_read_reg(mcp_reg_e reg, uint8_t *data, uint8_t len) {
  // TODO: check that reg is a value of mcp_reg_e.
  uint8_t reg_value = reg;
  i2c_write_blocking(i2c_default, MCP_DEFAULT_ADDR, &reg_value, 1, true);
  int res = i2c_read_blocking(i2c_default, MCP_DEFAULT_ADDR, data, len, false);
  if (res != len) {
    printf("Wanted %i bytes, got %i\n", len, res);
  }
}

void mcp_write_reg(mcp_reg_e reg, uint8_t *data, uint8_t len) {
  // TODO: check that reg is a value of mcp_reg_e.
  uint8_t block[len + 1];
  block[0] = reg;
  for (size_t i = 0; i < len; i++) {
    block[i + 1] = data[i];
  }
  i2c_write_blocking(i2c_default, MCP_DEFAULT_ADDR, &block, len + 1, false);
}

uint8_t mcp_calc_checksum(uint8_t *data, uint8_t len) {
  uint64_t sum = 0;
  
  for (size_t i = 0; i < len; i++) {
    sum += data[i];
  }

  if (sum > 0xff) {
    sum = ~sum;
    sum += 1;
  }

  sum = sum & 0xff;
  return (uint8_t)sum;
}

bool mcp_read_packet(mcp_can_packet_t *packet) {
  // Get the raw data.
  uint8_t raw_data[16] = {0};
  mcp_read_reg(MCP_REG_RECV, raw_data, 16);

  // Validate the checksum.
  const uint64_t checksum = mcp_calc_checksum(raw_data, 15);
  if (checksum != raw_data[15]) {
    return false;
  }

  // ID is a big-endian 64-bit number.
  uint64_t packet_id =
    (raw_data[0] << 24)
    | (raw_data[1] << 16)
    | (raw_data[2] << 8)
    | raw_data[3];

  bool extended = raw_data[4];
  bool rtr = raw_data[5];

  packet->id = packet_id;
  packet->extended = raw_data[4];
  packet->rtr = raw_data[5];
  packet->data_len = raw_data[6];
  
  for (size_t i = 0; i < packet->data_len; i++) {
    packet->data[i] = raw_data[7 + i];
  }

  return true;
}

uint8_t mcp_packet_count() {
    uint8_t data = 0;
    mcp_read_reg(MCP_REG_DNUM, &data, 1);
    return data;
}

void mcp_send_packet(mcp_can_packet_t *packet) {
  uint8_t send_buf[16] = {0};

  // Big-endian copy ID into buffer.
  send_buf[0] = 0xff & (packet->id >> 24);
  send_buf[1] = 0xff & (packet->id >> 16);
  send_buf[2] = 0xff & (packet->id >> 8);
  send_buf[3] = 0xff & packet->id;

  send_buf[4] = packet->extended;
  send_buf[5] = packet->rtr;

  send_buf[6] = packet->data_len;

  // Copy data into send buffer.
  for (size_t i = 0; i < packet->data_len; i++) {
    send_buf[7 + i] = packet->data[i];
  }

  // Checksum.
  send_buf[15] = mcp_calc_checksum(send_buf, 15);

  mcp_write_reg(MCP_REG_SEND, send_buf, 16);
}

void mcp_print_packet(mcp_can_packet_t *packet) {
  printf(
    "Packet(id=%4lld, extended=%i, rtr=%i, data_len=%hhu, data=[",
    packet->id,
    packet->extended,
    packet->rtr,
    packet->data_len
  );

  for (size_t i = 0; i < packet->data_len; i++) {
    printf(" %3hhu", packet->data[i]);
  }
  printf(" ])\n");
}

void mcp_set_mask(mcp_mask_e mask, bool ext, uint64_t mask_value) {
  switch (mask) {
    case MCP_MASK_0:
    case MCP_MASK_1:
      break;
    default:
      printf("Invalid mask.\n");
      return;
  }

  uint8_t send_buf[5] = {0};

  send_buf[0] = ext;
  send_buf[1] = 0xff & (mask_value >> 24);
  send_buf[2] = 0xff & (mask_value >> 16);
  send_buf[3] = 0xff & (mask_value >> 8);
  send_buf[4] = 0xff & mask_value;

  mcp_write_reg(mask, send_buf, 5);
}

void mcp_set_filter(mcp_filter_e filter, bool ext, uint64_t filter_value) {
  switch (filter) {
    case MCP_FILTER_0:
    case MCP_FILTER_1:
    case MCP_FILTER_2:
    case MCP_FILTER_3:
    case MCP_FILTER_4:
    case MCP_FILTER_5:
      break;
    default:
      printf("Invalid filter.\n");
      return;
  }

  uint8_t send_buf[5] = {0};

  send_buf[0] = ext;
  send_buf[1] = 0xff & (filter_value >> 24);
  send_buf[2] = 0xff & (filter_value >> 16);
  send_buf[3] = 0xff & (filter_value >> 8);
  send_buf[4] = 0xff & filter_value;

  mcp_write_reg(filter, send_buf, 5);
}
