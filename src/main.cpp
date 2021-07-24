#include <M5EPD.h>
#include <FS.h>

#define DocTextFile "/doc3.txt"   // 読み込みテキストファイル（UTF-8）
#define TtfFile "/font.ttf"   // フォントファイル（UTF-8）


  int gyoukan = 8;
  int pt = 32;
  int wc = 3;

M5EPD_Canvas canvas(&M5.EPD);

void setup() {
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
  canvas.loadFont(TtfFile,  SD);
  
  Serial.println("Loading done.");
  //canvas.createRender(96, 256);
  canvas.createRender(32, 256);

  canvas.drawString("OK!", 270, 254);
  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);

  M5.EPD.Clear(true);
}

String set_string(long pp){

  //String buf = u8"祗園精舎の鐘の声、諸行無常の響きあり。娑羅双樹の花の色、盛者必衰の理をあらはす。";

    String buf;
    long fpn=0; // 読み込みバイト数ポインタ
    //ファイルの中身を 一 文 字 ず つ 読み取る
    File file;
    //file = SD.open("/doc.txt",FILE_READ);
    file = SD.open(DocTextFile, FILE_READ);
    file.seek(pp);    // シークポイント

   Serial.print(F("SD FileRead: ")); Serial.print(DocTextFile);
   if(file){
       while (file.available()) {
           buf += char(file.read());
           fpn += 1;
           if(fpn> ((960/pt)*(540/pt)*wc))  break;    // 1ページ以上取れたら
      }
       Serial.println(fpn);
       Serial.println(buf);
   } else{Serial.println(F(" error..."));}
   file.close();

  //  Serial.println(buf);

    return buf;

}

void loop() {

  String buf;

  canvas.createCanvas(540, 960);

  canvas.setTextDatum(TL_DATUM);
  canvas.fillCanvas(0);


  //int lx = 540-64;
  int lx = 540-32;
  int lst = 0;
  int ly = lst;
  

  long cnt = 0;

  canvas.setTextSize(pt);
  buf = set_string(0);
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
    // 改行処理
    if( cnt > (960/pt)-1){  // 縦サイズ/文字サイズ -1 (1文字分少なく) を超えていたら改行
      cnt=0;
      lx -=(pt+gyoukan);
      ly=lst;
    }
    if(buf.charAt(i)==13){  // CR だったら？　（Windows系の CR/LFの頭のコード(0Dh)を見ているのみ。）MacならLF（0Ah)を見るべき
      i+=2;   // Windows系のCR/LFの2バイトをジャンプ -> Macならここは ＋１で。
      cnt=0;
      lx -= (pt+gyoukan);
      ly=lst;
    }

    //1バイトなら？
    if(buf.charAt(i)>8 and buf.charAt(i)<128){
      canvas.drawChar(buf.charAt(i), lx, ly);
      Serial.print(buf.charAt(i));
       //i+=1;
       i -= 2 ;   // 3バイト幅なので、次ループ用に2バイト戻しておく
      //wc=1;
    } else if (buf.charAt(i)<8) {  
      // 2バイトなら。。（未テスト）
        canvas.drawString(buf.substring(i, i+2), lx, ly);
        Serial.print(buf.substring(i, i+2));
        i -= 1 ;     // 3バイト幅なので、次ループ用に1バイト戻しておく
        break;
    } else  {
        // ここに来るのは3バイト
        Serial.print(buf.substring(i, i+wc));

        // 。や、の処理。無理やり描画
        if(buf.substring(i, i+wc).equals("。")){
          canvas.drawCircle(lx+(pt/1.2), ly+(pt/4), 4, 15);
        } else if (buf.substring(i, i+wc).equals("、")){
          canvas.drawLine(lx+(pt/1.2), ly+(pt/4),lx+(pt/1.2)+4, ly+(pt/4)+4, 4, 15);
        }  else if (buf.substring(i, i+wc).equals("ー")){
          canvas.drawLine(lx+(pt/2), ly+(pt/4),lx+(pt/2), ly+(pt-(pt/4)), 3, 15);
        } else {
            canvas.drawString(buf.substring(i, i+wc), lx, ly);
        }

    }

    
    ly += pt;
  
  }


 // canvas.pushCanvas(0,0,UPDATE_MODE_DU4);
  canvas.pushCanvas(0,0,UPDATE_MODE_GLD16);

   while(1) {}   // 無限ループ
  
}