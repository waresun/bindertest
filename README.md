# bindertest
mmm -B .../bindertest&&adb push out/target/product/xxx/system/bin/bindertest /system/bin/bindertest

adb reboot (if need)

adb shell ps |grep bindertest|awk 'NR==1{print $2}'|xargs adb shell kill&&adb shell /system/bin/bindertest&

adb shell dumpsys bindercalling && adb shell dumpsys bindercontainer
