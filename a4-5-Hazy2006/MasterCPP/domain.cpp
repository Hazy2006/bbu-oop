#include "domain.h"

/// <summary>
/// Declare Duration with minutes and seconds for tutorial
/// </summary>
/// <param name="minutes"></param>
/// <param name="seconds"></param>
Duration::Duration(int minutes, int seconds)
{
	this->minutes = minutes;
	this->seconds = seconds;
}

/// <summary>
/// Declare traits (parameters) for each tutorial 
/// </summary>
/// <param name="title"></param>
/// <param name="presenter"></param>
/// <param name="minutes"></param>
/// <param name="seconds"></param>
/// <param name="likes"></param>
/// <param name="link"></param>
Tutorial::Tutorial(const std::string& title, const std::string& presenter, int minutes, int seconds, int likes, const std::string& link)
{
	this->title = title;
	this->presenter = presenter;
	Duration d{ minutes,seconds };
	this->duration = d;
	this->likes = likes;
	this->link = link;
}

/// <summary>
/// Function to change likes
/// </summary>
void Tutorial::incrementLikes() {
	likes++;
}
/// <summary>
/// Operator to check the uniqueness of a tutorial (by title and presenter)
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
bool Tutorial::operator==(const Tutorial& other) const {
	return this->title == other.title && this->presenter == other.presenter;
}