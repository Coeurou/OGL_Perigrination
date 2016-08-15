#pragma once

class Uncopyable
{
protected:
	Uncopyable() {}
	~Uncopyable() {}

    Uncopyable(const Uncopyable&) = delete;
    Uncopyable& operator=(const Uncopyable&) = delete;
};