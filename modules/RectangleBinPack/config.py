#!/usr/bin/env python

def can_build(platform):
	return True

def configure(env):
	pass

def get_doc_classes():
    return [
        "RectangleBinPack",
        "MaxRectsBinPack",
        "GuillotineBinPack",
        "ShelfBinPack",
        "SkylineBinPack",
    ]

def get_doc_path():
    return "doc_classes"
