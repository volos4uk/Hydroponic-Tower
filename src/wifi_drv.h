#ifndef _WIFI_DRV_H_
#define _WIFI_DRV_H_

#include "main.h"

bool wifi_init(void);
void wifi_poll(void);

// Function to check internet availability from other modules
bool wifi_is_connected(void); 

#endif // _WIFI_DRV_H_