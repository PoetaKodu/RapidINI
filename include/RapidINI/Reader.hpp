#pragma once

#include <string>
#include <map>
#include <locale>
#include <cctype>
#include <algorithm>

namespace rapid_ini
{

/// <summary>
/// Provides static method `read` that parses string containing contents of the INI file.
/// </summary>
class Reader
{
	/// <summary>
	/// Enumeration of all possible INI reader algorithm states.
	/// </summary>
	enum class State
	{
		ReadingSectionName,
		ReadingKeyName,
		ReadingKeyValue,
		ReadingComment,
		Unknown
	};

public:
	/// <summary>
	/// Type of the `read` method result.
	/// </summary>
	using ReadResultType = std::map<std::string, std::string>;

	/// <summary>
	/// Reads INI-formatted string and returns map of read properties.
	/// </summary>
	/// <param name="data_">Pointer to string's first character</param>
	/// <param name="numberOfCharacters_">Number of characters in the string (without the null terminator).</param>
	/// <returns>
	/// Container that maps every key name (prefixed with its section name) to it's value.
	/// </returns>
	static ReadResultType read(char const* data_, std::size_t numberOfCharacters_)
	{
		
		// Types and settings:
		using SizeType = std::string::size_type;

		// Memory efficiency settings:
		constexpr int sectionNamePreReservedBytes	= 64;
		constexpr int keyNamePreReservedBytes		= 64;
		constexpr int keyValuePreReservedBytes		= 256;

		ReadResultType result;
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
					sectionName.reserve(sectionNamePreReservedBytes);
				}
				else
				{
					state = State::ReadingKeyName;
					keyName.clear();
					keyName.reserve(keyNamePreReservedBytes);

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
					keyValue.reserve(keyValuePreReservedBytes);

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

	/// <summary>
	/// Reads INI-formatted string and returns map of read properties.
	/// </summary>
	/// <param name="contents">INI-formatted string to read from.</param>
	/// <returns>
	/// Container that maps every key name (prefixed with its section name) to it's value.
	/// </returns>
	static ReadResultType read(std::string const & contents_)
	{
		return read(contents_.data(), contents_.length());
	}

private:

	/// <summary>
	/// Handles the line end while read algorithm is working. Written to avoid unnecessary repeating code.
	/// </summary>
	static void handleLineEnd(ReadResultType& result_, State & state_, std::string & currentSection_, std::string const & sectionName_,
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

}