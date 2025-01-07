//
// Created by wowa on 08.01.19.
//

#include <string.h>

#include <argtable3/argtable3.h>
#include <esp_console.h>
#include <esp_log.h>

#include "PN532.h"
#include "nfc_console.h"

static const char* TAG = "nfc_console";

/*
 * 'nfc' command to connect with wifi network
 */

/*
 * Arguments used by 'nfc_execute' function
 */
static struct
{
  struct arg_str* read_write;
  struct arg_int* page;
  struct arg_str* value;
  struct arg_end* end;
} nfc_args;

static int nfc_execute(int argc, char** argv)
{
  int nerrors = arg_parse(argc, argv, (void**)&nfc_args);
  if (nerrors != 0)
  {
    arg_print_errors(stderr, nfc_args.end, argv[0]);
    return 1;
  }
  uint8_t uid[7];
  uint8_t uidLength;

  // create a struct for the wifi login and copy the input parameters into it
  char read_write = *nfc_args.read_write->sval[0];
  uint8_t page = (uint8_t)nfc_args.page->ival[0];
  uint32_t value = strtoul(nfc_args.value->sval[0], NULL, 16);

  uint8_t nfc_buffer[4];

  if (read_write == 'r')
  {
    ESP_LOGD(TAG, "reading page %d\r\n", page);
    if (readPassiveTargetID(0, uid, &uidLength, 30000))
    {
      ESP_LOGD(TAG, "UID = %02x %02x %02x %02x %02x %02x %02x\r\n", uid[0], uid[1], uid[2], uid[3], uid[4], uid[5], uid[6]);
      mifareultralight_ReadPage(page, nfc_buffer);
      ESP_LOGD(TAG, "page %d = %02x%02x%02x%02x\r\n", page, nfc_buffer[3], nfc_buffer[2], nfc_buffer[1], nfc_buffer[0]);
    }
    else { ESP_LOGD(TAG, " FAILED TO READ\r\n"); }
  }
  else if (read_write == 'w')
  {
    ESP_LOGD(TAG, "writing page %d value %08lu \r\n", page, value);
    if (readPassiveTargetID(0, uid, &uidLength, 30000))
    {
      ESP_LOGD(TAG, "UID = %02x %02x %02x %02x %02x %02x %02x\r\n", uid[0], uid[1], uid[2], uid[3], uid[4], uid[5], uid[6]);

      nfc_buffer[0] = (value >> 24) & 0xFF;
      nfc_buffer[1] = (value >> 16) & 0xFF;
      nfc_buffer[2] = (value >> 8) & 0xFF;
      nfc_buffer[3] = value & 0xFF;
      mifareultralight_WritePage(page, nfc_buffer);
    }
    else { ESP_LOGD(TAG, " FAILED TO WRITE\r\n"); }
  }

  return 0;
}

void register_nfc()
{
  nfc_args.read_write = arg_str0(NULL, NULL, "<r/w>", "r = read, w = write");
  nfc_args.page = arg_int0(NULL, NULL, "<page>", "page number");
  nfc_args.value = arg_str0(NULL, NULL, "<value>", "value");
  nfc_args.end = arg_end(2);

  const esp_console_cmd_t nfc_cmd = {
      .command = "nfc", .help = "read or write a specific page of the NFC tag", .hint = NULL, .func = &nfc_execute, .argtable = &nfc_args};

  ESP_ERROR_CHECK_WITHOUT_ABORT(esp_console_cmd_register(&nfc_cmd));
}
