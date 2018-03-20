################################################################################
# 辅助工具脚本
# Created by Geequlim on 2018/01/14
################################################################################


# 弹出消息对话框并暂停程序  
# - - - - - - - - - -  
# *Parameters*  
# * [title:String] 对话框标题  
# * [msg:String] 对话框消息内容  
# * [buttons: Array<String>] 对话框按钮标题列表  
# - - - - - - - - - -  
# *Returns* String  
# * 返回用户按下的按钮标题
static func message_box(title, msg, buttons=["OK"]):
    var dlg = NativeMessageDialog.new()
    dlg.message = msg
    dlg.buttons = buttons
    dlg.show()
    return dlg.get_response_button_title()
	
# 弹出文件选择对话框并暂停程序  
# - - - - - - - - - -  
# *Parameters*  
# * [title:String] 对话框标题
# * [filters:Array<Dictionary>] 文件类型:e.g.[{"title":"TXT文本文件","type":"txt"},{"title":"JPEG图像文件","type":"jpg"}]
# * [mode:int] 对话框选模式:  MODE_SELECT_FILE,MODE_SELECT_DIR,MODE_SELECT_FILE_DIR,MODE_MULTI_SELECT,MODE_SAVE
# * [default_path:String] 对话框显示初始路径  
# - - - - - - - - - -  
# *Returns* Array<String>  
# * 返回用户按下的文件路径
static func file_dialog(title="", filters=[], mode=NativeFileDialog.MODE_SELECT_FILE, default_path=""):
	var dlg = NativeFileDialog.new()
	
	if title!="":
		dlg.title = title
	
	if not filters.empty():
		for filter in filters:
			dlg.add_filter(filter.title,filter.type)
	
	if default_path!="":
		dlg.default_path = default_path
		
	if mode != NativeFileDialog.MODE_SELECT_FILE:
		dlg.mode = mode
		
	dlg.show()
	return dlg.get_selected_pathes()
	
