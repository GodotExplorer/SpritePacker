################################################################################
# 项目唯一全局单例
# Created by Geequlim on 2018/01/14
################################################################################
extends Node

# gdutils 插件库
const gdutils = preload("res://addons/gdutils/__init__.gd")
# 接受的文件后缀类型
const ACCEPT_EXTENTIONS = ["png", "jpg"]

# 通用辅助工具
var utils = preload("../utils/utils.gd").new()
# 通信专用单例
var messenger = preload("messenger.gd").new()
# UI相关
var ui = null
# 合图配置
var config = preload("config.gd").new()
# 合并结果的页码
var pages = []
# 导出器
var exporter = null

# 合图服务
var _pack_server = null

func _init():
	ui = preload("ui.gd").new(self)
	config.add_slot(self, "start_pack")
	OS.low_processor_usage_mode = true # 开启低功耗模式
	
	messenger.connect("main_menu_click",self,"_main_menu_click")
	messenger.connect("main_toolbar_click",self,"_main_toolbar_click")
	messenger.connect("pack_done", self, "_on_pack_done")

func _enter_tree():
	_pack_server = preload("pack_server.gd").new(self)
	_pack_server.start()

func _ready():
	get_tree().connect("files_dropped", self, "append_files")
	load_settings()

func _exit_tree():
	_pack_server.stop()
	save_settings()

func _on_pack_done(p_pages):
	self.pages = p_pages

# 向合图配置中添加文件
# - - - - - - - - - -  
# *Parameters*  
# * [files: PoolStringArray] 文件路径  
# * [screen: int] 无用，但是拽入文件回调需需 
func append_files(files, screen=0):
	var images = Array(config.image_files)
	var file_added = false
	for file in files:
		if file.get_extension().to_lower() in ACCEPT_EXTENTIONS:
			if not file in images:
				images.append(file)
				file_added = true
	if file_added:
		config.image_files = images
		config.notify_change()
		
func start_pack():
	_pack_server.request_pack(self.config)

# 加载配置
func load_settings():
	# TODO: 加载应用配置
	# 设置主题
	var theme = "default"
	ui.set_theme(load(str("res://assets/themes/", theme, ".theme")))
	
# 保存配置
func save_settings():
	pass
	# TODO: 保存应用配置

# 设置导出器
func set_exporter(expter):
	if typeof(expter) == TYPE_OBJECT and expter is PageExporter:
		self.exporter = expter
	else:
		self.exporter = null

# 导出现有合并结果
func export_pages():
	if exporter:
		if not pages.empty():
			if OK == exporter.export_pages(self.pages, self.config):
				utils.message_box("", "Export Success!")
		else:
			utils.message_box("Export Failed", "Please add some sprite first!")
	else:
		utils.message_box("Export Failed", "Please select a valid export format!")

#响应主菜单
func _main_menu_click(menu_item):
	#退出
	if menu_item=="File/New project":
		new_project()
	elif menu_item=="File/Exit":
		get_tree().quit()

func _main_toolbar_click(button):
	if button == "new_project":
		new_project()

#打开新工程，即打开新窗口
func new_project():
	OS.execute(OS.get_executable_path(),[""],false)
#下面的先不要动，我要做快捷键用
# func ED_SHORTCUT(path, name, keycode):
#     var ie = InputEvent.new()
    
#     ShortCut sc = ShortCut.new()
#     if keycode:
#         ie.instance()
    
#     sc.shortcut = keycode

