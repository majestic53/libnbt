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

	// TODO: ADD FUNCTIONS TO HANDLE READING BLOCKS, ETC.

	/*
	 * Returns a string representation of a region file reader
	 */
	std::string to_string(void);
};

#endif
