![](https://tb2.bdstatic.com/tb/static-common/img/search_logo_big_v1_8d039f9.png)
# 百度贴吧图片爬取

#### 01目录下包含单线程版本的代码
#### 02目录下包含多线程版本的代码

编译
`make`

####说明
```
默认会生成两个可执行文件
teiba             ---   单线程版本的可执行文件
teiba_multi   ---   多线程版本的可执行文件

使用示例
./teiba  https://tieba.baidu.com/p/5709526060

本程序 暂时只支持 https开头的图片链接
可以修改程序正则查询部分的代码， 使其支持http开头的图片链接
```