#include <M5EPD.h>
#include <FS.h>

#include <WString.h> 
#include <string>

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

  //String buf = u8"祗園精舎の鐘の声、諸行無常の響きあり。娑羅双樹の花の色、盛者必衰の理をあらはす。";

    String buf;

    //ファイルの中身を 一 文 字 ず つ 読み取る
    File file;
    //file = SD.open("/doc.txt",FILE_READ);
    file = SD.open("/doc2.txt",FILE_READ);
    while(file.available()){
      buf.concat(file.readString());
    }
    Serial.println(buf);

    return buf;

}

void loop() {

  String buf;

  canvas.createCanvas(540, 960);

  canvas.setTextDatum(TL_DATUM);
  canvas.fillCanvas(0);


  //int lx = 540-64;
  int lx = 500;
  int lst = 16;
  int ly = lst;
  

  int pt = 32;
  int wc = 3;
  long cnt = 0;

  canvas.setTextSize(pt);
  buf = set_string();
  Serial.println(buf);

  int l =buf.length();
  Serial.println(l);

Serial.print("CR:");
Serial.println( '\r', HEX);
Serial.print("LF:");
Serial.println( '\n', HEX);


//  Serial.println(buf.charAt(1)); 

  for(int i=0;i<l;i+=wc){
    cnt+=1;
    if(cnt>30){
      cnt=0;
      lx -= pt;
      ly=lst;
    }
    if(buf.charAt(i)==13){  // CR だったら？　（Windows系の CR/LFの頭のコード(0Dh)を見ているのみ。）MacならLF（0Ah)を見るべき
      i+=2;   // Windows系のCR/LFの2バイトをジャンプ -> Macならここは ＋１で。
      cnt=0;
      lx -= pt;
      ly=lst;
    }

    Serial.println(buf.substring(i, i+wc));
    canvas.drawString(buf.substring(i, i+wc), lx, ly);
    ly += pt;
  }


  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);

  delay(50000);
  
}