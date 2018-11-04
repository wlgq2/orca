# orca
<br>A actor framework base on c++11/14.</br>
<br>一个C++11/14风格的actor库。</br>
<br>接口上参考过Theron库，实现上避开了Theron相关不合理实现及一些可复现的bug，ping-pong测试性能优于Theron和CAF。</br>
## Dependent
网络部分基于libuv，项目中提供libuv1.22.0 gcc5.5.0及visual studio 2017编译的版本，如需其他版本则需要自己编译libuv。
## Build
Linux：提供codebolcks工程与cmake编译环境；
<br>Windows：通过Visual Studios2017编译工程。</br>
## Testing
<br>本地消息测试：</br>

 local message | Theron|CAF|orca|
:---------:|:--------:|:--------:|:--------:|
1K字节(次/秒)    | / |/|/|
4K字节(次/秒)    | / |/|/|
8K字节(次/秒)    | / |/|/|

<br>远程消息测试：</br>

 local message | Theron|CAF|orca|
:---------:|:--------:|:--------:|:--------:|
1K字节(次/秒)    | / |/|/|
2K字节(次/秒)    | / |/|/|
4K字节(次/秒)    | / |/|/|

```
注：Theron的网络部分用非阻塞+轮询实现，所以性能很差，并未处理消息丢失。
```
## How To Use
* Read Wiki for documents https://github.com/wlgq2/orca/wiki
