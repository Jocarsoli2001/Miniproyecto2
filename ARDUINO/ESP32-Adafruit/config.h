/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "Jocarsoli_so_2001"
#define IO_KEY       "aio_YUem4445UYyg22Iy61yKsNlVJRV7"



#define WIFI_SSID "Iphone de Jocarsoli"
#define WIFI_PASS "Jocarsoli_so_2001"
#include "AdafruitIO_WiFi.h"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
