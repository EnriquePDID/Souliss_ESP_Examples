#ifndef SetupAndLoop_h
#define SetupAndLoop_h

// ***************************************************************************
// *                         setupGeneral FUNCTION                           *
// ***************************************************************************
void setupGeneral(){
  
    EEPROM_CONFIG();
    SLOT_CONFIG();
    PINS_CONFIG();
    analogWriteFreq(250);
    analogWriteRange(255);
	  //if(usartbridge) 
	  //SetAddress(0xCE01, 0xFF00, 0x0000);
 
//**************************** SENSORS INITIALIZE *****************************
    if(DHT_SENSOR){
        if(!dht_type)
          dht11.begin();
        else
          dht22.begin();
    }
    
    if(DALLAS_SENSOR){
        sensors.begin(); //Se inician los sensores DS18B20
    }
    
//**************************** SETUP SLOTS *****************************
    if(DHT_SENSOR){
        Set_Temperature(TEMPERATURE);
        Set_Humidity(HUMIDITY);
    }
    if(ONOFF_MODE){
        Set_SimpleLight(LEDPWM0);
        Set_SimpleLight(LEDPWM1);
        Set_SimpleLight(LEDPWM2);
    }
    if(PULSE_MODE){
        Set_PulseOutput(LEDPWM0);
        Set_PulseOutput(LEDPWM1);
        Set_PulseOutput(LEDPWM2);
    }
    if(PWM_MODE || PIR_MODE){
        Set_DimmableLight(LEDPWM0);
        Set_DimmableLight(LEDPWM1);  
    }
    
    if(PWM_MODE){
        //pinMode(0, INPUT);
        Set_DimmableLight(LEDPWM2);
    }
    
    if(PIR_MODE){
        Set_AutoLight(LED);
    }
    if(ALARM_MODE || ALARM_ENDSTOP){
        Set_Antitheft_Main(ALARM);
    }
    
    if(RGB_MODE){
        Set_LED_Strip(LEDRGB);
    }        
    if(THERMOSTAT_MODE){
        Set_Thermostat(THERMOSTAT);
    
    }
    if(RELAY){
        Set_SimpleLight(RELAY0);
        Set_SimpleLight(RELAY1);
    }
    
    if(LDR_SENSOR){
        Set_T54(LDR);
    }
    
    if(DALLAS_SENSOR){
    	for(int i=0; i < dallas_qty; i++){
        	Set_Temperature(DALLAS + (i*2));
    	}	
    }        

    if(BMP180){
        //Set_T51(PRESSURE0);
        Set_Pressure(PRESSURE0);
        Set_Temperature(BMP180TEMP);
    }
    
    if(CAPACITIVE && DEBUG_CAPSENSE){
        Set_T51(CAP0);
        Set_T51(CAP1);
        Set_T51(THRE);
    }
    if(PULSE_OUTPUT){
        Set_PulseOutput(PULSE0);
        Set_PulseOutput(PULSE1);  
    }
    if(GARAGE_DOOR){
        Set_GarageDoor(T2X);
    }
    if(WINDOW_CURTAIN){
        Set_Windows(T2X);
    }
    if(OPTO_AND_RELAY){
        Set_SimpleLight(OPTO);
    }
    if(RELAY || PULSE_OUTPUT || GARAGE_DOOR || WINDOW_CURTAIN){
        digitalWrite(RELAY0P,LOW);
        digitalWrite(RELAY1P,LOW);
        pinMode(RELAY0P, OUTPUT);                 
        pinMode(RELAY1P, OUTPUT);
    }
     if(ONOFF_MODE || PULSE_MODE){
        pinMode(LEDPWMP0, OUTPUT);
        pinMode(LEDPWMP1, OUTPUT);        
        pinMode(LEDPWMP2, OUTPUT);        
    }
    if(PWM_MODE || PIR_MODE){
        pinMode(LEDPWMP0, OUTPUT);
        pinMode(LEDPWMP1, OUTPUT);        
    }
    
    if(PWM_MODE){
        pinMode(LEDPWMP2, OUTPUT);
    }
        
    if(PIR_MODE){
        pinMode(PIRP, INPUT);
        pinMode(LEDP, OUTPUT);
        digitalWrite(LEDP,LOW);
        
        //AutoMode Enabled at Start
        SetInput(LED, Souliss_T1n_AutoCmd);
        Logic_AutoLight(LED);
        nDigOut(LEDP, Souliss_T1n_Coil, LED);
    }
    
    if(RGB_MODE){
        pinMode(LEDRP, OUTPUT);
        pinMode(LEDGP, OUTPUT);
        pinMode(LEDBP, OUTPUT);
    }
    if(THERMOSTAT_MODE){
        pinMode(THERM_HEATERP, OUTPUT);
        pinMode(THERM_FAN1P, OUTPUT);
        pinMode(THERM_FAN2P, OUTPUT);  
        pinMode(THERM_FAN3P, OUTPUT);          
    }
    if(BUTTONS || BUTTONS_2_STATE){
        pinMode(BUT0P, INPUT);
        pinMode(BUT1P, INPUT);
    }
    if(BUTTONS_PULLUP || ALARM_ENDSTOP){
        pinMode(BUT0P, INPUT_PULLUP);
        pinMode(BUT1P, INPUT_PULLUP);
    }
    if(OPTO_AND_RELAY){
        pinMode(BUT0P, INPUT);
        digitalWrite(RELAY1P,LOW);                 
        pinMode(RELAY1P, OUTPUT);
    }
    
    if(BMP180){
        //Wire.begin(SDAP,SCLP);
        delay(200);
        if (pressure.begin())
            if(DEBUG_PRESSURE) LOG.print(F("BMP180 init success\r\n"));
        else
        {
            // Oops, something went wrong, this is usually a connection problem,
            // see the comments at the top of this sketch for the proper connections.
            if(DEBUG_PRESSURE) LOG.print(F("BMP180 init fail\r\n"));
        }
    }
    
    button0 = true;
    //pinMode(0, INPUT);
    //RoutingTable(0xAB04, 0xCE00, 0);

}

void fastGeneral(){
    yield();
      
    //if (!button0) {
      //	if(millis()>60000) {
      		//button0 = true;
          //pinMode(0, INPUT);
          //LOG.println(F("Button GPIO ENABLED"));
    	//  }
	  //}
      
      FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds

            if(RELAY){
                Logic_SimpleLight(RELAY0);
                DigOut(RELAY0P, Souliss_T1n_Coil,RELAY0);
            
                Logic_SimpleLight(RELAY1);
                DigOut(RELAY1P, Souliss_T1n_Coil,RELAY1);
                
            }
            if(PWM_MODE || PIR_MODE || ONOFF_MODE || PIR_MODE){
                if(button0 && !OPTO_AND_RELAY) LowDigIn(0, Souliss_T1n_ToggleCmd, LEDPWM2);
                if(BUTTONS){
                    DigIn(BUT0P, Souliss_T1n_ToggleCmd, LEDPWM0);
                    DigIn(BUT1P, Souliss_T1n_ToggleCmd, LEDPWM1);
                }
                if(BUTTONS_PULLUP){
                    LowDigIn(BUT0P, Souliss_T1n_ToggleCmd, LEDPWM0);
                    LowDigIn(BUT1P, Souliss_T1n_ToggleCmd, LEDPWM1);
                }
                if(BUTTONS_2_STATE){
                    DigIn2State(BUT0P, Souliss_T1n_OnCmd, Souliss_T1n_OffCmd, LEDPWM0);
                    DigIn2State(BUT1P, Souliss_T1n_OnCmd, Souliss_T1n_OffCmd, LEDPWM1);
                }
            }
			      if(ONOFF_MODE){
                if(IR_ENABLE){
                   irButtons(ir_ONOFF);
                }
                
                Logic_SimpleLight(LEDPWM0);
                Logic_SimpleLight(LEDPWM1);
                Logic_SimpleLight(LEDPWM2);
                
                DigOut(LEDPWMP0, Souliss_T1n_Coil,LEDPWM0);
                DigOut(LEDPWMP1, Souliss_T1n_Coil,LEDPWM1);
                DigOut(LEDPWMP2, Souliss_T1n_Coil,LEDPWM2);
            }

            if(PWM_MODE || PIR_MODE){
                if(CAPACITIVE){
                    Souliss_CapSense(LEDPWM0,Souliss_T1n_ToggleCmd,Souliss_T1n_BrightSwitch, CAP0P, cap_thresold, 1500);
                    Souliss_CapSense(LEDPWM1,Souliss_T1n_ToggleCmd,Souliss_T1n_BrightSwitch, CAP1P, cap_thresold, 1500);
                    if(DEBUG_CAPSENSE_ALL) LOG.println("");
                }  
                
                Logic_DimmableLight(LEDPWM0);
                analogWrite(LEDPWMP0, mOutput(LEDPWM0+1));                
                
                Logic_DimmableLight(LEDPWM1); 
                analogWrite(LEDPWMP1, mOutput(LEDPWM1+1));
            }
            
            if( ALARM_ENDSTOP && mOutput(ALARM) == Souliss_T4n_Antitheft ){   //  IF Alarm is Armed
                LowDigIn(BUT0P, Souliss_T4n_Alarm, ALARM);                  //  Handle the Endstop to the Antitheft  
                LowDigIn(BUT1P, Souliss_T4n_Alarm, ALARM);                  //  Handle the Endstop to the Antitheft
            }
            
            if(PIR_MODE){
                if( ALARM_MODE && mOutput(ALARM) == Souliss_T4n_Antitheft ){   //  IF Alarm is Armed
                   DigIn(PIRP,Souliss_T4n_Alarm,ALARM);                        //  Handle the PIR to the Antitheft               
                }else {                                                        //  IF Alarm isnt Armed
                   DigIn(PIRP,LIGHT_ON_CYCLE+1,LED);                           //  Handle the PIR to the Autolight    
                }
                
                if(ALARM_MODE || ALARM_ENDSTOP) {
                  Logic_Antitheft_Main(ALARM);
                }
               
                Logic_AutoLight(LED);
                
                nDigOut(LEDP, Souliss_T1n_Coil,LED);
            }
            
            if(PWM_MODE){

                if(IR_ENABLE){
                  irButtons(ir_PWM);
                }
                Logic_DimmableLight(LEDPWM2);                        
                analogWrite(LEDPWMP2, mOutput(LEDPWM2+1));
            }
            
            if(RGB_MODE){
                if(button0 && !OPTO_AND_RELAY) LowDigIn(0, Souliss_T1n_ToggleCmd, LEDRGB);
                if(CAPACITIVE){
                    Souliss_CapSense(LEDRGB,Souliss_T1n_ToggleCmd,Souliss_T1n_BrightSwitch,CAP0P, cap_thresold, 1500);
                    Souliss_CapSense(LEDRGB,Souliss_T1n_ToggleCmd,Souliss_T1n_BrightSwitch,CAP1P, cap_thresold, 1500);
                } 
                if(BUTTONS){
                    DigIn(BUT0P, Souliss_T1n_ToggleCmd, LEDRGB);
                    DigIn(BUT1P, Souliss_T1n_BrightSwitch, LEDRGB);
                }
                if(BUTTONS_PULLUP){
                    LowDigIn(BUT0P, Souliss_T1n_ToggleCmd, LEDRGB);
                    LowDigIn(BUT1P, Souliss_T1n_BrightSwitch, LEDRGB);
                }
                if(IR_ENABLE){
                    irButtons(ir_RGB);
                }
                Logic_LED_Strip(LEDRGB);

                analogWrite(LEDRP, mOutput(LEDRGB+1));
                analogWrite(LEDGP, mOutput(LEDRGB+2));
                analogWrite(LEDBP, mOutput(LEDRGB+3));
            }
            if(THERMOSTAT_MODE){
                Logic_Thermostat(THERMOSTAT);
            
                // Start the heater and the fans
                nDigOut(THERM_HEATERP, Souliss_T3n_HeatingOn, THERMOSTAT);   // Heater
                nDigOut(THERM_FAN1P, Souliss_T3n_FanOn1   , THERMOSTAT);   // Fan1
                nDigOut(THERM_FAN2P, Souliss_T3n_FanOn2   , THERMOSTAT);   // Fan2
                nDigOut(THERM_FAN3P, Souliss_T3n_FanOn3   , THERMOSTAT);   // Fan3
                
                
            }
            if(GARAGE_DOOR){
                //*** ENDSTOPS MISSING ***
                Logic_GarageDoor(T2X);
                DigOut(RELAY0P, Souliss_T2n_Coil_Open,  T2X);    
                DigOut(RELAY1P, Souliss_T2n_Coil_Close, T2X);                
            }
            if(WINDOW_CURTAIN){
                Logic_Windows(T2X);
                DigOut(RELAY0P, Souliss_T2n_Coil_Open,  T2X);    
                DigOut(RELAY1P, Souliss_T2n_Coil_Close, T2X);
            }
            if(OPTO_AND_RELAY){
                //CODE MISSING get from here:
                //https://github.com/juanpintom/Souliss_Examples/blob/master/E00_Souliss_Wall_Switch_with_PLC_Reading.ino
                SoulissPLC_Read(OPTO, 0, BUT0P, RELAY1P);
                Logic_SimpleLight(OPTO);  
            }
                
        }
        
        FAST_110ms(){
                    
          if(CAPACITIVE && DEBUG_CAPSENSE){
            float temp;
            temp = readCapacitivePin(CAP0P);
            if(temp>0) Souliss_ImportAnalog(memory_map, CAP0, &temp);       
            temp = readCapacitivePin(CAP1P);
            if(temp>0) Souliss_ImportAnalog(memory_map, CAP1, &temp);       
            temp = cap_thresold;
            Souliss_ImportAnalog(memory_map, THRE, &temp);       
          }          
        }

        FAST_510ms(){
           if(CAPACITIVE && DEBUG_CAPSENSE){           
              Read_T51(CAP0);
              Read_T51(CAP1);
              Read_T51(THRE);
           }
        }
        FAST_x10ms(PULSE_TIMER/10){
            if(PULSE_MODE){
                Logic_PulseOutput(LEDPWM0);
                Logic_PulseOutput(LEDPWM1);
                Logic_PulseOutput(LEDPWM2);
                
                DigOut(LEDPWMP0, Souliss_T1n_Coil,LEDPWM0);
                DigOut(LEDPWMP1, Souliss_T1n_Coil,LEDPWM1);
                DigOut(LEDPWMP2, Souliss_T1n_Coil,LEDPWM2);                          
            }
            if(PULSE_OUTPUT){
                Logic_PulseOutput(PULSE0);
                Logic_PulseOutput(PULSE1);
                
                DigOut(RELAY0P, Souliss_T1n_Coil,PULSE0);
                DigOut(RELAY1P, Souliss_T1n_Coil,PULSE1);     
            }
        }
        FAST_9110ms()    {
            if(DALLAS){ 
                  // Acquire temperature from the microcontroller ADC
                  sensors.requestTemperatures(); //Prepara el sensor para la lectura
                  for(int i=0; i < dallas_qty; i++){
                      float dallas = sensors.getTempCByIndex(i);
                      if(DEBUG_DALLAS){
                        LOG.print("Dallas "); LOG.print(i); LOG.print(": ");
                        LOG.println(dallas);
                      }
                      Souliss_ImportAnalog(memory_map, DALLAS + (i*2), &dallas);
                      String stringDallas = "Dallas_sensor: ";
                      stringDallas += i;
                      if(Send_Emon) SendEmoncms(stringDallas, DALLAS + (i*2));
                      if(THERMOSTAT_MODE && i==0){
                         Souliss_ImportAnalog(memory_map, THERMOSTAT+1, &dallas);  //IMPORTED FROM FIRST DALLAS SENSOR FOR NOW     
                      }
                  }
                  
            } 
            
        }
        FAST_2110ms()
        {
            if(DHT_SENSOR){
                  Logic_Temperature(TEMPERATURE);
                  Logic_Humidity(HUMIDITY);
            }
            
            if(LDR_SENSOR){
                  Logic_T54(LDR);
            }
            
            if(DALLAS_SENSOR){
            	for(int i=0; i < dallas_qty; i++){
                  Logic_Temperature(DALLAS + (i*2));
            	}
            }                  
            
            if(BMP180){
    //              Read_T51(PRESSURE0);
                  Logic_Pressure(PRESSURE0);  
                  Logic_Temperature(BMP180TEMP);
            }
        } 
        
        FAST_7110ms()
        { 
            if(LDR_SENSOR){
                float ldr_read = Souliss_GetLux(in, out, SIZEOF)*10.0;  //ORIGINAL
                if(DEBUG_GETLUX){
                    LOG.print("ldrlead: "); 
                    LOG.print(ldr_read);
                }
                if (ldr_read == 0) ldr_read = 0.01;
                Souliss_ImportAnalog(memory_map, LDR, &ldr_read);
                if(Send_Emon) SendEmoncms("Lux_Sensor", LDR);
            }
        }
        FAST_x10ms(WINDOW_TIMER) { 
            if(WINDOW_CURTAIN){
                Timer_Windows(T2X);               
            }
        }
        


}

void slowGeneral(){
  
    SLOW_10s() {  // Read temperature and humidity from DHT every 110 seconds  
        if(RELAY){
            Timer_SimpleLight(RELAY0);
            Timer_SimpleLight(RELAY1);
        }
        if(ONOFF_MODE){
            Timer_SimpleLight(LEDPWM0);
            Timer_SimpleLight(LEDPWM1);
            Timer_SimpleLight(LEDPWM2);
        }
        if(PIR_MODE || PWM_MODE){
            Timer_DimmableLight(LEDPWM0);              
            Timer_DimmableLight(LEDPWM1);              
        }
        
        if(PIR_MODE){
            Timer_AutoLight(LED);
        }
        
        if(ALARM_MODE){
            Timer_Antitheft_Main(ALARM);
        }
        
        if(PWM_MODE){
            Timer_DimmableLight(LEDPWM2);                          
        }
        
        if(RGB_MODE){
            Timer_LED_Strip(LEDRGB);
        }
        if(GARAGE_DOOR){
            Timer_GarageDoor(T2X);
        }
    }            
    SLOW_x10s(2) {

        if(DHT_SENSOR){
            Souliss_GetDHT(TEMPERATURE, HUMIDITY, true);  
            if(Send_Emon) {
              SendEmoncms("DHT_Temp_Sensor", TEMPERATURE);
              SendEmoncms("DHT_Humi_Sensor", HUMIDITY);
            }
        }
    }
    SLOW_x10s(3) {
        if(BMP180){
            Souliss_GetPressure_BMP180(PRESSURE0,BMP180TEMP);  
            if(Send_Emon) {
              SendEmoncms("BMP180_Pressure", PRESSURE0);
              SendEmoncms("BMP180_Temp", BMP180TEMP);
            }  
        }   
    }

}

#endif
