
import paho.mqtt.client as mqtt
import time

mqttHost = "192.168.XXX.YYY"
mqttPort = 1883
rcvTopic = "test" # ESP32 -> server
sendTopic = "return" # server -> ESP32


class PubSub(mqtt.Client):
    
    def Connect(self):
        self.connect(mqttHost, mqttPort, 60)
        self.loop_start()


    def on_connect( self, client, userData, flag, rc ):
        print("Connect"+str(rc))
        self.subscribe(rcvTopic) # ESPからの受信を待つ

    def on_disconnect( self, client, userData, flag, rc ):
        if rc != 0:
            print("Disconnected")
    
    def on_message( self, client, userData, msg ):
        print("Recieve : "+ str(msg.payload) )
        self.publish("return","Hellow!") # ESPから受信したら送り返す


    def on_publish(self, client, userdata, msg ):
        # publishしたら呼び出される
        pass

if __name__ == "__main__":

    cl = PubSub()
    cl.Connect()

    while True:
        time.sleep(3)