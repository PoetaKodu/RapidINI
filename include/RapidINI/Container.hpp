#pragma once

#include <map>
#include <string>
#include <stdexcept>

#include "Reader.hpp"

namespace rapid_ini
{

/// <summary>
/// Wraps around result from <c>Reader::read()</c> and provides convenient way of accessing properties.
/// </summary>
class Container
{
public:
	/// <summary>
	/// Result type of the <c>Reader::read()</c> function.
	/// </summary>
	using ReadResultType = Reader::ReadResultType;

	/// <summary>
	/// Initializes instance of the <c>Container</c> class.
	/// </summary>
	Container() = default;

	/// <summary>
	/// Initializes instance of the <c>Container</c> class - a copy constructor.
	/// </summary>
	Container(Container const&) = default;

	/// <summary>
	/// Initializes instance of the <c>Container</c> class - a move constructor.
	/// </summary>
	Container(Container &&) = default;

	/// <summary>
	/// Copies properties from other instance - a copy-assign operator.
	/// </summary>
	Container& operator=(Container const&) = default;

	/// <summary>
	/// Moves properties from other instance - a move-assign operator.
	/// </summary>
	Container& operator=(Container&&) = default;

	/// <summary>
	/// Initializes instance of the <c>Container</c> class using value returned from <c>Reader::read()</c> method.
	/// </summary>
	/// <param name="readResult_">Result of the <c>Reader::read()</c> method.</param>
	Container(ReadResultType && readResult_)
	{
		_properties = std::forward<ReadResultType>(readResult_);
	}

	/// <summary>
	/// Initializes instance of the <c>Container</c> class using value returned from <c>Reader::read()</c> method.
	/// </summary>
	/// <param name="readResult_">Result of the <c>Reader::read()</c> method.</param>
	Container(ReadResultType const & readResult_)
	{
		_properties = readResult_;
	}

	/// <summary>
	/// Imports properties from value returned from <c>Reader::read()</c> method. Removes previous properties.
	/// </summary>
	/// <param name="readResult_">Result of the <c>Reader::read()</c> method.</param>
	void importProperties(ReadResultType && readResult_)
	{
		_properties = std::forward<ReadResultType>(readResult_);
	}

	/// <summary>
	/// Imports properties from value returned from <c>Reader::read()</c> method. Removes previous properties.
	/// </summary>
	/// <param name="readResult_">Result of the <c>Reader::read()</c> method.</param>
	void importProperties(ReadResultType const & readResult_ )
	{
		_properties = readResult_;
	}

	/// <summary>
	/// Sets value of the property with specified key name. If the property does not exist, it will be created.
	/// </summary>
	/// <param name="keyName_">Key name of the property.</param>
	/// <param name="keyValue_">Value of the property.</param>
	void setProperty(std::string const & keyName_, std::string const & keyValue_)
	{
		_properties[keyName_] = keyValue_;
	}

	/// <summary>
	/// Determines whether property with specified name exists.
	/// </summary>
	/// <param name="name_">Name of the property.</param>
	/// <returns>
	///		<c>true</c> if property with specified name exists; otherwise, <c>false</c>.
	/// </returns>
	bool keyExists(std::string const & name_) const
	{
		return _properties.find(name_) != _properties.end();
	}

	/// <summary>
	/// Returns value of property with the specified name.
	/// </summary>
	/// <param name="keyName_">Name of the property.</param>
	/// <exception>std::invalid_argument</exception>
	/// <returns>
	///		Value of the property.
	/// </returns>
	std::string getValue(std::string const & keyName_) const
	{
		auto const it = _properties.find(keyName_);

		if (it == _properties.end())
			throw std::invalid_argument("Key name not exists.");

		return it->second;
	}

	/// <summary>
	/// Returns value of property with the specified name from the specified section.
	/// </summary>
	/// <param name="sectionName_">Name of the section.</param>
	/// <param name="keyName_">Name of the property.</param>
	/// <exception>std::invalid_argument</exception>
	/// <returns>
	///		Value of the property.
	/// </returns>
	std::string getValue(std::string const & sectionName_, std::string const & keyName_) const
	{
		if (sectionName_.empty())
			return this->getValue(keyName_);

		return this->getValue(sectionName_ + "." + keyName_);
	}

	/// <summary>
	/// Returns value of property with the specified name or specified replacement value if property does not exist.
	/// </summary>
	/// <param name="keyName_">Name of the property.</param>
	/// <param name="valueIfNotFound_">Value to return if property not found.</param>
	/// <returns>
	///		Value of the property or specified replacement value if property does not exist.
	/// </returns>
	std::string getValueOr(std::string const & keyName_, std::string const & valueIfNotFound_) const
	{
		auto const it = _properties.find(keyName_);

		if (it == _properties.end())
			return valueIfNotFound_;

		return it->second;
	}

	/// <summary>
	/// Returns value of property with the specified name in specified section or specified replacement value if property does not exist.
	/// </summary>
	/// <param name="sectionName_">Name of the section.</param>
	/// <param name="keyName_">Name of the property.</param>
	/// <param name="valueIfNotFound_">Value to return if property not found.</param>
	/// <returns>
	///		Value of the property or specified replacement value if property does not exist.
	/// </returns>
	std::string getValueOr(std::string const & sectionName_, std::string const & keyName_, std::string const & valueIfNotFound_) const
	{
		if (sectionName_.empty())
			return this->getValueOr(keyName_, valueIfNotFound_);

		return this->getValueOr(sectionName_ + "." + keyName_, valueIfNotFound_);
	}

	/// <summary>
	/// Returns cref to underlying property container.
	/// </summary>
	/// <returns>cref to underlying property container.</returns>
	ReadResultType const& getProperties() const
	{
		return _properties;
	}

private:
	ReadResultType _properties;
};

}