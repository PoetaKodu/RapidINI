#pragma once

#include <map>
#include <string>
#include <stdexcept>

#include "IniReader.hpp"

namespace rapid_ini
{


class IniContainer
{
public:
	using ParseResultType = IniReader::ParseReturnType;

	IniContainer() = default;

	IniContainer( ParseResultType && parseResult_ )
	{
		_properties = std::forward<ParseResultType>(parseResult_);
	}

	IniContainer( ParseResultType const & parseResult_ )
	{
		_properties = parseResult_;
	}

	void importProperties(ParseResultType && parseResult_)
	{
		_properties = std::forward<ParseResultType>(parseResult_);
	}

	void importProperties( ParseResultType const & parseResult_ )
	{
		_properties = parseResult_;
	}

	void setProperty(std::string const & keyName_, std::string const & keyValue_)
	{
		_properties[keyName_] = keyValue_;
	}

	bool keyExists(std::string const & name_) const
	{
		return _properties.find(name_) != _properties.end();
	}

	std::string getValue(std::string const & keyName_) const
	{
		auto const it = _properties.find(keyName_);

		if (it == _properties.end())
			throw std::invalid_argument("Key name not exists.");

		return it->second;
	}

	std::string getValue(std::string const & sectionName_, std::string const & keyName_) const
	{
		if (sectionName_.empty())
			return this->getValue(keyName_);

		return this->getValue(sectionName_ + "." + keyName_);
	}

	std::string getValueOr(std::string const & keyName_, std::string const & valueIfNotFound_) const
	{
		auto const it = _properties.find(keyName_);

		if (it == _properties.end())
			return valueIfNotFound_;

		return it->second;
	}

	std::string getValueOr(std::string const & sectionName_, std::string const & keyName_, std::string const & valueIfNotFound_) const
	{
		if (sectionName_.empty())
			return this->getValueOr(keyName_, valueIfNotFound_);

		return this->getValueOr(sectionName_ + "." + keyName_, valueIfNotFound_);
	}

	ParseResultType const& getProperties() const
	{
		return _properties;
	}

private:
	ParseResultType _properties;
};

}