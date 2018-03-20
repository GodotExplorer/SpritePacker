################################################################################
# 事件管理沟通单例
# Created by Geequlim on 2018/01/19
################################################################################

# 合并任务完成
signal pack_done(pages)

# 选中图片
signal selection_changed()

#添加最近打开的场景
signal add_recent(recent)

#清空最近打开的场景
signal clear_recent()

#主菜单点击响应事件
signal main_menu_click(menu_item)

#工具栏点击响应事件
signal main_toolbar_click(button)