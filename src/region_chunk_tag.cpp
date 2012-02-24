/*
 * region_chunk_tag.cpp
 * Copyright (C) 2012 David Jolly
 * ----------------------
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "region_chunk_tag.hpp"

/*
 * Region chunk tag assignment
 */
region_chunk_tag &region_chunk_tag::operator=(const region_chunk_tag &other) {

	// check for self
	if(this == &other)
		return *this;

	// set attributes
	cleanup(root);
	root = other.root;
	return *this;
}

/*
 * Region chunk tag equals
 */
bool region_chunk_tag::operator==(const region_chunk_tag &other) {

	// check for self
	if(this == &other)
		return true;

	// check attributes
	return root == other.root;
}

/*
 * Cleanup a series of tags
 */
void region_chunk_tag::cleanup(generic_tag *tag) {
	compound_tag *cmp_tag = NULL;
	list_tag *lst_tag = NULL;

	// cleanup each tag based off its type
	switch(tag->get_type()) {
		case generic_tag::COMPOUND:
			cmp_tag = dynamic_cast<compound_tag *>(tag);
			for(unsigned int i = 0; i < cmp_tag->size(); ++i)
				cleanup(cmp_tag->at(i));
			cmp_tag->value.clear();
			delete cmp_tag;
			break;
		case generic_tag::LIST:
			lst_tag = dynamic_cast<list_tag *>(tag);
			for(unsigned int i = 0; i < lst_tag->size(); ++i)
				cleanup(lst_tag->at(i));
			lst_tag->value.clear();
			delete lst_tag;
			break;
		default: delete tag;
			break;
	}
}

/*
 * Return a region chunk tag tag at a given name
 */
generic_tag *region_chunk_tag::get_tag_by_name(const std::string &name) {

	// TODO: Add ability to return a region subtag at a given name

	return NULL;
}

/*
 * Set region chunk tag root tag
 */
void region_chunk_tag::set_root_tag(generic_tag *root) {
	cleanup(this->root);
	this->root = root;
}

/*
 * Returns a string representation of a region chunk tag
 */
std::string region_chunk_tag::to_string(void) {
	if(!root)
		return "Empty";
	return root->to_string();
}
