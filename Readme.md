
## 一、MergePicture
合并几张相同大小的图片。基于OpenCV。

## 二、如何使用

* 下载源码，解压进入目录执行命令:  
```
cd run
cmake ..
make
./MergePicture -c 8 -m 2 -n 2 -t src.txt -s merge -p ./图片组1 ./图片组2 ./图片组3 ./图片组4
```
执行完上面的命令没有报错，run目录内会有一个叫做merge
的文件夹，如果里面有8张图片，说明程序已经可以正常运行。
* 调用方法  
```
./MergePicture -c number -m number -n number -t txtName -s save_path -p path1 path2 ... pathm*n
 ```
* 参数解释  
-c 要合并图片的组数;  
-m 合并后横向包含几张图片;  
-n 合并后纵向包含几张图片;  
-t 这m*n个目录中含有目录中图片名字的txt文件名，每个
目录中的名字都需要相同;  
-s 指定保存路径;  
-p 指定m*n个包含图片名称的txt文件路径;若路径数量不
够m*n，则会用空白补足。  

* 调用demo:  
```
./MergePicture -c 8 -m 2 -n 2 -t src.txt -s merge -p ./图片组1 ./图片组2 ./图片组3 ./图片组4
```