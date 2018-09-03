// Standard includes:
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <exception>
#include <chrono>
#include <thread>

// RapidINI reader:
#include <RapidINI/IniContainer.hpp>

namespace ini = rapid_ini;

namespace rapid_ini_tests
{

namespace details
{

std::string sequentialReadFile(std::string const & fileName_)
{
	std::string fileContents;
	
	constexpr int bufferSize = 4 * 1024;
	char buffer[bufferSize];
	std::ifstream file("Test.ini");

	if (!file.is_open()) {
		throw std::exception("Test file not found.");
	}

	while ( file.read( buffer, sizeof(buffer) ) )
		fileContents.append( buffer, sizeof(buffer) );
	fileContents.append( buffer, static_cast<std::string::size_type>( file.gcount() ) );
	
	return fileContents;
}

} // namespace details


// Function that actually uses the reader:
void testReader(std::string const & inputFile_)
{
	// 1. Load file contents:
	std::string const fileContents = details::sequentialReadFile(inputFile_);
	
	// 2. Parse INI and pass the result to the IniContainer:
	ini::IniContainer container( ini::IniReader::read(fileContents) );

	// 3. Display every property:
	std::cout << "Following properties were found:\n";
	for (auto const & prop : container.getProperties())
	{
		std::cout << prop.first << "=" << prop.second << "\n";
	}
	std::cout << "===================================" <<  std::endl;

	// Separate container actions:

	// 4. `getValue` method:

	// Be careful! `getValue` can throw exception if key name does not exist.
	std::cout << "# getValue()" << std::endl;
	try
	{
		std::string const val = container.getValue("Database.Connection.HostName"); // Note: case does matter.

		std::cout << "Value of key \"Database.Connection.HostName\" = \"" << val << "\"" << std::endl;
	}
	catch(std::exception &exc_)
	{
		std::cerr << "[ERR]: Cannot display value of \"Database.Connection.HostName\". Exception ocurred, details: " << exc_.what() << std::endl;
	}

	// 5. `getValueOr` method:

	// Better use `getValueOr` method:
	std::cout << "# getValueOr()" << std::endl;
	std::string const val = container.getValueOr("Database.Connection.HostName", "localhost"); // Note: letter case does matter.

	std::cout << "Value of key \"Database.Connection.HostName\" = \"" << val << "\"" << std::endl;
}

} // namespace rapid_ini_tests


int main()
{
	
	bool success = false;
	try
	{
		rapid_ini_tests::testReader("Test.ini");

		success = true;
	}
	catch(std::exception& exc) {
		std::cerr << "[ERR]: A critical error ocurred during application execution and it had to be shut down.\nDetails: " << exc.what() << std::endl;
	}
	catch(...) {
		std::cerr << "[ERR]: A critical error ocurred during application execution and it had to be shut down.\nNo additional details available." << std::endl;
	}

	if (success) {
		std::clog << "[LOG]: Application finished successfully." << std::endl;
	}

	constexpr int shutdownSeconds = 30;
	std::clog << "[LOG]: Program will shut down in " << shutdownSeconds << " seconds" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds{ shutdownSeconds });
}