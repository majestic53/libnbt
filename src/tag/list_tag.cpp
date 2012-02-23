/*
 * list_tag.cpp
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
#include "list_tag.hpp"

/*
 * List tag assignment
 */
list_tag &list_tag::operator=(const list_tag &other) {

	// check for self
	if(this == &other)
		return *this;

	// set attributes
	generic_tag::operator =(other);
	value.assign(other.value.begin(), other.value.end());
	return *this;
}

/*
 * List tag equals
 */
bool list_tag::operator==(const list_tag &other) {

	// check for self
	if(this == &other)
		return true;

	// check attributes
	if(generic_tag::operator !=(other)
			|| value.size() == other.value.size())
		return false;
	for(unsigned int i = 0; i < value.size(); ++i)
		if(value.at(i) != other.value.at(i))
			return false;
	return true;
}

/*
 * Returns a tag at a given index in a list tag
 */
generic_tag *list_tag::at(unsigned int index) {
	if(index >= value.size())
		return 0;
	return value.at(index);
}

/*
 * Returns a string representation of a list tag
 */
std::string list_tag::to_string(void) {
	std::stringstream ss;

	// create string representation
	ss << generic_tag::type_to_string(type);
	if(!name.empty())
		ss << " " << name << " (" << value.size() << ")";
	if(!value.empty()) {
		ss << " {" << std::endl;
		for(unsigned int i = 0; i < value.size(); ++i)
			ss << "\t" << value.at(i)->to_string() << std::endl;
		ss << "}";
	}
	return ss.str();
}