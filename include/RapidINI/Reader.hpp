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

	/// <summary>
	/// Determines whether specified character is a whitespace.
	/// </summary>
	template <typename TChar>
	static bool isSpace(TChar const & character_)
	{
		return character_ == ' ' || character_ == '\t';
	}
public:

	/// <summary>
	/// Type of the `read` method result.
	/// </summary>
	using ReadResultType = std::map<std::string, std::string>;

	/// <summary>
	/// Reads INI-formatted string and returns map of read properties.
	/// </summary>
	/// <param name="first_">Iterator to the first collection element.</param>
	/// <param name="last_">Iterator to the one-past-last collection element.</param>
	/// <returns>
	/// Container that maps every key name (prefixed with its section name) to it's value.
	/// </returns>
	template <typename TForwardIterator>
	static ReadResultType read(TForwardIterator first_, TForwardIterator last_)
	{
		ReadResultType result;
		State state = State::Unknown;

		std::string currentSection;
		std::string keyName;
		std::string keyValue;

		TForwardIterator sectionNameFirst = last_;
		TForwardIterator keyNameFirst = last_;
		TForwardIterator keyValueFirst = last_;

		bool valueHasOnlySpaces = true;

		for (auto it = first_; it != last_; ++it)
		{
			typename std::iterator_traits<TForwardIterator>::value_type character = *it;

			// Skip the '\r' character.
			if (character == '\r')
				continue;

			// Handle new line character:
			if (character == '\n')
			{
				handleLineEnd(result, state, currentSection, keyName, it, keyValueFirst, valueHasOnlySpaces);
				continue;
			}

			// Handle line start:
			if (state == State::Unknown)
			{
				// Skip whitespaces in the front:
				if (isSpace(character))
					continue;

				if (character == ';')
					state = State::ReadingComment;
				else if (character == '[')
				{
					state = State::ReadingSectionName;
					sectionNameFirst = it;
					++sectionNameFirst;
				}
				else
				{
					state = State::ReadingKeyName;
					keyNameFirst = it;
				}
			}
			else if (state == State::ReadingSectionName)
			{
				if (character == ']')
				{
					state = State::Unknown;
					currentSection = std::string(sectionNameFirst, it);
				}
			}
			else if (state == State::ReadingKeyName)
			{
				if (character == '=')
				{
					// Start reading value:
					state = State::ReadingKeyValue;
					keyName = std::string(keyNameFirst, it);
					keyValueFirst = it;
					++keyValueFirst;

					valueHasOnlySpaces = true;
				}
			}
			else if (state == State::ReadingKeyValue)
			{
				if (!isSpace(character))
					valueHasOnlySpaces = false;
			}
		}

		handleLineEnd(result, state, currentSection, keyName, last_, keyValueFirst, valueHasOnlySpaces);

		return result;
	}

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
		return read(data_, data_ + numberOfCharacters_);
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
	template <typename TForwardIterator>
	static void handleLineEnd(ReadResultType& result_, State & state_, std::string const & currentSection_, std::string const & keyName_,
		TForwardIterator currentIt_, TForwardIterator keyValueFirst_, bool valueHasOnlySpaces_)
	{
		std::string prefix = currentSection_.empty()
			? ""
			: currentSection_ + '.';

		if (state_ == State::ReadingKeyValue)
		{
			if (valueHasOnlySpaces_)
			{
				result_[prefix + keyName_] = "";
			}
			else
			{
				result_[prefix + keyName_] = std::string(keyValueFirst_, currentIt_);
			}
		}

		state_ = State::Unknown;
	}
};

}