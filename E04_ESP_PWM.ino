// Configure the framework
#include "bconf/MCU_ESP8266.h"              // Load the code directly on the ESP8266
#include "conf/Gateway.h"                   // The main node is the Gateway, we have just one node
#include "conf/IPBroadcast.h"

// Define the WiFi name and password
#define WIFICONF_INSKETCH
#define WiFi_SSID               "SSID"
#define WiFi_Password           "PASSWORD"    

// Include framework code and libraries
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "Souliss.h"

// This identify the number of the SLOT logic
#define LEDPWM      0     //THIS TYPICAL USES TWO SLOTS, SO THE NEXT FREE SLOT IS 2.

//PWM pin
#define LEDPWMP     5

void setup()
{   
	analogWriteFreq(500); 
    analogWriteRange(255); 

    Initialize();
    // Connect to the WiFi network and get an address from DHCP
    GetIPAddress();      
    SetAsGateway(myvNet_dhcp);
    
    // This is the vNet address for this node, used to communicate with other
	// nodes in your Souliss network
    SetAddress(0xAB01, 0xFF00, 0x0000);
    //Example of Peer Definition
    //SetAsPeerNode(0xAB02, 1);
    // This node will serve all the others in the network providing an address
    
    Set_DimmableLight(LEDPWM);
    
    pinMode(LEDPWMP, OUTPUT);
}


void loop()
{ 
    
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds
            Logic_DimmableLight(LEDPWM);                        
            analogWrite(LEDPWMP, mOutput(LEDPWM+1));
        }
        // Here we handle here the communication with Android
        FAST_GatewayComms();                                        
    }
    EXECUTESLOW() {
 UPDATESLOW();


            SLOW_10s() {  // Read temperature and humidity from DHT every 110 seconds  
                Timer_DimmableLight(LEDPWM);              
            }            


    }
}    
