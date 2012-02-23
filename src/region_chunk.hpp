/*
 * region_chunk.cpp
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

#ifndef REGION_CHUNK_HPP_
#define REGION_CHUNK_HPP_

#include <ctime>
#include <string>

class region_chunk {
private:

	/*
	 * Chunk compression type
	 */
	unsigned int type;

	/*
	 * Chunk size, position
	 */
	size_t size, pos;

	/*
	 * Chunk last modified time
	 */
	time_t modified;

public:

	/*
	 * Compression types
	 */
	enum COMPRESSION { UNDEFINED, GZIP, ZLIB };

	/*
	 * Region chunk constructor
	 */
	region_chunk(void);

	/*
	 * Region chunk constructor
	 */
	region_chunk(const region_chunk &other);

	/*
	 * Region chunk constructor
	 */
	region_chunk(unsigned int type, size_t size, size_t pos, time_t modified) : type(type), size(size), pos(pos), modified(modified) { return; }

	/*
	 * Region chunk destructor
	 */
	virtual ~region_chunk(void) { return; }

	/*
	 * Region chunk assignment
	 */
	region_chunk &operator=(const region_chunk &other);

	/*
	 * Region chunk equals
	 */
	bool operator==(const region_chunk &other);


	/*
	 * Region chunk not equals
	 */
	bool operator!=(const region_chunk &other) { return !(*this == other); }

	/*
	 * Returns a region chunks last modified time
	 */
	time_t get_modified(void) { return modified; }

	/*
	 * Returns a region chunks position
	 */
	size_t get_position(void) { return pos; }

	/*
	 * Returns a region chunks size
	 */
	size_t get_size(void) { return size; }

	/*
	 * Returns a region chunks type
	 */
	unsigned int get_type(void) { return type; }

	/*
	 * Sets a region chunks last modified time
	 */
	void set_modified(time_t modified) { this->modified = modified; }

	/*
	 * Sets a region chunks position
	 */
	void set_position(size_t pos) { this->pos = pos; }

	/*
	 * Sets a region chunks size
	 */
	void set_size(size_t size) { this->size = size; }

	/*
	 * Sets a region chunks type
	 */
	void set_type(unsigned int type) { this->type = type; }

	/*
	 * Returns a string representation of a region chunk
	 */
	std::string to_string(void);
};

#endif
