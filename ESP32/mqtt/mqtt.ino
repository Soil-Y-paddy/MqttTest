#include <arduino.h>
#include <PubSubClient.h>
#include<WiFi.h>

// WiFi
const char ssid[] = "*************"; // WIFIルータのESS-ID
const char pass[] = "*************"; // ESS-IDのパスワード 

// MQTT
const char mqttHost[] = "192.168.XXX.YYY";  // MQTTブローカのIPアドレス
const int mqttPort = 1883;                  // MQTTのポート
const int QOS = 0;                          // MQTTのサービス品質

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char* pub_topic = "test";             // 送信先のトピック名
const char* subs_topic = "return";          // 受信用トピック

void setup()
{
	Serial.begin(115200);
	connectWiFi();
	connectMqtt();
	
	mqttClient.setCallback(callback);

	
}


void loop()
{
	//

     // 送信処理 topic, payloadは適宜
     char payload[] = "payload";
     mqttClient.publish(pub_topic, payload);
     delay(1000);

     // WiFi
     if ( WiFi.status() == WL_DISCONNECTED ) {
         connectWiFi(); 
     }
     // MQTT
     if ( ! mqttClient.connected() ) {
         connectMqtt();
     }
     mqttClient.loop();  
	
}

/* Connect WiFi */
void connectWiFi()
{
    WiFi.begin(ssid, pass);
    Serial.print("WiFi connecting>");
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.print("\n connected. ");
    Serial.println(WiFi.localIP());
}

/* Connect MQTT */
void connectMqtt()
{
    mqttClient.setServer(mqttHost, mqttPort);
    while( ! mqttClient.connected() ) 
    {
        Serial.println("Connecting to MQTT...");
        String clientId = "ESP32-" + String(random(0xffff), HEX);
        if ( mqttClient.connect(clientId.c_str()) ) {
            Serial.println("connected"); 
        }
        // サブスクライブ
        mqttClient.subscribe(subs_topic, QOS);
        
        delay(1000);
        randomSeed(micros());
	}
}

// Callback
void callback(char* topic, byte* payload, unsigned int length) {
  char strDump[128]="";
  sprintf(strDump,"Message arrived [%s]", topic);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}