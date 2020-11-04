# code
プログラムはこちら

# Requirement
* M5Stack　Gray
* Angle Unit
* SERVO Module
* PbHUB UNIT

# Sampleフォルダ

sampleフォルダには動作確認用コードが入っています。

* wifi:wifiが接続するか確認
* serial:シリアルモタに表示されるか確認
* firbase:firebaseにアクセスできるか確認
* UltrasonicDisplay:Groveキット(7セグ)が使えるか確認
* PbHub:PbHubと角度センサーが使えるか確認
* ServoModule:サーボモジュールとサーボモーターが使えるか確認

# demoフォルダ

demoフォルダには本番デモ用コードが入っています。

開発する際にはこのコードのみをいじってください。

* firebase_face:アバターの表情を送る側のコード
* firebase_receive:受け取る側のコード


* firebase_all_in_one:受け取る側と送る側の両方を入れたコード。(サーボモーターおよび角度センサーのコード付き)


* firebase_only_face_risa:りさちゃん用のコード
* firebase_only_face_jun:任くん用のコード

# Note
* wifiの値を自分のものに変更して下さい

# Author

# Reference
* Firebaseのセットアップ
  https://www.mgo-tec.com/blog-entry-firebase-realtime-database-sever-sent-events-esp32-m5stack.html
* M5StackのLibrary: firebase-ESP32-client-document
  https://github.com/mobizt/Firebase-ESP32
* firebase-ESP32-clientの応用例
  https://github.com/mobizt/Firebase-ESP32/blob/master/examples/Beginner_start_here/Beginner_start_here.ino
