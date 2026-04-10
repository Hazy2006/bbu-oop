#pragma once
#include "repository.h"

class Service
{
private:
	Repo& r;
public:
	Service(Repo& r);
	void add(const std::string& title = "", const std::string& presenter = "", int minutes = 0, int seconds = 0, int likes = 0, const std::string& link = "");
	int length() const;
	void remove(const std::string& title, const std::string& presenter);
	void update(const std::string& title, const std::string& presenter, const std::string& newTitle, const std::string& newPresenter, int newMinutes, int newSeconds, const std::string& newLink);
	void find(const std::string& title, const std::string& presenter);
	DynamicVector<Tutorial> get_all() const;
	DynamicVector<Tutorial> getByPresenter(const std::string& presenter) const;
	void incrementLikes(const std::string& title, const std::string& presenter);
};