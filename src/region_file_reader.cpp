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
#include "tag/byte_array_tag.hpp"
#include "tag/int_tag.hpp"

/*
 * Supported tags
 */
const std::string region_file_reader::TAGS[TAG_COUNT] = {
		"Blocks",
		"HeightMap",
		"xPos",
		"zPos",
};

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
			//file.get_chunk_tag(j, i, data[pos]);
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
 * Returns a chunk tag blocks array at a given x, z coord
 */
bool region_file_reader::get_chunk_blocks_at(unsigned int x, unsigned int z, std::vector<int8_t> &value) {
	byte_array_tag *tag = get_chunk_value_at<byte_array_tag>(x, z, TAGS[BLOCKS]);
	if(!tag)
		return false;
	value = tag->value;
	return true;
}

/*
 * Returns a chunk tag height array at a given x, z coord
 */
bool region_file_reader::get_chunk_heights_at(unsigned int x, unsigned int z, std::vector<int8_t> &value) {
	byte_array_tag *tag = get_chunk_value_at<byte_array_tag>(x, z, TAGS[HEIGHTS]);
	if(!tag)
		return false;
	value = tag->value;
	return true;
}

/*
 * Returns a chunk tag x position at a given x, z coord
 */
bool region_file_reader::get_chunk_x_pos_at(unsigned int x, unsigned int z, int32_t &value) {
	int_tag *tag = get_chunk_value_at<int_tag>(x, z, TAGS[XPOS]);
	if(!tag)
		return false;
	value = tag->value;
	return true;
}

/*
 * Returns a chunk tag z position at a given x, z coord
 */
bool region_file_reader::get_chunk_z_pos_at(unsigned int x, unsigned int z, int32_t &value) {
	int_tag *tag = get_chunk_value_at<int_tag>(x, z, TAGS[ZPOS]);
	if(!tag)
		return false;
	value = tag->value;
	return true;
}

/*
 * Returns a chunk tag at a given x, z coord
 */
region_chunk_tag &region_file_reader::get_chunk_tag_at(unsigned int x, unsigned int z) {
	unsigned int pos = z * region_file::REGION_SIZE + x;

	// check if x, z coord are out-of-bounds
	if(pos >= region_file::CHUNK_COUNT) {
		unsigned int coord[] = {x, z};
		std::vector<unsigned int> coord_vec(coord, coord + 2);
		throw region_file_exc(region_file_exc::OUT_OF_BOUNDS, coord_vec);
	}
	return data[pos];
}

/*
 * Returns fill status at a given x, z coord
 */
bool region_file_reader::is_filled(unsigned int x, unsigned int z) {
	unsigned int pos = z * region_file::REGION_SIZE + x;

	// check if x, z coord are out-of-bounds
	if(pos >= region_file::CHUNK_COUNT) {
		unsigned int coord[] = {x, z};
		std::vector<unsigned int> coord_vec(coord, coord + 2);
		throw region_file_exc(region_file_exc::OUT_OF_BOUNDS, coord_vec);
	}
	return fill[pos];
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
