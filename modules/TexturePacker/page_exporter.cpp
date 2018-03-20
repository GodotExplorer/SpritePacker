#include "page_exporter.h"
#include <core/os/file_access.h>
#include <core/class_db.h>

Error PageExporter::save_text_file(const String &content, const String &path) const {
	if (FileAccessRef f = FileAccess::open(path, FileAccess::WRITE)) {
		f->store_string(content);
		f->flush();
		f->close();
		return OK;
	}
	return ERR_FILE_CANT_OPEN;
}

Error PageExporter::save_image_file(Ref<Image> img, Image::Format format, const String &path) const {
	ERR_FAIL_COND_V(img.is_null(), ERR_INVALID_PARAMETER);
	return img->save_png(path);
// TODO: Save images with other formats
//	if (FileAccessRef f = FileAccess::open(path, FileAccess::WRITE)) {
//		if (img->get_format() != format) {
//			img = img->duplicate();
//			img->convert(format);
//		}
//		const PoolByteArray & data = img->get_data();
//		f->store_buffer(data.read().ptr(), data.size());
//		f->flush();
//		f->close();
//		return OK;
//	}
	return ERR_FILE_CANT_OPEN;
}

String PageExporter::get_image_extention(Image::Format format) const {
	static Map<Image::Format, String> extentions;
	if (extentions.empty()) {
		extentions[Image::FORMAT_BPTC_RGBA] = "png";
	}
	if (extentions.has(format))
		return extentions[format];
	return "img";
}

void PageExporter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("export_pages", "p_pages", "p_config"), &PageExporter::export_pages);
	ClassDB::bind_method(D_METHOD("get_page_description", "p_page", "p_congih"), &PageExporter::get_page_description);
}

Error PageExporter::export_pages(const Array &pages, Ref<TexturePackerConfig> config) const {
	ERR_FAIL_COND_V( !pages.size() || config.is_null(), ERR_INVALID_PARAMETER);
	for (int i = 0; i < pages.size(); i++) {
		Ref<PackedPage> p = pages[i];
		ERR_FAIL_COND_V(p.is_null(), ERR_INVALID_PARAMETER);

		String img_file = config->get_save_image() + "." + get_image_extention(config->get_save_format());
		Error err = save_image_file(p->get_image(), config->get_save_format(), img_file);
		ERR_FAIL_COND_V(err != OK, err);

		String meta_file = config->get_save_meta() + "." + get_meta_extention();
		err = save_text_file(get_page_description(p, config, img_file), meta_file);
		ERR_FAIL_COND_V(err != OK, err);
	}
	return OK;
}

String GenericXMLExporter::get_page_description(Ref<PackedPage> page, Ref<TexturePackerConfig> config, const String &image_file) const {
	static const String FILE_TEMPLATE =
R"(<?xml version="1.0" encoding="UTF-8"?>
<TextureAtlas imagePath="{image_file}" width="{width}" height="{height}">
{items}
</TextureAtlas>
)";
	static const String ITEM_TEMPLATE =R"(    <sprite n="{atlas}" x="{x}" y="{y}" w="{w}" h="{h}" />)";
	String items;
	Array keys = page->get_path_region_map().keys();
	for (int i=0; i<keys.size(); i++) {
		String path = keys[i];
		Rect2 region = page->get_path_region_map()[keys[i]];
		Dictionary dict;
		dict["atlas"] = path.get_file();
		dict["x"] = itos(int(region.position.x));
		dict["y"] = itos(int(region.position.y));
		dict["w"] = itos(int(region.size.y));
		dict["h"] = itos(int(region.size.y));
		items += ITEM_TEMPLATE.format(dict);
		if (i < keys.size() -1) {
			items += "\n";
		}
	}
	Dictionary dict;
	dict["image_file"] = image_file;
	dict["width"] = page->get_image()->get_size().x;
	dict["height"] = page->get_image()->get_size().y;
	return FILE_TEMPLATE.format(dict).replace("{items}", items);
}
