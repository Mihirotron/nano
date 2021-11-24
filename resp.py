import urllib.request
import time
import RPi.GPIO as GPIO
import json

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(9,GPIO.IN)
GPIO.setup(10,GPIO.IN)

def ThingSpeakLinkRead(link):
    RspR=urllib.request.urlopen(link)
    response = RspR.read()
    data=json.loads(response)
    Stats=data['feeds'][1]['field1']
    RspR.close()
    return Stats
    

Stats=0
Flag=1
while True:
    LinkR="https://api.thingspeak.com/channels/1578051/feeds.json?api_key=IB8IVRNUFEJP5DYO&results=2"
    Stats=ThingSpeakLinkRead(LinkR)
    if str(Stats)!='None':
        print("PIR Status: "+Stats)
        if Stats=="1":
            if Flag==1:
                StatA=GPIO.input(9)
                StatB=GPIO.input(10)
                while (StatA==1 and StatB==1) or (StatA==0 and StatB==0):
                    print("Enter Gate Status")
                    StatA=GPIO.input(9)
                    StatB=GPIO.input(10)
                    print(StatA, StatB)
                    time.sleep(1)
                if StatA==0 and StatB==1:
                    print("Gate Close")
                    print("Sending Status")
                    time.sleep(15)
                    Rspn=urllib.request.urlopen("https://api.thingspeak.com/update?api_key=9JK0GJG0SDSZ9IWT&field1=&field2=0")
                    print("Sending Status")
                    time.sleep(15)
                elif StatB==0 and StatA==1:
                    print("Gate Open")
                    print("Sending Status")
                    time.sleep(1)
                    Rspn=urllib.request.urlopen("https://api.thingspeak.com/update?api_key=9JK0GJG0SDSZ9IWT&field2=1")
                    print("Sending Status")
                    time.sleep(1)
                    Rspn=urllib.request.urlopen("https://api.thingspeak.com/update?api_key=9JK0GJG0SDSZ9IWT&field2=0")
                    print("Sending Status")
                    time.sleep(1)
                Flag=2     
        else:
            Flag=1
        time.sleep(1)
         
     

