#pragma once
#include <string>

class Duration
{
private:
	int minutes;
	int seconds;
public:
	Duration(int minutes = 0, int seconds = 0);
	int getMinutes() const {
		return minutes;
	}
	int getSeconds() const {
		return seconds;
	}
};

class Tutorial 
{
private:
	std::string title;
	std::string presenter;
	Duration duration;
	int likes;
	std::string link;
public:
	Tutorial(const std::string& title = "", const std::string& presenter = "", int minutes = 0, int seconds = 0, int likes = 0, const std::string& link = "");
	void setTitle(const std::string& t) { title = t; }
	void setPresenter(const std::string& p) { presenter = p; }
	void setDuration(const Duration& d) { duration = d; }
	void setLink(const std::string& l) { link = l; }
	std::string getTitle() const { return title; }
	std::string getPresenter() const { return presenter; }
	std::string getLink() const { return link; }
	Duration getDuration() const { return duration; }
	int getLikes() const { return likes; }
	void incrementLikes();
	bool operator==(const Tutorial& other) const;
};