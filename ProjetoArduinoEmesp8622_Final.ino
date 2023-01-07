
/*librarys*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DFRobot_DHT11.h>
//#include <DFRobot_DHT11.h> //todo: LL
/*-----------------------------*/

//define DHT sensor
//DFRobot_DHT11 DHT;
DFRobot_DHT11 DHT;


/*sensor and vent outpts and leds*/
#define VENT 16
#define HEATER 13
#define DHT11_PIN 14

/*DHT dht(SENSOR, DHT21); //define what sensor are we using*/

#define SERVER_IP "smarthouse23.azurewebsites.net"  //website link

/*network*/
#ifndef STASSID
#define STASSID "iPhone do Pedro"  //Wifi ssid
#define STAPSK "arduino123" //WiFi passWord 
#endif




//////NETWORK AND WIFI STUFF
void wifiSetup() {
  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

String syncData(int temp, int hum) {
  // wait for WiFi connection

  String result = "";
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url

    String tempStr = String(temp);
    String humStr = String(hum);
    http.begin(client, "http://" SERVER_IP "/Home/sync?temperature=" + tempStr + "&humidity=" + humStr);  //HTTP //todo: find out how to connect stings
    //http.begin(client, "http://" SERVER_IP "/Home/sync?temperature=" + temp.toString()" + "&humidity=40"); //HTTP //todo: find out how to connect stings
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    int httpCode = http.POST("");

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
        result = payload;
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  return result;
}

//////////////



void setup() {

  Serial.begin(115200);
  //dht.begin();

  pinMode(HEATER, OUTPUT);
  pinMode(VENT, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.println();

  wifiSetup();
}

void loop() {

  //todo: obtain temperature from sensor

  //todo: otain sensor humidity
  //int temp = DHT.temperature;
  //int hum = DHT.humidity;
  /*dht.read();
  Serial.print("passou do dht.read");*/
  /*float temp = dht.getTemperatureC();
  float hum = dht.getHumidity();*/
  DHT.read(DHT11_PIN);
  int temp = DHT.temperature;
  int hum = DHT.humidity;
  String triggerStr = syncData(temp, hum);  //todo: needs 2 to chanche the funcion to resive humidity and temperature data, and gives a int that is the vent trigger number



  int trigger = triggerStr.toInt();


  if (temp > trigger) {
    Serial.print("ventuinha ligado ligada");
    digitalWrite(VENT, HIGH);
    digitalWrite(HEATER, LOW);
  } else if (temp < trigger) {

    Serial.print("aquecedor ligado ligada");
    digitalWrite(VENT, LOW);
    digitalWrite(HEATER, HIGH);

  } else {
    digitalWrite(VENT, LOW);
    digitalWrite(HEATER, LOW);
  }

  delay(5000);
}
