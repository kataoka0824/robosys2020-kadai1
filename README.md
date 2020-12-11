# robosys2020-kadai1
## ロボシス2020年の課題１
## ドライバの使い方
1. ```cd driver```をしてdriverのディレクトリに入ってmake.shを実行
2. ```/dev/mystep0```に 1, 2, 3, 4の順番に書き込むことによってモータを1相 (低トルク) で使用することができる。
3. ```/dev/mystep0```に 5, 6, 7, 8の順番に書き込むことによってモータを2相（高トルク）で使用することができる。
4. ```/dev/mystep0```に 0を書き込むことでフリー回転になる
## テストプログラムの使い方
1. ```cd driver && ./make.sh```を実行することでドライバのコンパイルと実装
2. robosys2020-kadai1のディレクトリにある test.py を実行することでモータの動作を確認することができる。
3. modeは0で1相、1で2相の動きをする。
- 動作の様子:
    1. 1相<br/>https://youtu.be/SrfqMdDaEHo
    2. 2相<br/>https://youtu.be/Se_fWRdSnS8
## 注意
- 使用後は```sudo rmmod mystep```を実行する
## 使用器具
- ステッピングモータ
    - SM-42BYG011
- Raspberry Pi 3B+
## 参考文献
- https://github.com/anhquan18/robosys2019_raspberry_led/blob/master/myled.c
    - Nguyen Anh Quan, 2019
