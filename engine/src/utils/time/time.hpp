#pragma once

namespace gloria::util {
	class Time {
	public:
		Time() {}

		Time(float t) : mCurrentTime(t) {}

		~Time() {}

		operator float() const { return mCurrentTime; };

		float seconds() const { return mCurrentTime; };

		float milliseconds() const { return mCurrentTime * 1000.0f; };

		float minutes() const { return mCurrentTime * 1000.0f * 60.0f; };

		float deltatime() const { return seconds(); };
	private:
		float mCurrentTime{ 0.0f };
	};
}