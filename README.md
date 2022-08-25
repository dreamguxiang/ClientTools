# ClientTools
适用于修改MCBE WIN10客户端导入表并加载额外DLL的方案

## 使用方法
1. 将PeEditor.exe放入EXE所在目录   
2. 双击打开，生成出mod.exe   
3. 随后改名替换原有EXE   
4. 把ClientPreLoader放在EXE同目录下   
5. 在目录下创建`plugins`文件夹

注：PeEditor.exe可用于修改任意EXE导入表，导入函数：`imp_func`
```
.\LLPeEditor.exe --exe xxxx.exe --dll xxxx.dll
```

## 📍 许可证

| Project                                                                 | License                                   |
| ----------------------------------------------------------------------- | ----------------------------------------- |
| [LiteLoader](https://github.com/LiteLDev/LiteLoader)                    | LGPLv3                                    |
| [PeEditor](https://github.com/LiteLDev/PeEditor)                        | AGPLv3                                    |
| [MoonlyClient](https://github.com/MoonlyClient/MoonlyClient)            | Apache-2.0 license                        |