## 【目录说明】

* UI相关放在`ui`目录下，一个组件一个目录
* 逻辑的放`scripts`里
* 通用资源放`assets`

## 【UI制作提示】

1. 需要与程序交互的UI控件全部用 `export(NodePath)` 的方式导出
2. 尽量抽象组件化UI控件，提高复用率
3. 节点名称尽量使用有意义的命名方式，允许并鼓励使用`中文`
4. 组件应具备独立性，不得操作组件外的节点
5. 组件间通过`signal`通信，通过`setget`更新属性并刷新UI元素

#### UI布局时控件类型的选择流程：

1. SplitContainer ScrollContainer
2. HBoxContainer/VBoxContainer
4. CenterContainer/MarginContainer
5. GridContainer/PanelContainer
6. Control
7. Node2D

## 【NEVER DO IT】

```
get_node("..")
get_node("../xxxxx")
```

## 【命名规范】

* 类名使用`PascalCase`方式
* 一般变量和方法使用`小写+下划线`的方式
* 常量使用全`大写+下划线`的方式


## 【编码提示】

* 全局只提供一个单例`global`，其他对象需要作为单例的请将其添加到`global.gd`内，作为该类的成员
* 全局global的获取方式为`get_node('/root/global')`, 请不要直接使用`global`变量，这样会造成它在编辑器解析脚本报错的问题。

## 【主窗口菜单信号使用说明】

* 主界面菜单脚本文件：`scripts\ui\main_menu\MainMenu.gd`,场景文件:`ui\Menu\MainMenu.tscn`
* menus为Dictionary类型的数据，存放了所有的主菜单
* 信号已集成到global.messenger,如需调用请参考messenger的信号调用方式

* ~~menu的id请查看`assets\json\UI_main_menu.json`文件中的对应项目~~
* menu的id需要`connect global.messenger main_menu_click(menu,id)`查看