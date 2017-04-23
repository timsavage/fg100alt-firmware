/*---------------------------------------------------------------------------
 *
 *                             crHARPER.com
 *
 *---------------------------------------------------------------------------
 *
 * File: config.c
 * Author: crHARPER
 * $Revision: $
 * $Date: 17 Apr, 2017$
 *
 * Description:
 * For storing/retreaving configurables to EEPROM
 *
 *
 *---------------------------------------------------------------------------
 */
#include <avr/eeprom.h>

#include "config.h"
#include "dds.h"

Config_Eeprom_Type ui_state;
void config_once(void);

extern uint8_t  crc8( uint8_t *, uint8_t );

/*---------------------------------------------------------------------------
 * read the EEPROM
 *---------------------------------------------------------------------------
 */
uint8_t config_read(void){
    uint8_t crc = 0;

    for (int i=0; i < sizeof(Config_Eeprom_Type); ++i) {
         ((uint8_t*) &ui_state)[i] = eeprom_read_byte((uint8_t*) i);
    }

    crc = crc8((uint8_t*) &ui_state, sizeof(Config_Eeprom_Type)-1);

    if (crc == ui_state.crc) {
        return 1;   //pass
    }

    return 0;       //fail
}


/*---------------------------------------------------------------------------
 * write to EEPROM
 *---------------------------------------------------------------------------
 */
void config_write(void){
    ui_state.version = CONFIG_VERSION;

    ui_state.crc = crc8((uint8_t*) &ui_state, sizeof(Config_Eeprom_Type)-1);

    for (int i=0; i < sizeof(Config_Eeprom_Type); ++i) {
        eeprom_write_byte((uint8_t*) i, ((uint8_t*) &ui_state)[i]);
    }
}


/*---------------------------------------------------------------------------
 * upon power up read EEPROM
 *---------------------------------------------------------------------------
 */
void config_init(void){
    if(!config_read()) {
        config_once(); //fault, initialize EEPROM
    }
}


/*---------------------------------------------------------------------------
 * upon first time power up with a blank EEPROM
 *
 * load with default values
 * occures any time the crc test fails
 *---------------------------------------------------------------------------
 */
void config_once(void){
    ui_state.version = CONFIG_VERSION;
    ui_state.frequency = 1000;
    ui_state.cursor = 3;
    ui_state.waveform = DDS_SINE_WAVE;

    config_write();
}
