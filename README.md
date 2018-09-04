# RapidINI
A robust, header only INI file reader and writer (planned) written in pure C++11.

## How to use?

Include the files [`Reader.hpp`](include/RapidINI/Reader.hpp), [`Writer.hpp`](include/RapidINI/Writer.hpp) and [`Container.hpp`](include/RapidINI/Container.hpp) (located inside [`include/RapidINI/`](include/RapidINI`) directory) depending on which classes you want to use.

## Classes

RapidINI consists of three classes:

- `Reader` - provides static method `read`, which reads INI file that you need to provide as a single `std::string` (or `char` array).
- `Writer` **(not implemented yet)** - provides multiple ways of saving `Container` to the `std::string`.
- `Container` - wraps around the result of `Reader::read()` method. Provides handy way to manipulate properties.

These classes are located inside `rapid_ini` namespace.

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
ini::Container container( ini::Reader::read(fileContents) );

// Note: letter case does matter.
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
  #include <RapidINI/Reader.hpp>
  // etc
  ```

## Trivia

RapidINI is inspired by `RapidXML` parser.


