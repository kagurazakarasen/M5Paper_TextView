#include <M5EPD.h>
#include <FS.h>

#include <WString.h> 

#include <locale.h>

M5EPD_Canvas canvas(&M5.EPD);

void setup() {
  setlocale(LC_CTYPE, "ja_JP.UTF-8");
  M5.begin();
  M5.TP.SetRotation(90);
  M5.EPD.SetRotation(90);
  M5.EPD.Clear(true);

  canvas.createCanvas(540, 960);
  canvas.setTextDatum(TC_DATUM);
  canvas.setTextSize(3);

  canvas.drawString("... Initializing ...", 270, 640);
  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);


  // SDからフォントを読み込む
  canvas.drawString("Loading font ...", 270, 230);
  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);
  Serial.println("Loading font from SD.");
  canvas.loadFont("/font.ttf", SD);
  Serial.println("Loading done.");
  canvas.createRender(96, 256);
  canvas.createRender(32, 256);

  canvas.drawString("OK!", 270, 254);
  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);

  M5.EPD.Clear(true);
}

String set_string(){
  String buf = "祗園精舎の鐘の声、諸行無常の響きあり。娑羅双樹の花の色、盛者必衰の理をあらはす。";
  return buf;
}

void loop() {

  String buf;

  canvas.createCanvas(540, 960);

  canvas.setTextDatum(TL_DATUM);
  canvas.fillCanvas(0);


  int lx = 540-64;

  int pt = 32;

  canvas.setTextSize(pt);
  buf = set_string();
  Serial.println(buf.c_str());

  Serial.println(buf.length());

  Serial.println(buf.charAt(1)); 

  canvas.drawString(buf,10, 32, true);


/*
  buf = "祗園精舎の鐘の声、";
  canvas.drawString(buf, lx, 32, true);
  lx -= pt;

  buf = "諸行無常の響きあり。";
  canvas.drawString(buf, lx, 32, true);
  lx -= pt;

  buf = "娑羅双樹の花の色、";
  canvas.drawString(buf, lx, 32, true);
  lx -= pt;

  buf = "盛者必衰の理をあらはす。";
  canvas.drawString(buf, lx, 32, true);
  lx -= pt;

  buf = "おごれる人も久しからず、";
  canvas.drawString(buf, lx, 32, true);
  lx -= pt;

  buf = "唯春の夜の夢のごとし。";
  canvas.drawString(buf, lx, 32, true);
  lx -= pt;

  buf = "たけき者も遂にはほろびぬ、";
  canvas.drawString(buf, lx, 32, true);
  lx -= pt;

  buf = "偏に風の前の塵に同じ。";
  canvas.drawString(buf, lx, 32, true);
  lx -= pt;
*/
  

/* 

canvas.print("祗園精舎の鐘の声、");
canvas.print("諸行無常の響きあり。");
canvas.print("娑羅双樹の花の色、");
canvas.print("盛者必衰の理をあらはす。");
canvas.println("");
canvas.print("おごれる人も久しからず、");
canvas.print("唯春の夜の夢のごとし。");
canvas.print("たけき者も遂にはほろびぬ、");
canvas.print("偏に風の前の塵に同じ。");
*/

  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);

  delay(50000);
  
}