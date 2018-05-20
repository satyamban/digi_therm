#include <lcdout.h>
#include <ESP8266WiFi.h>
#include <TaskScheduler.h>

void post();
void scan();
void reset();


Task t2(2000, TASK_FOREVER, &post);
Task t1(20000, TASK_FOREVER, &scan);
Task t3(86400000,TASK_FOREVER, &reset);

bool error = false;

WiFiServer server(6000);
int n=0;

Scheduler runner;


 WiFiClient client; 
 uint8_t fl = 1;
 char shhed[17] = "MIN   Real   MAX";
 float mint=500.0,maxt=0.0;
 
void reset()
{
  maxt = 0.0;
  mint = 500.0; 
}

void firesms()
{
   if (client.connect("maker.ifttt.com", 80)) 
   {
     Serial.print("requesting URL: ");
     
     client.print(String("GET ") + "/trigger/esp/with/key/hbOkk9myVtyw9nq0ujj1p" + " HTTP/1.1\r\n" +
               "Host: " + "maker.ifttt.com" + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n"); 
               
    Serial.println("request sent");
    
  while (client.connected()) 
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  
    String line = client.readStringUntil('\n');

    Serial.println("reply was:");
    Serial.println("==========");
    Serial.println(line);
    Serial.println("==========");
    Serial.println("closing connection");               
 }


  else
    Serial.println("connection failed");
}


void post() 
{  
  uint16_t anl; 
  float tmp;
  String s,s_mx,s_mn; 
   
  char shtem[6],shmax[6],shmin[6];
 
  String body; 
  if(!error)
  {
  if (client.connect("http://satyamban.pythonanywhere.com", 80)) 
  {    
  anl = analogRead(0); 
  tmp = (anl/1024.0)*330.0;
  
  if(tmp>50)  
   analogWrite(0,((int)tmp/10)*100);
  else
   analogWrite(0,0); 

  if(tmp>40)
    digitalWrite(16,HIGH);
  else
   digitalWrite(16,LOW);
   
  if(tmp>50)
    digitalWrite(2,HIGH);
  else
    digitalWrite(2,LOW);
    
  if(tmp>60)
    digitalWrite(14,HIGH);
  else
    digitalWrite(14,LOW);
  
   
  if(tmp>maxt)
  maxt = tmp;
  if(tmp<mint)
  mint = tmp; 
  s = String(tmp,1);
  s_mx = String(maxt,1);
  s_mn = String(mint,1);
  s.toCharArray(shtem,5);
  s_mx.toCharArray(shmax,5);
  s_mn.toCharArray(shmin,5);
  
  Serial.println("\n"+s);
  clear();
  pos(0,0);
  lprint(shhed);
  pos(1,0);
  lprint(shmin);
  pos(1,6);
  lprint(shtem);
  pos(1,12);
  lprint(shmax);  
  if(tmp>=50&&fl==1)
  {
       fl=0;
       firesms();
  }     
  body = "----temp\nContent-Disposition: form-data;name=\"Chat\"\n\n"+s_mn+"$$"+s+"$$"+s_mx+"\n----temp--"; 
  client.println("POST /10200314038_XYZ_ESP-TEMP HTTP/1.1");  
  client.println("Host: satyamban.pythonanywhere.com"); 
  client.println("Content-Length: "+ String(body.length()));  
  client.println("Content-Type: multipart/form-data; boundary=--temp");  
  client.println();
  client.println(body);     
  client.stop();              
  }
  } 
 else 
    Serial.println("Error in WiFi connection"); 
      
} 


void scan()
{
     Serial.print("Scan start ... ");
     n = WiFi.scanNetworks();
     Serial.print(n);
     Serial.println(" network(s) found");   
   
}

void reconnect(String str_1,String str_2)
{ 
  int re_conn=0;  
  clear();
  pos(0,0);
  char ssid[50],pass[50];
  WiFi.disconnect();   
  lprint("Disconnecting");
  pos(1,0);  
   while (WiFi.status() == WL_CONNECTED) 
  {  
    delay(20);   
    lprint("."); 
  }  

  str_1.toCharArray(ssid,50);
  str_2.toCharArray(pass,50);
  clear();
  pos(0,0);
   WiFi.begin(ssid,pass);   //WiFi connection  
   lprint("Connecting");
   pos(1,0);
   error = false;
  while (WiFi.status() != WL_CONNECTED) //Wait for the WiFI connection completion 
  {  
    delay(500);   
    lprint(".");
    re_conn++;
    if(re_conn==20)
    {
      clear();
      pos(0,0);
      lprint("Connection Error");
      error = true;
      break;     
    }
 
  } 
  
}

 
void setup() 
{  
  Serial.begin(115200);
  lcd_init();
  pinMode(0,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(14,OUTPUT);
  int conn = 0;
  WiFi.mode(WIFI_AP_STA);   
  
  
   pos(0,0);
   lprint("Connecting");
   pos(1,0);
   error = false;
  while (WiFi.status() != WL_CONNECTED) //Wait for the WiFI connection completion 
  {  
    delay(500);    
    lprint("."); 
    conn++;
    if(conn==20)
    {
      clear();
      pos(0,0);
      lprint("Connection Error");
      error = true;
      break;     
    }
 
  }  
  
  boolean result = WiFi.softAP("NODE", "bckv1167");
  
   if(result == true)
    {
      Serial.println("Ready");
    }
   else
    {
      Serial.println("Failed!");
    }
  
  server.begin(); 
   
     runner.init();
    
    runner.addTask(t2);
    runner.addTask(t1);
    runner.addTask(t3);

    t3.enable();
    t2.enable();
    t1.enable();
    
 
}

void loop()
{
   runner.execute(); 
    Serial.println("Waiting for Client to connect..."); 
  client = server.available();
  if(client) 
  {
    Serial.println("A client has connected .......");

     if(client.available())
    {
    String str = "";
    while(client.available())
    {
      str = str+ char(client.read());
    }
     int ind1 = str.indexOf('$');
     String str_1 = str.substring(0,ind1);
     Serial.println(str_1);
     String str_2 = str.substring(ind1+1);
     Serial.println(str_2); 
     client.stop();
     reconnect(str_1,str_2);      
    }
    
    
     if(client.connected())
    { 
      client.print(String(n)+"$");
      for (int i = 0; i < n; i++)
      {
        client.print(WiFi.SSID(i)+"$");
      }
           
     
    }
    
     client.stop();
     Serial.println("Client is disconnected");
  }
    delay(50);    
}

