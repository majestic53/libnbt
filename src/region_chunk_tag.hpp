/*
 * region_chunk_tag.hpp
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

#ifndef REGION_CHUNK_TAG_HPP_
#define REGION_CHUNK_TAG_HPP_

#include "tag/compound_tag.hpp"
#include "tag/generic_tag.hpp"
#include "tag/list_tag.hpp"

class region_chunk_tag {
private:

	/*
	 * Root region chunk tag
	 */
	generic_tag *root;

	/*
	 * Get tag by name helper
	 */
	void get_tag_by_name_helper(const std::string name, generic_tag *root, generic_tag *&tag);

public:

	/*
	 * Region chunk tag constructor
	 */
	region_chunk_tag(void) : root(NULL) { return; }

	/*
	 * Region chunk tag constructor
	 */
	region_chunk_tag(const region_chunk_tag &other) : root(other.root) { return; }

	/*
	 * Region chunk tag destructor
	 */
	virtual ~region_chunk_tag(void) { cleanup(root); }

	/*
	 * Region chunk tag assignment
	 */
	region_chunk_tag &operator=(const region_chunk_tag &other);

	/*
	 * Region chunk tag equals
	 */
	bool operator==(const region_chunk_tag &other);

	/*
	 * Region chunk tag not equals
	 */
	bool operator!=(const region_chunk_tag &other) { return !(*this == other); }

	/*
	 * Cleanup a series of tags
	 */
	static void cleanup(generic_tag *tag);

	/*
	 * Region chunk tag empty status
	 */
	bool empty(void) { return !root; }

	/*
	 * Return region chunk tag root tag
	 */
	generic_tag *&get_root_tag(void) { return root; }

	/*
	 * Return a region chunk tag tag at a given name
	 */
	bool get_tag_by_name(const std::string &name, region_chunk_tag &tag);

	/*
	 * Set region chunk tag root tag
	 */
	void set_root_tag(generic_tag *root);

	/*
	 * Returns a string representation of a region chunk tag
	 */
	std::string to_string(void);
};

#endif
