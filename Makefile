obj-m := myled.o        #オブジェクトファイルの名前を指定（拡張子はo）

myled.ko: myled.c
	    make -C /usr/src/linux-headers-`uname -r` M=`pwd` V=1 modules

clean:
	    make -C /usr/src/linux-headers-`uname -r` M=`pwd` V=1 clean
