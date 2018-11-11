# orca
A actor framework base on c++11/14.
<br>一个C++11/14风格的actor库。</br>
<br>接口上参考过Theron库，实现上避开了Theron相关不合理实现及一些可复现的bug，ping-pong测试性能优于Theron和CAF。</br>
## Dependent
网络部分基于libuv，项目中提供libuv1.22.0 gcc5.5.0及visual studio 2017编译的版本，如需其他版本则需要自己编译libuv。
<br>C++封装了一层libuv：https://github.com/wlgq2/libuv_cpp11</br>
## Build
Linux：提供codebolcks工程与cmake编译环境；
<br>Windows：通过Visual Studios2017编译工程。</br>
## Testing
本地消息测试：

 local message | Theron|CAF|orca|
:---------:|:--------:|:--------:|:--------:|
1K字节(次/秒)    | 3707 753.8 |113 343.2|3036 779.6|
4K字节(次/秒)    | 2108 082.8 |116 756.4|3089 924.6|
8K字节(次/秒)    | 1800 637.2 |111 539|3108 350.2|
```
注：Theron本地消息性能远高于CAF，orca本地消息实现zore-copy，所以只在1K字节以下性能略低于Theron。
```
<br>远程消息测试：</br>

 remote message | Theron|CAF|orca|
:---------:|:--------:|:--------:|:--------:|
1K字节(次/秒)    | 250 |5508.8|19148.4|
2K字节(次/秒)    | 250 |5426.4|17813.8|
4K字节(次/秒)    | 250 |5028|16058.2|

```
注：Theron的网络部分用非阻塞+轮询实现，所以性能很差。
```
## How To Use
* 阅读Wiki： https://github.com/wlgq2/orca/wiki
* 使用简介： https://segmentfault.com/blog/wlgq2
