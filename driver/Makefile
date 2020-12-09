obj-m := mystep.o        #オブジェクトファイルの名前を指定（拡張子はo）

mystep.ko: mystep.c
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` V=1 modules

clean:
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` V=1 clean
