// 8 Relay Module Board
// Power is
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EthernetClient.h>

// Print the Variable Name
#define getName(var)  #var


const char* ssid = "ARRIS-C282";//"belkin.340";
const char* password = "F2BE3A6A71AD4BD7";// "6ae4f626";

String readString;
String ipStr ;
String s;

String lastReq = "";
String req = "";

float  OnTimer = 0;

// Print the Variable Name
#define getName(var)  #var

// Map GPIO -- to the lable Digital Pin
// D1 --> GPIO 16  (Pin D0)
/*
#define D0 16
#define D1  5
#define D2  4
#define D4  2
#define D5  14

// --- Needs to be maped
#define D6  12
#define D7  13
#define D8  15

*/
// Zero Index to 1 base index.
#define Zone00 00
//
#define Zone01 D1
#define Zone02 D2
#define Zone03 D3
#define Zone04 D4
#define Zone05 D5


#define Zone06 D6
#define Zone07 D7
#define Zone08 D8


// Number of Sprinkers to be activated:
#define MaxZones 8


// Active Low requirements:
#define SW_OFF HIGH
#define SW_ON LOW

#define BaudRate 115200

int Zones[MaxZones + 1]       = {Zone00,  Zone01, Zone02, Zone03, Zone04, Zone05, Zone06, Zone07, Zone08};
String strZones[MaxZones + 1] = {getName(null), getName(Zone01), getName(Zone02), getName(Zone03), getName(Zone04), getName(Zone05), getName(Zone06), getName(Zone07), getName(Zone08)};

//String classStyle[2] = {" x.classList = \"ONstyle\" ", " x.classList = \"OFFstyle\" "};

int  ZoneID = 0;

String GetZoneSt(int );

boolean LED_state[4] = {0}; // stores the states of the LEDs


// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


// Define functions 
long UpdateRSSI(void); 
void setS_HTML(void);
String GetZoneSt(int); 
void ZonesOFF(void); 
void ZoneON(int); 
void OtherZoneOFF(int); 
void AjaxUpdate(WiFiClient); 
void SetLEDs(void); 
void XML_response(EthernetClient); 
char StrContains(String , String );


long UpdateRSSI() {

  // Wifi Received Signal Strength
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  return rssi;
}


/*********** Setting up the HTML page *************/
void setS_HTML() {

  s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>";
  s += "<head>\r\n";

  /*****************************************************************************/
  // Set the refresh rate:
  s += "<meta http-equiv=\"refresh\" content=\"5\">\r\n";
  // s += "<meta http-equiv=\"refresh\" content=\"5;URL='"; s += ipStr +"'\">\r\n";

  //"refresh" content="0;URL='http://thetudors.example.com/'"
  /*****************************************************************************/
  // Setting the width of the page:
  //https://developer.mozilla.org/en-US/docs/Mozilla/Mobile/Viewport_meta_tag
 s += "\n";
 s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  /*
    //https://www.w3schools.com/css/css3_buttons.asp
    //https://www.w3schools.com/howto/tryit.asp?filename=tryhow_css_switch
  */

  /*****************************************************************************/
  // JaveScripts(.js) and Style Sheet (.css)
  //s += "  <link rel=\"stylesheet\" href=\"https://www.dropbox.com/s/7ez6br22dqg1x4c/OnOff.css?raw=1\" />";
  // on off .css was redireceted to this link: 
  s += "\n";  
  s += "  <link rel=\"stylesheet\" href=\"https://ucb42cf74ae2f54e41f66917d6e4.dl.dropboxusercontent.com/cd/0/inline/AtVAY7qeKZVeh6u271EFb3yKrthN8o_kEmch2-4s5BGAmtMjq_na9wzk3-zoH4_ZxPb9zvhFYNKgOgOJqC-GOPlNbvDLqkKIyvp_jAgcNe50Og/file\"/>";
 
  //s += "<script src=\"https://www.dropbox.com/s/pzgwppj4ezf2p27/ToggleState_01.js?raw=1\" type='text/javascript'> </script>";
  // s += "<script src=\"https://www.dropbox.com/s/bezcvc7zmwlz3pg/ToggleState_02.js?raw=1\" type='text/javascript'> </script>";
// _02.js was redireceted to this link: 
 s += "\n";
 s += "<script src=\"https://uc4c1325a7a8be8144693f19b930.dl.dropboxusercontent.com/cd/0/inline/AtUDLBBs1zXJDfvUSLsUXaSfd5MAMvn4bHMdtHAIiriSEEtpCdsgLtqp5b7dOvLoMqFC_G4pvb5zz2K85mEGb8Vu3vj2n2h03ayOjwTD6IbQGA/file\" type='text/javascript'> </script>";
  
  /*  <!-- https://www.w3schools.com/howto/tryit.asp?filename=tryhow_js_toggle_class -->
     <!--
     <script src="https://www.dropbox.com/s/ppnlbpni3h31hin/ToggleState.js?raw=1" type='text/javascript'>
     </script> -->
  */

  s += "</head> \r\n" ;

  s += "<BODY>";
  s += "<h1> Hello from Sami Oweis, PhD.</H1> \r\n";
  s += "<H3> Sprinker System using an ESP8266 <H3> \r\n";
  s += " IP is " + ipStr + "<p>\r\n";

  s += "<div id=\"RSSI\"> rssi " +  String(UpdateRSSI()) + "  dBm </div> \r \n ";

  /************************** custom buttons***************************************************/
  /* Set the buttons for the Zones */
  // MaxZones

  String Buttons = "";
  bool tmON  = false;
  for (int iZone = 1; iZone <= MaxZones; iZone++) {
    
      Buttons += "<input type=submit value=\"Zone " + String(iZone) + " ON\" style=width:100px;height:45px;background-color:#008CBA;border-radius:8px;color:white; onClick=location.href='/Zone/" + String(iZone) + "/SwON' target='" + ipStr + "'>" ;
      Buttons += "<input type=submit value=\"Zone " + String(iZone) + " OFF\" style=width:100px;height:45px;background-color:#f44336;border-radius:8px;color:white; onClick=location.href='/Zone/" + String(iZone) + "/SwOFF' target='" + ipStr + "'>" ;
      Buttons += "\t";
      Buttons += (String)GetZoneSt(iZone);
      Buttons += "<p>\r\n";
    

    Buttons += "<div id=\"Zone" + String(iZone) + "\" button onclick=\"ToggleState(" + String(iZone);
    if (digitalRead(Zones[iZone]) == SW_ON)
    {
      Buttons += ")\" class=\"ONstyle\">Zone " +  String(iZone) + " ON </button> </div> ";
      // if(OnTimer>0)
      //  {
      tmON = true; // (OnTimer  -(millis()/1000));
      // }else
    }
    else
    {
      Buttons += ")\" class=\"OFFstyle\">Zone " +  String(iZone) + " </button> </div> ";
    }
    //   Buttons += String(iZone)+ " </button> </div> \r\n";
Buttons += "\r\n";
  }

  Serial.println("---------------------Buttons--------------------------------------------------");
  Serial.println(Buttons);
  Serial.println();

  s += Buttons;

  if (tmON)
  {
    s += "<div id=\"OnTimer\"> On Timer : " + String((millis() / 1000) - OnTimer) + "  Secs</div> \r \n ";
  }
  else
  {
    s += "<div id=\"OnTimer\"> -------  </div> \r \n ";
  }


  s += "&nbsp;<input type=submit value='ALL OFF' style=width:100%;height:45px onClick=location.href='/Zone/ALL_SwOFF'>";

  s += "\r\n</BODY>";
  s += "</html>\r\n\r\n";
}




/************* ZONES Status ************/
// GetZoneSt(iZone)


String GetZoneSt(int ID) {

  String  st = "Zone XX is Not Determined" ;
  if ((ID >= 1) && (ID <= MaxZones) ) {
    if (digitalRead(Zones[ID]) == SW_ON)
      st = strZones[ID] + " is Active (ON) " ;
    else
      st = strZones[ID] + " is NOT Active (OFF) " ;
  }

  return st ;
}

/************* GPIOs Status ************/


void ZonesOFF() {
  String str = "";
  for (int iZone = 1; iZone <= MaxZones; iZone++)
  {
    pinMode(Zones[iZone], OUTPUT);
    digitalWrite(Zones[iZone], SW_OFF);
    Serial.println(GetZoneSt(iZone));
  }
  OnTimer = 0;
}

void ZoneON(int iZone) {
  if ((iZone <= MaxZones) && (iZone >= 1)) {
    ZonesOFF();
    digitalWrite(Zones[iZone], SW_ON);
    Serial.print(Zones[iZone]); Serial.print("  "); Serial.println(GetZoneSt(iZone));
    OnTimer = millis() / 1000;
  }
}


void ZoneOFF(int iZone) {
  if ((iZone <= MaxZones) && (iZone >= 1)) {
    digitalWrite(Zones[iZone], SW_OFF);
    Serial.print(Zones[iZone]); Serial.print("  "); Serial.println(GetZoneSt(iZone));
  }
}


void OtherZoneOFF(int iZone) {
  if ((iZone <= MaxZones) && (iZone >= 1)) {
    OnTimer = 0;
    digitalWrite(Zones[iZone], SW_OFF);
    Serial.print(Zones[iZone]); Serial.print("  "); Serial.println(GetZoneSt(iZone));
  }
}

void AjaxUpdate(WiFiClient Linkclient){
   Linkclient.println("Content-Type: text/xml");
   Linkclient.println("Connection: keep-alive");
   Linkclient.println();


/* 
   if (StrContains(HTTP_req, "ajax_inputs")) {
                        // send rest of HTTP header
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();
                        SetLEDs();
                        // send XML file containing input states
                        XML_response(client);
   }
   */

   
}



// checks if received HTTP request is switching on/off LEDs
// also saves the state of the LEDs
void SetLEDs(){
  String HTTP_req = req; 


   String strLED1 = "LED1"; strLED1.toLowerCase();
   String strLED2 = "LED2"; strLED2.toLowerCase();
   String strLED3 = "LED3"; strLED3.toLowerCase();
   
   String strOn  = "=1";   strOn.toLowerCase();
   String strOff = "=0";  strOff.toLowerCase();

    
//    int ZoneStrLen = ZoneStr.length();//strlen(ZoneStr); //.length();
//    int ZonesOFFStrPos = HTTP_req.indexOf(ZonesOFFStr);

 int strLED1Pos = HTTP_req.indexOf(strLED1);
 int strLED2Pos = HTTP_req.indexOf(strLED2);
 int strLED3Pos = HTTP_req.indexOf(strLED3);
 int strOnPos   = HTTP_req.indexOf(strOn);
 int strOffPos  = HTTP_req.indexOf(strOff);


    

    // LED 1 (pin 6)
    if ((strLED1Pos != -1) && (strOnPos != -1) ) {
        LED_state[0] = 1;  // save LED state
      //      digitalWrite(Zone, HIGH);
      ZoneON(1); 
    }
    else   if ((strLED1Pos != -1) && (strOffPos != -1) ) {
      LED_state[0] = 0;  // save LED state
      ZoneOFF(1); 
  //      digitalWrite(6, LOW);
    }
    // LED 2 (pin 7)
    if (StrContains(HTTP_req, "LED2=1")) {
        LED_state[1] = 1;  // save LED state
        digitalWrite(7, HIGH);
    }
    else if (StrContains(HTTP_req, "LED2=0")) {
        LED_state[1] = 0;  // save LED state
        digitalWrite(7, LOW);
    }
    // LED 3 (pin 8)
    if (StrContains(HTTP_req, "LED3=1")) {
        LED_state[2] = 1;  // save LED state
        digitalWrite(8, HIGH);
    }
    else if (StrContains(HTTP_req, "LED3=0")) {
        LED_state[2] = 0;  // save LED state
        digitalWrite(8, LOW);
    }
    // LED 4 (pin 9)
    if (StrContains(HTTP_req, "LED4=1")) {
        LED_state[3] = 1;  // save LED state
        digitalWrite(9, HIGH);
    }
    else if (StrContains(HTTP_req, "LED4=0")) {
        LED_state[3] = 0;  // save LED state
        digitalWrite(9, LOW);
    }
}

// send the XML file with analog values, switch status
//  and LED status
void XML_response(EthernetClient cl)
{
    int analog_val;            // stores value read from analog inputs
    int count;                 // used by 'for' loops
    int sw_arr[] = {2, 3, 5};  // pins interfaced to switches
    
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
    // read analog inputs
    for (count = 2; count <= 5; count++) { // A2 to A5
        analog_val = analogRead(count);
        cl.print("<analog>");
        cl.print(analog_val);
        cl.println("</analog>");
    }
    // read switches
    for (count = 0; count < 3; count++) {
        cl.print("<switch>");
        if (digitalRead(sw_arr[count])) {
            cl.print("ON");
        }
        else {
            cl.print("OFF");
        }
        cl.println("</switch>");
    }
    // checkbox LED states
    // LED1
    cl.print("<LED>");
    if (LED_state[0]) {
        cl.print("checked");
    }
    else {
        cl.print("unchecked");
    }
    cl.println("</LED>");
    // LED2
    cl.print("<LED>");
    if (LED_state[1]) {
        cl.print("checked");
    }
    else {
        cl.print("unchecked");
    }
     cl.println("</LED>");
    // button LED states
    // LED3
    cl.print("<LED>");
    if (LED_state[2]) {
        cl.print("on");
    }
    else {
        cl.print("off");
    }
    cl.println("</LED>");
    // LED4
    cl.print("<LED>");
    if (LED_state[3]) {
        cl.print("on");
    }
    else {
        cl.print("off");
    }
    cl.println("</LED>");
    
    cl.print("</inputs>");
}




void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println(" Reseting the Zones:  ");
  String str = "";

  for (int iZone = 1; iZone <= MaxZones; iZone++)
  {
    // Serial.print(getName(Zones[iZone]));Serial.print("   =  ");   Serial.println(Zones[iZone]);
    // Serial.print(getName(strZones[iZone]));

    //     Serial.print(strZones[iZone]);
    pinMode(Zones[iZone], OUTPUT);
    digitalWrite(Zones[iZone], SW_OFF);
    // Print the status

    /*

      str = GetZoneSt(iZone);
      Serial.println(str);
    */
    Serial.println(GetZoneSt(iZone));

  }
  // Connect to WiFi network
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  // String req = client.readStringUntil('\r');
  
  req = "";
  req = client.readStringUntil('\r');
  
  //Serial.println(req);

  // Convert to lower case:
  req.toLowerCase();

  Serial.println();
  Serial.println("___________________________________________________req___Start______________________________________________________________________________");
  Serial.println(req);
  Serial.println("___________________________________________________req___End______________________________________________________________________________");

  client.flush();
  if ((lastReq != req) && (req.indexOf("favicon.ico") == -1)) {
    lastReq = req;
    Serial.println("___________________________________________________Processing req ______________________________________________________________________________");

    // if (req == "/" )  {
    IPAddress ip = WiFi.localIP();
    ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

   

    String ZonesOFFStr = "ALL_SwOFF"; ZonesOFFStr.toLowerCase();
    

    String ZoneStr = "/zone/"; ZoneStr.toLowerCase();
    String SwOnStr = "/swon"; SwOnStr.toLowerCase();
    String SwOffStr = "/swoff"; SwOffStr.toLowerCase();
    String ZoneIDStr = "";
    int ZoneStrLen = ZoneStr.length();//strlen(ZoneStr); //.length();

    int ZonesOFFStrPos = req.indexOf(ZonesOFFStr);
    
        
    int ZoneStrPos = req.indexOf(ZoneStr);
    int SwOnStrPos = req.indexOf(SwOnStr);
    int SwOffStrPos = req.indexOf(SwOffStr);

  String AjaxInputStr = "ajax_inputs"; AjaxInputStr.toLowerCase();
  
  ZonesOFFStrPos = req.indexOf(AjaxInputStr);
    


    Serial.print("Index of Zone: "); Serial.println(ZoneStrPos, DEC);
    Serial.print("Index of SwON "); Serial.println(SwOnStrPos, DEC);
    Serial.print("Index of SwOFF "); Serial.println(SwOffStrPos, DEC);

    if (ZonesOFFStrPos != -1) {
      Serial.println();
      Serial.print(" ALL Zones are to be shut down!! ");
      ZonesOFF();
    }
    else {
      if (ZoneStrPos != -1) {

        // GPIO 13 Low
        Serial.println();
        Serial.print("Index of Zone: "); Serial.println(ZoneStrPos, DEC);

        ZoneID = 0;

        if (SwOnStrPos != -1) {
          Serial.print("Index of SwON "); Serial.println(SwOnStrPos, DEC);
          ZoneID = (req.substring(ZoneStrPos + ZoneStrLen, SwOnStrPos)).toInt();
          Serial.print("Zone to be ON "); Serial.println( strZones[ZoneID]);
          ZoneON(ZoneID);

        }
        else if (SwOffStrPos != -1) {
          Serial.print("Index of SwOFF "); Serial.println(SwOffStrPos, DEC);
          ZoneID = (req.substring(ZoneStrPos + ZoneStrLen, SwOffStrPos)).toInt();
          Serial.print("Zone to be Off "); Serial.println( strZones[ZoneID]);
          ZoneOFF(ZoneID);
        }

      }
      else {
        Serial.println("invalid request");
        // client.stop();
        //  return;
      }

    }
  }
  setS_HTML();
  Serial.println(s);
  client.print(s);

  delay(1);
  Serial.println("Client disonnected");
  req = "";

}



char StrContains(String str, String sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = str.length();//strlen(str);
    
    if (sfind.length() > len) {
    //if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
              if (sfind.length() == found){
     //       if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;    
}
