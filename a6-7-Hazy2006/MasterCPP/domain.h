#pragma once
#include <string>
#include <iostream>

class Duration {
private:
    int minutes;
    int seconds;
public:
    Duration(int minutes = 0, int seconds = 0) : minutes(minutes), seconds(seconds) {}
    int getMinutes() const { return minutes; }
    int getSeconds() const { return seconds; }
};

class Tutorial {
private:
    std::string title;
    std::string presenter;
    Duration duration;
    int likes;
    std::string link;
public:
    Tutorial(const std::string& title = "", const std::string& presenter = "",
        int minutes = 0, int seconds = 0, int likes = 0, const std::string& link = "")
        : title(title), presenter(presenter), duration(minutes, seconds), likes(likes), link(link) {
    }
    void setTitle(const std::string& t) { title = t; }
    void setPresenter(const std::string& p) { presenter = p; }
    void setDuration(const Duration& d) { duration = d; }
    void setLink(const std::string& l) { link = l; }
    std::string getTitle() const { return title; }
    std::string getPresenter() const { return presenter; }
    std::string getLink() const { return link; }
    Duration getDuration() const { return duration; }
    int getLikes() const { return likes; }
    void incrementLikes() { likes++; }
    bool operator==(const Tutorial& other) const {
        return title == other.title && presenter == other.presenter;
    }
    // format: title|presenter|minutes|seconds|likes|link
    friend std::ostream& operator<<(std::ostream& os, const Tutorial& t) {
        os << t.title << "|" << t.presenter << "|"
            << t.duration.getMinutes() << "|" << t.duration.getSeconds() << "|"
            << t.likes << "|" << t.link;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Tutorial& t) {
        std::string min, sec, lk;
        std::getline(is, t.title, '|');
        std::getline(is, t.presenter, '|');
        std::getline(is, min, '|');
        std::getline(is, sec, '|');
        std::getline(is, lk, '|');
        std::getline(is, t.link);
        if (!t.title.empty() && !min.empty() && !sec.empty() && !lk.empty()) {
            t.duration = Duration(std::stoi(min), std::stoi(sec));
            t.likes = std::stoi(lk);
        }
        return is;
    }
};