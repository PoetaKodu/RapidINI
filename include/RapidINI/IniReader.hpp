#pragma once

#include <string>
#include <map>
#include <locale>
#include <cctype>
#include <algorithm>

namespace rapid_ini
{
	
// Header part:

class IniReader
{
	enum class State
	{
		ReadingSectionName,
		ReadingKeyName,
		ReadingKeyValue,
		ReadingComment,
		Unknown
	};

public:
	using ParseReturnType = std::map<std::string, std::string>;

	static ParseReturnType parse(char const* data_, std::size_t numberOfCharacters_)
	{
		using SizeType = std::string::size_type;

		ParseReturnType result;
		State state = State::Unknown;

		std::string currentSection;

		std::string sectionName;
		std::string keyName;
		std::string keyValue;

		bool valueHasOnlySpaces = true;

		char const* end = data_ + numberOfCharacters_;
		for (char const* it = data_; it != end; it++)
		{
			// Skip the '\r' character.
			if (*it == '\r')
				continue;

			// Handle new line character:
			if (*it == '\n')
			{
				handleLineEnd(result, state, currentSection, sectionName, keyName, keyValue, valueHasOnlySpaces);
				continue;
			}

			// Handle line start:
			if (state == State::Unknown)
			{
				// Skip whitespaces in the front:
				if (std::isspace(*it))
					continue;

				if (*it == ';')
					state = State::ReadingComment;
				else if (*it == '[')
				{
					state = State::ReadingSectionName;
					sectionName.clear();
					keyName += *it;
				}
				else
				{
					state = State::ReadingKeyName;
					keyName.clear();
					keyName += *it;
				}
			}
			else if (state == State::ReadingSectionName)
			{
				if (*it == ']')
				{
					state = State::Unknown;
					currentSection = sectionName;
				}
				else
					sectionName += *it;
			}
			else if (state == State::ReadingKeyName)
			{
				if (*it == '=')
				{
					// Start reading value:
					state = State::ReadingKeyValue;
					keyValue.clear();
					valueHasOnlySpaces = true;
				}
				else
					keyName += *it;
			}
			else if (state == State::ReadingKeyValue)
			{
				if (!std::isspace(*it))
					valueHasOnlySpaces = false;

				keyValue += *it;
			}
		}

		handleLineEnd(result, state, currentSection, sectionName, keyName, keyValue, valueHasOnlySpaces);

		return result;
	}

	static ParseReturnType parse(std::string contents_)
	{
		return parse(contents_.data(), contents_.length());
	}

private:

	static void handleLineEnd(ParseReturnType& result_, State & state_, std::string & currentSection_, std::string const & sectionName_,
		std::string const & keyName_, std::string const & keyValue_, bool valueHasOnlySpaces_)
	{
		std::string prefix = currentSection_.empty()
			? ""
			: currentSection_ + '.';

		if (state_ == State::ReadingKeyValue)
		{
			result_[prefix + keyName_] = valueHasOnlySpaces_ ? "" : keyValue_;
		}
		else if (state_ == State::ReadingKeyName)
		{
			// Line end while reading key name? Ignore that.
		}
		else if (state_ == State::ReadingSectionName)
		{
			currentSection_ = sectionName_;
		}

		state_ = State::Unknown;
	}
};


// Source part:


}