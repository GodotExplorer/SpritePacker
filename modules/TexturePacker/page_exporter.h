#ifndef PAGE_EXPORTER_H
#define PAGE_EXPORTER_H
#include <core/reference.h>
#include <core/error_list.h>
#include "texture_packer.h"
#include "texture_packer_config.hpp"

class PageExporter : public Reference {
	GDCLASS(PageExporter, Reference);
protected:
	virtual String get_page_description(Ref<PackedPage> page, Ref<TexturePackerConfig> config, const String& image_file) const = 0;
	virtual String get_meta_extention() const = 0;
	Error save_text_file(const String& content, const String& path) const;
	Error save_image_file(Ref<Image> img, Image::Format format, const String& path) const;
	String get_image_extention(Image::Format format) const;
	static void _bind_methods();
public:
	virtual Error export_pages(const Array& pages, Ref<TexturePackerConfig> config) const;
};

class GenericXMLExporter : public PageExporter {
	GDCLASS(GenericXMLExporter, PageExporter);
public:
	virtual String get_page_description(Ref<PackedPage> page, Ref<TexturePackerConfig> config, const String& image_file) const;
	virtual String get_meta_extention() const { return "xml"; }
};

#endif // PAGE_EXPORTER_H
