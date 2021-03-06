#include <M5EPD.h>
#include <FS.h>

#define DocTextFile "/doc.txt"   // 読み込みテキストファイル（UTF-8）
#define TtfFile "/font.ttf"   // フォントファイル
//#define TtfFile "/GenEiKoburiMin6-R.ttf"   // フォントファイル


int gyoukan = 8;    // 行間
int pt = 32;            //  フォントサイズ：ポイント
int wc = 3;             // UTF-8の基本ワードバイト数

  int lst = 0;          // ページ上部空き


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
  canvas.createRender(pt, 256);

  canvas.setTextSize(pt); //フォントサイズ指定

  canvas.drawString("OK!", 270, 254);
  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);

  M5.EPD.Clear(true);

    canvas.createCanvas(540, 960);

  canvas.setTextDatum(TL_DATUM);
  canvas.fillCanvas(0);

  canvas.setTextSize(pt); //フォントサイズ指定


}

void test(){
  //縦書き試験用サンプル
  String buf = u8"諸行無常の響きあり。";
  canvas.drawString(buf, 0, pt*9);
  /*
  for(int i=0;i<10;i++){
     canvas.drawString(buf.substring(i*3, i*3+wc), pt*9, i*pt);
  }
  */

//void setTextArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
  canvas.setTextArea(pt*9, 0,pt, 11*pt);
  /*
    TL_DATUM = Top left
    TC_DATUM = Top centre
    TR_DATUM = Top right
    ML_DATUM = Middle left
    MC_DATUM = Middle centre
    MR_DATUM = Middle right
    BL_DATUM = Bottom left
    BC_DATUM = Bottom centre
    BR_DATUM = Bottom right
  */
  canvas.setTextDatum(TC_DATUM);  // 1文字づつでは関係なかった

  canvas.print(buf);

  canvas.pushCanvas(0,0,UPDATE_MODE_GLD16);
  while(1){}
}

String set_string(long pp){

    String buf;
    long fpn=0; // 読み込みバイト数ポインタ
    //ファイルの中身を 一 文 字 ず つ 読み取る
    File file;
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

int pageView(String buf){
  //String buf;

  //int lx = 540-64;
  int lx = 540-32;
  int ly = lst;

  long cnt = 0; //ページ内文字カウント
  
  int l =buf.length();
  Serial.println(l);

  canvas.fillCanvas(0);

//  Serial.println(buf.charAt(1)); 
int i;
  for(i=0;i<l;i+=wc){
     cnt+=1;
    // 改行処理
    bool ret_on=false;
    if( cnt > (960/pt)-1)  {  // 縦サイズ/文字サイズ -1 (1文字分少なく) を超えていたら改行
      cnt=0;
      lx -=(pt+gyoukan);
      ly=lst;
      ret_on=true;
    }
    if(buf.charAt(i)==13){  // CR だったら？　（Windows系の CR/LFの頭のコード(0Dh)を見ているのみ。）MacならLF（0Ah)を見るべき
      i+=2;   // Windows系のCR/LFの2バイトをジャンプ -> Macならここは ＋１で。
      cnt=0;
      lx -= (pt+gyoukan);
      ly=lst;
      ret_on=true;
    }
    if(ret_on==true){ //改行あり。ページ末チェック
      if(lx<pt/2){  // 最新のlx が文字幅以下なら？
        Serial.print(":PEnd(i)：");Serial.println(i);
        Serial.print("次文字：");
        Serial.println(buf.substring(i, i+3));
        break;
      }
    }

    // UTF-8 Check https://seiai.ed.jp/sys/text/java/utf8table.html 参照
    //1バイトなら？
    //if(buf.charAt(i)>8 and buf.charAt(i)<128){
    if(buf.charAt(i)<0x80){
      canvas.drawChar(buf.charAt(i), lx, ly);
      Serial.print(buf.charAt(i));
       //i+=1;
       i -= 2 ;   // 3バイト幅なので、次ループ用に2バイト戻しておく
      //wc=1;
    } else if (buf.charAt(i)>0xC0 and  buf.charAt(i)<0xE0) {  
      // 2バイトなら。。（未テスト）
        canvas.drawString(buf.substring(i, i+2), lx, ly);
        Serial.print(buf.substring(i, i+2));
        i -= 1 ;     // 3バイト幅なので、次ループ用に1バイト戻しておく
        break;

      } else  if (buf.charAt(i)>0xF0 and buf.charAt(i)>0xF7 ) {  
        canvas.drawString(buf.substring(i, i+wc+1), lx, ly);  // 4バイト文字
        i += 1 ; 
      } else  if (buf.charAt(i)>0xF8 and buf.charAt(i)>0xFB ) {  
        canvas.drawString(buf.substring(i, i+wc+2), lx, ly);  //５バイト文字
        i += 2 ; 
      } else  if (buf.charAt(i)>0xFC and buf.charAt(i)>0xFD ) {  
        canvas.drawString(buf.substring(i, i+wc+3), lx, ly);  //6バイト文字
        i += 3 ; 

    } else  {   //  else if (buf.charAt(i)>0xE0 and buf.charAt(i)>0xF0 ) {  ※本来3バイト文字は 0xE0～0xF0が開始バイトのようだけれどそうでもない？？　ここでは上に該当するもの以外を3バイト文字として表示する。
        // ここに来るのは3バイト 
        //Serial.print(buf.substring(i, i+wc));

        //「 。」「、」「ー」の処理。無理やり描画
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

  return(i);

}

void coverView(){
      canvas.fillCanvas(0);
      canvas.drawJpgFile(SD, "/image.jpg");   // image.jpgがあれば表示
      canvas.pushCanvas(0,0,UPDATE_MODE_GLD16);

      while(1){
          if( M5.BtnR.wasPressed()){
            M5.update();
            break;
          }
          M5.update();
          delay(100);       
      }

}


void loop() {

  //test();

  coverView();

  long page_p[1000];   // ページポインタ：とりあえず1000ページまで

  String buf;
  int this_page=0;  //現在ページ

  page_p[this_page]=0;

  buf = set_string(page_p[this_page]);
  Serial.println(buf);

  //this_page+=1;
  page_p[this_page+1] = pageView(buf);    // pageView の戻り値はそのページの末尾＋１
  Serial.println(page_p[this_page+1]);
  

   while(1) {

    if( M5.BtnL.wasPressed()) { // Back
        Serial.println("Btn L Pressed");
        if(this_page>0){

            this_page-=1;

            buf = set_string(page_p[this_page]);
              
            Serial.print("OldEndP:");          Serial.println(page_p[this_page+1]);  
            long ll = pageView(buf);    // pageView の戻り値はそのページの末尾＋１
            Serial.print("NewEndP:");          Serial.println(ll);
           // page_p[this_page+1] = ll;
        }
       

    }

    if( M5.BtnP.wasPressed()){
          Serial.println("Btn P Pressed: オマケ。表紙描画");
          coverView();
          this_page=0;
          page_p[this_page]=0;

          buf = set_string(page_p[this_page]);
          page_p[this_page+1] = pageView(buf);    // pageView の戻り値はそのページの末尾＋１

    }

    if( M5.BtnR.wasPressed())
    {
        Serial.println("Btn R Pressed");
        Serial.println("NextPage!");
        canvas.fillCanvas(0);

        this_page+=1;

        buf = set_string(page_p[this_page]);
        
        page_p[this_page+1] = page_p[this_page] + pageView(buf);    // pageView の戻り値はそのページの末尾＋１
        Serial.println(page_p[this_page+1]);


    }
    M5.update();
    delay(100);

   }   // 無限ループ
  
}