
#include "config.h"

/************************ Variables  *******************************/
char Sem_Estado = '0';        // Varible con el estado del semáforo
int LV = 0;                   // Varible para guardar el valor de led verde
int LR = 0;                   // Varible para guardar el valor de led rojo
int LA = 0;                   // Varible para guardar el valor de led amarillo
char Datos_girox = '0';          // Array con todos los valores
char Datos_giroy = '0';          // Array con todos los valores
int bandera = 1;
char Gy1 = '0';
char Gx1 = '0';
String Sema;

/************************ Loop delay *******************************/
#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;

/************************ Feeds a utilizar *******************************/
AdafruitIO_Feed *LrojoFeed = io.feed("LedR");
AdafruitIO_Feed *LamaFeed = io.feed("LedA");
AdafruitIO_Feed *LverdeFeed = io.feed("LedV");
AdafruitIO_Feed *Gx = io.feed("Valor sensor x");
AdafruitIO_Feed *Gy = io.feed("Valor sensor y");

/************************ Example Starts Here *******************************/
void setup() {
  
  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // start MQTT connection to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  LrojoFeed->onMessage(handleMessage);
  LamaFeed->onMessage(handleMessage);
  LverdeFeed->onMessage(handleMessage);


  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  LrojoFeed->get();
  LamaFeed->get();
  LverdeFeed->get();

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  
   while(Serial2.available()>0){
    int datos = Serial2.read();

    Serial.println(datos);
    Gx->save(datos);
    delay(3000);
  }

}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());

  Sem_Estado = *data->value();
  

  if(Sem_Estado = '1'){
    
    Serial2.print("0"); 
  }
  else if(Sem_Estado == '1'){
    Serial2.print("1");
  }
  else if(Sem_Estado == '2'){
    Serial2.print("2");
  }
  else if(Sem_Estado == '3'){
    Serial2.print("3");
  }
  

}
