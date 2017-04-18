/*---------------------------------------------------------------------------
 *
 *                             crHARPER.com
 *
 *---------------------------------------------------------------------------
 *
 * File: config.h
 * Author: crHARPER
 * $Revision: $
 * $Date: 17 Arp, 2017$
 * 
 * Description:
 * 
 *
 *
 *---------------------------------------------------------------------------
 */
#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_VERSION 1

//EEPROM structure for FG-100
typedef struct {
    uint8_t version;
	uint32_t frequency; 
	uint8_t cursor;   
	uint8_t waveform;  
    uint8_t crc;
} Config_Eeprom_Type;

void config_init(void);
void config_write(void);

#endif // CONFIG_H
