/*************************************************************************/
/*  register_types.cpp                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include <core/os/os.h>
#include <core/class_db.h>
#include "texture_packer.h"
#include <core/os/dir_access.h>
#include "page_exporter.h"

void register_TexturePacker_types() {

    ClassDB::register_class<TexturePackerConfig>();
    ClassDB::register_class<TexturePacker>();
    ClassDB::register_class<PackedPage>();
    ClassDB::register_virtual_class<PageExporter>();
    ClassDB::register_class<GenericXMLExporter>();
#if 0
    PoolStringArray files;
    List<String> argv = OS::get_singleton()->get_cmdline_args();
    for(List<String>::Element * E = argv.front(); E; E = E->next()) {

		Error err;
		DirAccessRef dir = DirAccess::open(E->get(), &err);

		if (dir) {
			dir->list_dir_begin();
			String path;
			bool isdir;
			path = dir->get_next(&isdir);
			while (path != String()) {
				if (!isdir && (path.ends_with(".png") || path.ends_with(".jpg"))) {
					String file = E->get();
					file += "/";
					file += path;
					files.append(file);
				}
				path = dir->get_next(&isdir);
			}
			dir->list_dir_end();
		} else {
			files.append(E->get());
		}
    }

    TexturePacker tp;
    tp.get_pack_config()->set_image_files(files);
    if (OK == tp.pack()) {
        Array pages = tp.get_packed_pages();
        GenericXMLExporter exporter;
        ERR_FAIL_COND( OK != exporter.export_pages(pages, tp.get_pack_config()) );
    }
#endif
}

void unregister_TexturePacker_types() {

}
