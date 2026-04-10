#include <stdexcept>
#include "repository.h"

/// <summary>
/// Add data 
/// </summary>
/// <param name="t"></param>
void Repo::add(const Tutorial& t)
{
	if (this->v.search(t) != -1)
		throw std::exception("Tutorial already exists!");
	this->v.add(t);
}

/// <summary>
/// Prepare length
/// </summary>
/// <returns></returns>
int Repo::length() const
{
	return this->v.length();
}

/// <summary>
/// Remove from data package
/// </summary>
/// <param name="index"></param>
void Repo::remove(int index)
{
	this->v.remove(index);
}

/// <summary>
/// Update existing data
/// </summary>
/// <param name="element"></param>
/// <param name="index"></param>
void Repo::update(Tutorial element, int index) {
	this->v.update(element, index);
}

/// <summary>
/// Display
/// </summary>
/// <returns></returns>
DynamicVector<Tutorial> Repo::get_all() const
{
	return this->v;
}

/// <summary>
/// Search into the data package
/// </summary>
/// <param name="t"></param>
/// <returns></returns>
int Repo::search(const Tutorial& t)
{
	return this->v.search(t);
}
