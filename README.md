# M5Paper_TextView

簡易版M5Paper用テキスト縦書きビューワ（超力業で無理やり表示しています）


## 使い方
### 準備
* MicroSDカードに日本語フォントを ”/font.ttf ” として保存。
* 表示したいテキストをUTF－8形式で "/doc.txt"として保存。

### コンパイル＆転送
* VSCode + PratformIO でコンパイルおよび実機への転送をしてください。詳しくは、 https://note.com/rasen/n/n722764b12f20 を参照のこと。（一度プロジェクトを新規作成して、そこにソースを突っ込むと楽かも？）
    * main.cpp をそのままArduinoIDEにコピペしても行けそうな気はしますが未確認です。

### 操作
* テキストファイルが縦書きで表示されます。ページ遷移は本体サイドのSWの上下で進んだり戻ったりします。
* ページ操作時にSDカードから「そのページ部分」のテキストを読み込んで表示します。なので、大きなファイルも比較的高速です。

## ご注意
* あくまで簡易版です。「。」や「、」「ー」の回転が出来なかったので無理やり描画しています。
    * なので、手が回らなかった他の記号類は回転できていません。
* 現在32ポイント表示限定です。他のサイズにすることも（ソース参照）できる、はず（未確認）
* 行間や上部の空きも設定可能（なハズ）
* UTF-8の解釈（１バイト～3バイトコード判定）も自力でやっているので、なんだか変です（ぉぃ）2バイトコードが混ざると文字化けしそう＞＜
* 現状ページ数の限界は１０００ページまで。『虚数の情緒』ぐらいの本まではイケます！（無理）

## 製作記事
* https://note.com/rasen/n/n3d3f056d66f7
    * ↑こちらのnoteでうだうだ書いてます～。
    