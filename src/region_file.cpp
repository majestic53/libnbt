/*
 * region_file.cpp
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
#include <zlib.h>
#include "region_file.hpp"

/*
 * Region file pattern
 */
const boost::regex region_file::PATTERN = boost::regex("r\\.([-]?[0-9]+)\\.([-]?[0-9]+)\\.mcr");

/*
 * Region file constructor
 */
region_file::region_file(void) : filled(0), x(0), z(0) {
	chunks = new region_chunk[CHUNK_COUNT];
	if(!chunks)
		throw region_file_exc(region_file_exc::ALLOC_FAIL);
}

/*
 * Region file constructor
 */
region_file::region_file(const region_file &other) : filled(other.filled), path(other.path), x(other.x), z(other.z) {
	chunks = new region_chunk[CHUNK_COUNT];
	if(!chunks)
		throw region_file_exc(region_file_exc::ALLOC_FAIL);

	// assign all attributes
	for(unsigned int i = 0; i < CHUNK_COUNT; ++i)
		chunks[i] = other.chunks[i];
}

/*
 * Region file constructor
 */
region_file::region_file(const std::string &path) : filled(0), path(path), x(0), z(0) {
	chunks = new region_chunk[CHUNK_COUNT];
	if(!chunks)
		throw region_file_exc(region_file_exc::ALLOC_FAIL);

	// parse the filename for coordinants
	boost::cmatch ref;
	std::stringstream stream;
	std::string name = path.substr(path.find_last_of('/') + 1);
	if(boost::regex_match(name.c_str(), ref, PATTERN)) {
		stream << ref[1];
		stream >> z;
		stream.clear();
		stream << ref[2];
		stream >> x;
	} else
		throw region_file_exc(region_file_exc::INVALID_PATH, path);

	// retrieve region file information
	read();
}

/*
 * Region file assignment
 */
region_file &region_file::operator=(const region_file &other) {

	// check for self
	if(this == &other)
		return *this;

	// assign attributes
	filled = other.filled;
	path = other.path;
	x = other.x;
	z = other.z;
	delete[] chunks;
	chunks = new region_chunk[CHUNK_COUNT];
	if(!chunks)
		throw region_file_exc(region_file_exc::ALLOC_FAIL);
	for(unsigned int i = 0; i < CHUNK_COUNT; ++i)
		chunks[i] = other.chunks[i];
	return *this;
}

/*
 * Region file equals
 */
bool region_file::operator==(const region_file &other) {

	// check for self
	if(this == &other)
		return true;

	// check attributes
	if(filled != other.filled
			|| path != other.path
			|| x != other.x
			|| z != other.z)
		return false;
	for(unsigned int i = 0; i < CHUNK_COUNT; ++i)
		if(chunks[i] != other.chunks[i])
			return false;
	return true;
}

/*
 * Returnd region chunk data at a given x, z coord
 */
int region_file::get_chunk_data(unsigned int x, unsigned int z, std::vector<char> &data) {

	// Check to see if (x, z) are out-of-bounds
	if(x + z * REGION_SIZE >= CHUNK_COUNT)
		throw region_file_exc(region_file_exc::OUT_OF_BOUNDS);

	// gather chunk info
	region_chunk chunk = chunks[x + z * REGION_SIZE];
	int comp_size = chunk.get_size();

	// check if chunk is empty
	if(chunk.get_position() == 0) {
		data.clear();
		return 0;
	}

	// open region file
	std::ifstream chunk_file;
	chunk_file.open(path.c_str(), std::ios::in | std::ios::binary);

	// check if file exists
	if(!chunk_file)
		throw region_file_exc(region_file_exc::INVALID_PATH, path);

	// read data from file
	std::vector<char> in_buff;
	char *comp_buff = new char[comp_size];
	chunk_file.seekg(chunk.get_position(), std::ios::beg);
	chunk_file.read(comp_buff, comp_size);
	in_buff.assign(comp_buff, comp_buff + comp_size);
	delete[] comp_buff;

	// close file
	chunk_file.close();

	// decompress data
	int size = 0;
	switch(chunk.get_type()) {
		case region_chunk::GZIP: throw region_file_exc(region_file_exc::UNSUPPORTED_COMPRESSION);
			break;
		case region_chunk::ZLIB: size = inflate_zlib(in_buff, data);
			break;
		default: throw region_file_exc(region_file_exc::UNKNOWN_COMPRESSION);
			break;
	}

	// return size of decompressed data
	return size;
}

/*
 * Returns region chunk information at a given x, z coord
 */
bool region_file::get_chunk_info(unsigned int x, unsigned int z, region_chunk &info) {

	// check if coord are out-of-bounds
	if(x + z * REGION_SIZE >= CHUNK_COUNT)
		return false;

	// assign chunk info
	info = region_chunk(chunks[x + z * REGION_SIZE]);
	return true;
}

/*
 * ZLib inflation routine
 */
int region_file::inflate_zlib(std::vector<char> &in, std::vector<char> &out) {
	z_stream str;
	int ret, pos = 0;

	// setup zlib object
	str.zalloc = Z_NULL;
	str.zfree = Z_NULL;
	str.opaque = Z_NULL;
	str.avail_in = 0;
	str.next_in = Z_NULL;

	// begin inflation
	ret = inflateInit(&str);
	do {
		int size = SEGMENT_SIZE;
		if((unsigned int) (in.size() - pos) < SEGMENT_SIZE)
			size = in.size() - pos;
		char *in_buff = new char[size];
		char *out_buff = new char[size];
		for(int i = 0; i < size; i++)
			in_buff[i] = in.at(pos + i);
		str.avail_in = size;
		str.next_in = (Bytef *) in_buff;
		do {
			str.avail_out = size;
			str.next_out = (Bytef *) out_buff;
			ret = inflate(&str, Z_NO_FLUSH);
			for(int i = 0; i < size; i++)
				out.push_back(out_buff[i]);
		} while(str.avail_out == 0);
		pos += size;
		delete[] in_buff;
		delete[] out_buff;
	} while(ret != Z_STREAM_END);

	// end inflation
	inflateEnd(&str);

	// return length of inflation & array of characters
	return out.size();
}

/*
 * Reads in a series of region chunks from a given path
 */
void region_file::read(const std::string &path) {
	int pos[CHUNK_COUNT];
	int time[CHUNK_COUNT];

	// open file at path
	file.open(path.c_str(), std::ios::in | std::ios::binary);

	// check if file exists
	if(!file)
		throw region_file_exc(region_file_exc::INVALID_PATH, path);

	// read in chunk positions & convert to little-endian
	for(unsigned int i = 0; i < CHUNK_COUNT; i++) {
		file.read(reinterpret_cast<char *>(&pos[i]), sizeof(int));
		convert_endian(pos[i]);

		// increment filled if chunk is not empty
		if(pos[i] != 0)
			filled++;
	}

	// read in chunk timestamps & covert to little-endian
	for(unsigned int i = 0; i < CHUNK_COUNT; i++) {
		file.read(reinterpret_cast<char *>(&time[i]), sizeof(int));
		convert_endian(time[i]);
	}

	// add chunks to array
	for(unsigned int i = 0; i < CHUNK_COUNT; i++) {
		int size = 0;
		char comp_type = 0;
		region_chunk chunk;

		// retrieve chunk size, compression type, & data if chunk exists
		if(pos[i] != 0) {
			file.seekg(0, std::ios::beg);
			file.seekg((pos[i] >> 8) * SECTOR_SIZE, std::ios::cur);
			file.read(reinterpret_cast<char *>(&size), sizeof(int));
			convert_endian(size);
			file.read(&comp_type, sizeof(char));
			pos[i] = file.tellg();
			chunk = region_chunk(comp_type, size, pos[i], time[i]);
		}

		// create all chunks
		chunks[i] = chunk;
	}

	// close file
	file.close();
}

/*
 * Returns a string representation of a region file
 */
std::string region_file::to_string(void) {
	std::stringstream ss;

	// form string representation
	ss << "[REGION] (" << x << ", " << z << "), filled: " << filled << "/" << CHUNK_COUNT << ", path: " << path;
	return ss.str();
}
