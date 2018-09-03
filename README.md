# RapidINI
A robust, header only INI file reader and writer (planned) written in pure C++11.

## How to use?

Include the files [`IniReader.hpp`](include/RapidINI/IniReader.hpp), [`IniWriter.hpp`](include/RapidINI/IniWriter.hpp) and [`IniContainer.hpp`](include/RapidINI/IniContainer.hpp) (located inside [`include/RapidINI/`](include/RapidINI`) directory) depending on which classes you want to use.

## Classes

RapidINI consists of three classes:

- `IniReader` - provides static method `read`, which reads INI file that you need to provide as a single `std::string` (or `char` array).
- `IniWriter` (planned) - provides multiple ways of saving `IniContainer` to the `std::string`.
- `IniContainer` - wraps around the result of `IniReader::read()` method. Provides handy way to manipulate properties.

## Example

Minimalist example code can look like this:

```cpp
namespace ini = rapid_ini; // change the dirty and long `rapid_ini` name to simple `ini`.

std::string fileContents = R"(
[Player]
MaxHealth=300
MaxEnergy=200
; MovementSpeed is measured in pixels per second.
MovementSpeed=400
Name=Bezimienny
)";
ini::IniContainer container( ini::IniReader::read(fileContents) );

std::cout << "Our player has MaxHealth equal to " << container.getValueOr("Player.MaxHealth", "100" ); // use 100 by default.
```

## What RapidINI does not provide?

RapidINI does not contain any way of converting property values to any type. You must handle casting string contents to e.g. `int` by yourself.

It also won't read directly from the file. Checkout how to read entire file sequentially in `Reader` test.

## FAQ:

- **Can it run on pre-C++11 compiler?**  
  No, it can't. I use plenty of `auto`s here which are available (in this context) since C++11. If you are using pre-C++11 compiler please update it. We now have C++17 and C++20 is coming out soon.

- **What is the best way to use it?**  
  Clone the repo to the disk, include path: `path/to/RapidINI_repo/include` and
  ```cpp
  #include <RapidINI/IniReader.hpp>
  // etc
  ```

## Trivia

RapidINI is inspired by `RapidXML` parser.


