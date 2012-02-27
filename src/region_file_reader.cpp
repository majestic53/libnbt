/*
 * region_file_reader.cpp
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

#include <sstream>
#include "region_chunk_info.hpp"
#include "region_file_reader.hpp"

/*
 * Region file reader constructor
 */
region_file_reader::region_file_reader(void) : fill_count(0) {

	// assign attribute values
	for(unsigned int i = 0; i < region_file::CHUNK_COUNT; ++i)
		fill[i] = false;
}

/*
 * Region file reader constructor
 */
region_file_reader::region_file_reader(const region_file_reader &other): fill_count(other.fill_count), path(other.path) {

	// assign attribute values
	for(unsigned int i = 0; i < region_file::CHUNK_COUNT; ++i) {
		data[i] = other.data[i];
		fill[i] = other.fill[i];
	}
}

/*
 * Region file reader constructor
 */
region_file_reader::region_file_reader(const std::string &path) : path(path) {
	unsigned int pos;
	region_chunk_info info;
	region_file file;

	// open file at path
	file = region_file(path);
	fill_count = 0;

	// assign attribute values
	for(unsigned int i = 0; i < region_file::REGION_SIZE; ++i)
		for(unsigned int j = 0; j < region_file::REGION_SIZE; ++j) {
			pos = i * region_file::REGION_SIZE + j;
			file.get_chunk_info(j, i, info);
			if(!info.get_position())
				continue;
			file.get_chunk_tag(j, i, data[pos]);
			fill[pos] = true;
			fill_count++;
		}
}

/*
 * Region file reader assignment
 */
region_file_reader &region_file_reader::operator=(const region_file_reader &other) {

	// check for self
	if(this == &other)
		return *this;

	// assign all attributes
	fill_count = other.fill_count;
	path = other.path;
	for(unsigned int i = 0; i < region_file::CHUNK_COUNT; ++i) {
		fill[i] = other.fill[i];
		data[i] = other.data[i];
	}
	return *this;
}

/*
 * Region file reader equals
 */
bool region_file_reader::operator==(const region_file_reader &other) {

	// check for self
	if(this == &other)
		return true;

	// check attributes
	if(fill_count != other.fill_count
			|| path != other.path)
		return false;
	for(unsigned int i = 0; i < region_file::CHUNK_COUNT; ++i)
		if(data[i] != other.data[i]
		                         || fill[i] != other.fill[i])
			return false;
	return true;
}

/*
 * Returns a chunk tag in a region file reader at a given x, z coord
 */
region_chunk_tag &get_chunk_tag_at(unsigned int x, unsigned int z) {

	// TODO

}

/*
 * Returns fill status of a chunk in a region file reader at a given x, z coord
 */
bool is_filled(unsigned int x, unsigned int z) {

	// TODO

}

/*
 * Returns a string representation of a region file reader
 */
std::string region_file_reader::to_string(void) {
	std::stringstream ss;

	// form string representation
	ss << "[READER] filled: " << fill_count << "/" << region_file::CHUNK_COUNT;
	if(!path.empty())
		ss << ", path: " << path;
	return ss.str();
}
