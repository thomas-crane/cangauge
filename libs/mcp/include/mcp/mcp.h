#ifndef MCP_MCP_H
#define MCP_MCP_H

#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"

typedef enum {
  MCP_REG_ADDR = 0x01,
  MCP_REG_DNUM = 0x02,
  MCP_REG_BAUD = 0x03,
  MCP_REG_SEND = 0x30,
  MCP_REG_RECV = 0x40,
  MCP_REG_MASK0 = 0x60,
  MCP_REG_MASK1 = 0x65,
  MCP_REG_FILT0 = 0x70,
  MCP_REG_FILT1 = 0x80,
  MCP_REG_FILT2 = 0x90,
  MCP_REG_FILT3 = 0xA0,
  MCP_REG_FILT4 = 0xB0,
  MCP_REG_FILT5 = 0xC0,
} mcp_reg_e;

typedef enum {
  MCP_MASK_0 = 0x60,
  MCP_MASK_1 = 0x65,
} mcp_mask_e;

typedef enum {
  MCP_FILTER_0 = 0x70,
  MCP_FILTER_1 = 0x80,
  MCP_FILTER_2 = 0x90,
  MCP_FILTER_3 = 0xA0,
  MCP_FILTER_4 = 0xB0,
  MCP_FILTER_5 = 0xC0,
} mcp_filter_e;

typedef struct mcp_can_packet_t {
  uint64_t id;
  bool extended;
  bool rtr;
  uint8_t data_len;
  uint8_t data[8];
} mcp_can_packet_t;

void mcp_read_reg(mcp_reg_e reg, uint8_t *data, uint8_t len);
void mcp_write_reg(mcp_reg_e reg, uint8_t *data, uint8_t len);

uint8_t mcp_calc_checksum(uint8_t *data, uint8_t len);

void mcp_set_mask(mcp_mask_e mask, bool ext, uint64_t mask_value);
void mcp_set_filter(mcp_filter_e filter, bool ext, uint64_t filter_value);

bool mcp_read_packet(mcp_can_packet_t *packet);
void mcp_send_packet(mcp_can_packet_t *packet);
void mcp_print_packet(mcp_can_packet_t *packet);


uint8_t mcp_packet_count();

#endif // MCP_MCP_H
