# 小智聊天机器人APP版本

支持在思澈solution框架下，将小智AI作为一个应用程序运行。

C语言实现了小智iot的基础功能，可以用iot thing manager API管理硬件。目前已支持扬声器和电话拨号程序。

SiFli solution 1.6.0版本编译通过，理论上此版本支持的硬件均可运行。

目前基础功能可用，休眠、退出、后台运行等功能可能存在问题，待后续实现和优化。

## 源码构建

1. 复制仓库内app/src目录下的源文件到watch\application\xiaozhi目录，不需要字体、图片、opus_heap等。
2. 复制仓库内app/asset目录下的png文件到watch\sifli\resource\images\xiaozhi\common\ezip目录下。
3. menuconfig打开PKG_USING_XIAOZHI。
3. Butterfli编译。

## 管理地址

后台管理地址为: <https://xiaozhi.me>
