// This Ardino Sketch is used to control a Keithley 7013-C relay board.
// It sends serial data to the three UCN-5814 shift registers on the Keithley card






#include <string.h>

#define Data         8  // Shift Register Data In
#define CLK          9  // Shift Register CLK
#define Strobe       10 // Shift Register Strobe
#define Latch_Enable 11 // Shift Register Enable. Active Low

byte data;
byte Relay_array[24];  //This is the main array that holds the relays states. However, the final  arrays that gets loaded to the shift registers is located in the f_shiftOut()  function

const int ON = 0;
const int OFF = 1;

bool  rel_State;

bool Debug_state;

String str_Received, str_Relay, str_State;
char char_Received[50];

//other String processing stuff
int ind1, ind_last, ind2, ind3, str_len;
int loop_index;


void setup() 
{
Debug_state = 0;  //This sets the debug state. When Debug is set to 1 (ON) it sends back serial data for the string and relay processing. Only used for Development purposes

Serial.begin(19200);
  
  pinMode(11, OUTPUT);
  digitalWrite(Latch_Enable, HIGH);
  pinMode(Data, OUTPUT);
  digitalWrite(Data, LOW);
  pinMode(9, OUTPUT);
  digitalWrite(CLK, LOW);
  pinMode(10, OUTPUT);
  digitalWrite(Strobe, LOW);
  
  //debug purposes only
  Relay_array[0] = 0x01;
  Relay_array[1] = 0x01;  //All Relays Initially OFF
  Relay_array[2] = 0x01;
  Relay_array[3] = 0x01;
  Relay_array[4] = 0x01;
  Relay_array[5] = 0x01;
  Relay_array[6] = 0x01;
  Relay_array[7] = 0x01;
  Relay_array[8] = 0x01;
  Relay_array[9] = 0x01;
  Relay_array[10] = 0x01;
  Relay_array[11] = 0x01;
  Relay_array[12] = 0x01;
  Relay_array[13] = 0x01;
  Relay_array[14] = 0x01;
  Relay_array[15] = 0x01;
  Relay_array[16] = 0x01;
  Relay_array[17] = 0x01;
  Relay_array[18] = 0x01;
  Relay_array[19] = 0x01;
  Relay_array[20] = 0x01;
  Relay_array[21] = 0x01;
  Relay_array[22] = 0x01;
  Relay_array[23] = 0x01;
  //*************************
  
  char_Received[255] =0;


}

void loop() {

  if (Serial.available())
  {
    for(int i=0; i<50; i++)
    {
      char_Received[i] = Serial.read();
     }
    String str_Received((char*)char_Received);  // Char array to String Conversion
    if (Debug_state == 1)
    {
      Serial.println(str_Received);
    }
     ind_last =  str_Received.lastIndexOf(';'); 
     str_Received.remove(ind_last + 1);
     if (Debug_state == 1)
     { 
       Serial.println(str_Received); 
     }    
    if ( str_Received == "ALL_ON;")
    {
      //f_shiftOut_All(ON); //This function is not used. Will be updated in future versions
    }

    else
    {
      if ( str_Received == "ALL_OFF;")
      {
        //f_shiftOut_All(OFF); //This function is not used. Will be updated in future versions
       }
       else
      {
    
      if (Debug_state == 1)
      {    
        Serial.print("Last Index of ; is: "); 
        Serial.println(ind_last);
      }
    loop_index = 0;   
    while (loop_index != 1)
    {
       ind1 = str_Received.indexOf('=');  // get index of the "=" sign
       str_Relay = str_Received.substring(0, ind1); //get character  that is between start of string and index of first "=". This will eventually be the Relay channel
       if (Debug_state == 1)
       {
         Serial.print("Relay Number is:");
         Serial.println(str_Relay); // Print the relay number  
       } 
       ind2 = str_Received.indexOf('O');
       ind3 = str_Received.indexOf(';');
        if (Debug_state == 1)
        {
         Serial.print("Index of first ; is :");
         Serial.println(ind3);
        }
       if ((ind3 - ind2) == 2)
          {
            rel_State = 0;   // if the index between the O and the ; is 2, that means the word is "ON"
          }
        else
          {
            if ((ind3 - ind2) == 3)
            {
              rel_State = 1;
            }
          }
             if (Debug_state == 1)
             {
               Serial.print("Relay state is: ");
               Serial.println(rel_State); 
             }
            Relay_array[str_Relay.toInt() - 1] = rel_State;
            str_len = str_Received.length();  
             if (Debug_state == 1)
             {
               Serial.print("String Length is: ");
               Serial.println(str_len);
             }
             
            if ((str_len - 1) == ind3)  
            {
              loop_index = 1;
             }  
             else
             {
              str_Received.remove(0, ind3 + 1);
               if (Debug_state == 1)
               {
                Serial.print("Trimmed loop string is: ");
                Serial.println(str_Received);
               }
             }              
         }  // end of while

          f_shiftOut();  //After the conversion is done, this sends out the data to the relays
      } // end of else
    }
  }  // end of if seial available 
  }



void f_shiftOut() 
{
  byte Relay_array_toSend[24];
  int i;
  
//The following conversion is needed because of the way the relays are connected to the shift registers and how the shift registers work
  Relay_array_toSend[0] =  Relay_array[4];
  Relay_array_toSend[1] =  Relay_array[5];
  Relay_array_toSend[2] =  Relay_array[6];
  Relay_array_toSend[3] =  Relay_array[7];
  Relay_array_toSend[4] =  Relay_array[3];
  Relay_array_toSend[5] =  Relay_array[2];
  Relay_array_toSend[6] =  Relay_array[1];
  Relay_array_toSend[7] =  Relay_array[0];

  Relay_array_toSend[8] =  Relay_array [12];
  Relay_array_toSend[9] =  Relay_array [13];
  Relay_array_toSend[10] =  Relay_array[14];
  Relay_array_toSend[11] =  Relay_array[15];
  Relay_array_toSend[12] =  Relay_array[11];
  Relay_array_toSend[13] =  Relay_array[10];
  Relay_array_toSend[14] =  Relay_array[9];
  Relay_array_toSend[15] =  Relay_array[8];

  Relay_array_toSend[16] =  Relay_array[20];
  Relay_array_toSend[17] =  Relay_array[21];
  Relay_array_toSend[18] =  Relay_array[22];
  Relay_array_toSend[19] =  Relay_array[23];
  Relay_array_toSend[20] =  Relay_array[16];
  Relay_array_toSend[21] =  Relay_array[17];
  Relay_array_toSend[22] =  Relay_array[18];
  Relay_array_toSend[23] =  Relay_array[19];
  
  
  //digitalWrite(Latch_Enable, HIGH);  //Not used. If this is used, then for every change to a relay, it will turn OFF then ON ALL the relays. 
  digitalWrite(Data, LOW);
  digitalWrite(CLK, LOW);
  delayMicroseconds(100);   
  
  for (i = 0; i<24; i++)
  {
    if (Relay_array_toSend[i] == 0x01)
       {
         digitalWrite(Data, LOW);
         delayMicroseconds(90); 
         digitalWrite(CLK, HIGH);
         delayMicroseconds(5); 
         digitalWrite(CLK, LOW);
         delayMicroseconds(100); 
        }
     else
       {
         digitalWrite(Data, HIGH);
         delayMicroseconds(5); 
         digitalWrite(CLK, HIGH);
         delayMicroseconds(5); 
         digitalWrite(CLK, LOW);
         delayMicroseconds(100); 
        }     
      
    }

    digitalWrite(Strobe, HIGH);
    delayMicroseconds(10); 
    digitalWrite(Strobe, LOW);
    delayMicroseconds(90);
    digitalWrite(Latch_Enable, LOW); //Enable is ACtive all the time. Only the Strobe signal is used to load and activate the relays. This saves on relay life expectancy
    delayMicroseconds(90);
    
 }
