#include "service.h"

Service::Service(Repo& r) : r(r) {}
/// <summary>
/// Add a tutorial to the list
/// </summary>
/// <param name="title"></param>
/// <param name="presenter"></param>
/// <param name="minutes"></param>
/// <param name="seconds"></param>
/// <param name="likes"></param>
/// <param name="link"></param>
void Service::add(const std::string& title, const std::string& presenter, int minutes, int seconds, int likes, const std::string& link)
{
	if (title.empty()) throw std::exception("Title cannot be empty!");
	if (presenter.empty()) throw std::exception("Presenter cannot be empty!");
	if (minutes < 0) throw std::exception("Minutes cannot be negative!");
	if (seconds < 0 || seconds > 59) throw std::exception("Seconds must be between 0 and 59!");
	if (likes < 0) throw std::exception("Likes cannot be negative!");
	if (link.empty()) throw std::exception("Link cannot be empty!");
	Tutorial t{ title,presenter,minutes,seconds,likes,link };
	this->r.add(t);
}

/// <summary>
/// return number of tutorials to work eventually on stack (when adding/removing)
/// </summary>
/// <returns></returns>
int Service::length() const
{
	return this->r.length();
}

/// <summary>
/// Remove an existing title, raise errors if nothing found
/// </summary>
/// <param name="title"></param>
/// <param name="presenter"></param>
void Service::remove(const std::string& title, const std::string& presenter)
{
	if (title.empty()) throw std::exception("Title cannot be empty!");
	if (presenter.empty()) throw std::exception("Presenter cannot be empty!");
	Tutorial dummy{ title, presenter };
	int index = this->r.search(dummy);
	if (index == -1) throw std::exception("Tutorial not found!");
	this->r.remove(index);
}

/// <summary>
/// Update an existing tutorial, search if exists first and if not throw exception, otherwise update
/// </summary>
/// <param name="title"></param>
/// <param name="presenter"></param>
/// <param name="newTitle"></param>
/// <param name="newPresenter"></param>
/// <param name="newMinutes"></param>
/// <param name="newSeconds"></param>
/// <param name="newLink"></param>
void Service::update(const std::string& title, const std::string& presenter, const std::string& newTitle, const std::string& newPresenter, int newMinutes, int newSeconds, const std::string& newLink)
{
	if (title.empty()) throw std::exception("Title to find cannot be empty!");
	if (presenter.empty()) throw std::exception("Presenter to find cannot be empty!");
	if (newTitle.empty()) throw std::exception("New title cannot be empty!");
	if (newPresenter.empty()) throw std::exception("New presenter cannot be empty!");
	if (newMinutes < 0) throw std::exception("Minutes cannot be negative!");
	if (newSeconds < 0 || newSeconds > 59) throw std::exception("Seconds must be between 0 and 59!");
	if (newLink.empty()) throw std::exception("New link cannot be empty!");

	Tutorial dummy{ title, presenter };
	int index = this->r.search(dummy);
	if (index == -1) throw std::exception("Tutorial not found!");

	Tutorial existing = this->r.get_all().get(index);
	int preservedLikes = existing.getLikes();

	Tutorial updated{ newTitle, newPresenter, newMinutes, newSeconds, preservedLikes, newLink };
	this->r.update(updated, index);
}
void Service::find(const std::string& title, const std::string& presenter)
{
	Tutorial dummy{ title, presenter };
	int index = this->r.search(dummy);
	if (index == -1) throw std::exception("Tutorial not found!");
}
/// <summary>
/// getAll function in order to display the list of tutorials
/// </summary>
/// <returns></returns>
DynamicVector<Tutorial> Service::get_all() const
{
	return this->r.get_all();
}

DynamicVector<Tutorial> Service::getByPresenter(const std::string& presenter) const
{
	DynamicVector<Tutorial> all = this->r.get_all();
	if (presenter.empty()) return all;
	DynamicVector<Tutorial> result;
	for (int i = 0; i < all.length(); i++)
	{
		Tutorial t = all.get(i);
		if (t.getPresenter() == presenter)
			result.add(t);
	}
	return result;
}

void Service::incrementLikes(const std::string& title, const std::string& presenter)
{
	Tutorial dummy{ title, presenter };
	int index = this->r.search(dummy);
	if (index == -1) throw std::exception("Tutorial not found!");
	Tutorial t = this->r.get_all().get(index);
	t.incrementLikes();
	this->r.update(t, index);
}


