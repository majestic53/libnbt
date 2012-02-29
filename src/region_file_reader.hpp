/*
 * region_file_reader.hpp
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

#ifndef REGION_FILE_READER_HPP_
#define REGION_FILE_READER_HPP_

#include <cstdint>
#include <string>
#include <vector>
#include "region_chunk_tag.hpp"
#include "region_file.hpp"
#include "region_file_exc.hpp"

class region_file_reader {
private:

	/*
	 * Chunk tag data
	 */
	region_chunk_tag data[region_file::CHUNK_COUNT];

	/*
	 * Chunk fill count
	 */
	unsigned int fill_count;

	/*
	 * Chunk fill status
	 */
	bool fill[region_file::CHUNK_COUNT];

	/*
	 * Region file path
	 */
	std::string path;

	/*
	 * Supported tags
	 */
	enum TAG_NAME { BLOCKS, HEIGHTS, XPOS, ZPOS, };
	static const std::string TAGS[];
	static const unsigned int TAG_COUNT = 4;

	/*
	 * Returns a chunk tag value at a given x, z coord
	 */
	template <class T>
	T *get_chunk_value_at(unsigned int x, unsigned int z, const std::string &name) {
		unsigned int pos = z * region_file::REGION_SIZE + x;

		// check if x, z coord are out-of-bounds
		if(pos >= region_file::CHUNK_COUNT) {
			unsigned int coord[] = {x, z};
			std::vector<unsigned int> coord_vec(coord, coord + 2);
			throw region_file_exc(region_file_exc::OUT_OF_BOUNDS, coord_vec);
		}

		// cache tag data if cache-miss occurs
		if(fill[pos]
		        && data[pos].empty()) {
			region_file file(path);
			file.get_chunk_tag(x, z, data[pos]);
		}
		return dynamic_cast<T *>(data[pos].get_tag_by_name(name));
	}

public:

	/*
	 * Region file reader constructor
	 */
	region_file_reader(void);

	/*
	 * Region file reader constructor
	 */
	region_file_reader(const region_file_reader &other);

	/*
	 * Region file reader constructor
	 */
	region_file_reader(const std::string &path);

	/*
	 * Region file reader destructor
	 */
	virtual ~region_file_reader(void) { return; }

	/*
	 * Region file reader assignment
	 */
	region_file_reader &operator=(const region_file_reader &other);

	/*
	 * Region file reader equals
	 */
	bool operator==(const region_file_reader &other);

	/*
	 * Region file reader not equals
	 */
	bool operator!=(const region_file_reader &other) { return !(*this == other); }

	/*
	 * Returns a chunk tag blocks array at a given x, z coord
	 */
	bool get_chunk_blocks_at(unsigned int x, unsigned int z, std::vector<int8_t> &value);

	/*
	 * Returns a chunk tag height array at a given x, z coord
	 */
	bool get_chunk_heights_at(unsigned int x, unsigned int z, std::vector<int8_t> &value);

	/*
	 * Returns a chunk tag x position at a given x, z coord
	 */
	bool get_chunk_x_pos_at(unsigned int x, unsigned int z, int32_t &value);

	/*
	 * Returns a chunk tag z position at a given x, z coord
	 */
	bool get_chunk_z_pos_at(unsigned int x, unsigned int z, int32_t &value);

	/*
	 * Returns a chunk tag at a given x, z coord
	 */
	region_chunk_tag &get_chunk_tag_at(unsigned int x, unsigned int z);

	/*
	 * Returns a region file readers fill count
	 */
	unsigned int get_fill_count(void) { return fill_count; }

	/*
	 * Returns a region file readers path
	 */
	std::string get_path(void) { return path; }

	/*
	 * Returns fill status at a given x, z coord
	 */
	bool is_filled(unsigned int x, unsigned int z);

	/*
	 * Returns a string representation of a region file reader
	 */
	std::string to_string(void);
};

#endif
