#include <Arduino.h>
#include <SPI.h>
#include <stdio.h>
#include <stdint.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <time.h>
#include <sntp.h>
// #include <picture.h>

const char* ssid        = "LUMI_TEST";
const char* password    = "lumivn274!";

const char* ntpServer1 = "0.asia.pool.ntp.org";
const char* ntpServer2 = "asia.pool.ntp.org";
const long  gmtOffset_sec = 6*3600;
const int   daylightOffset_sec = 3600;

// int imageW=100;
// int imageH=81;

// int screenW=480;
// int screenH=320;
// int m=imageW;

unsigned short imageS[8100]={0}; // edit this screenW * screen H

TFT_eSPI tft = TFT_eSPI();

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    //tft.printf("No time available (yet)");
    return;
  }

  //Display Date & Time
  tft.fillRect(20, 80, 260, 20, TFT_RED);
  tft.setCursor(130, 80);
  tft.print(&timeinfo, "%a %b %d");
  tft.setCursor(30, 80);
  tft.print(&timeinfo, "%H:%M:%S");
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  tft.init();

  pinMode(23, OUTPUT);
  digitalWrite(23, 128);

  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  // set notification call-back function
  sntp_set_time_sync_notification_cb( timeavailable );

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  //connect to WiFi
  tft.setTextSize(2);
  Serial.printf("Connecting to %s ", ssid);
  tft.fillScreen(TFT_BLACK);
  tft.printf("Connecting to ");
  tft.setCursor(30, 80);
  tft.printf(ssid);
  delay(1000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  tft.fillScreen(TFT_BLACK);
  tft.printf("CONNECTED");
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  tft.drawRect(20, 20, 440, 280, TFT_BLUE);
  tft.drawLine(280, 20, 280, 300, TFT_BLUE);

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Xin chao LUMI", 30, 40);

  // tft.setCursor(60,150);
  // tft.pushImage(0,0,screenW,screenH,imageS);

  delay(1000);
  printLocalTime();
}