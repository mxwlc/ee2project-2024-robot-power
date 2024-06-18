# Computer Vision C++ Program

## MarkerDetect

First prototype webcam ArUco Marker Detection tool.\

- Identifies the location and id of markers in an image.
- Translates the location of the marker into a corresponding robot movement instruciton.
- Debug tools such as
    - Print all Marker corners coordinates
    - Print all Marker Ids
    - Draw Box around all Markers
    - Show location of overlays

Source Code : src/marker_detect.cpp

## RPEyes

Minimal version of marker detect with limited UI and debug functions.

- Translates the location of each target marker into a driving detection, given a target id.

Source Code : src/marker_detect_rpi.cpp

## MarkerGen

Source Code : src/marker_gen.cpp

## Miscellaneous Programs

### Colour

> Debug Program, cycles through Black, Blue, Green and Red Images

Source Code : src/colour_test.cpp

### ImageDetector

> Program written during testing period, detects and draws markers along with the corrsponding images from a give image.

Source Code : src/marker_detect_image.cpp

### Test

> Debug, a preconfigured target for ad hoc testing.

Source Code : src/test_writing.cpp

# Computer Vision C++ Objects

## MarkerDict

### Methods

```c++
    class MarkerDict
{
private:
std::map<int, states> marker_map;
public:
MarkerDict();
MarkerDict(std::map<int, states>& dict);
MarkerDict(std::string filename);
~MarkerDict();
void add_marker(int id, states marker_state);
states marker_translate(int id);
std::map<int, states> return_dict() const;
std::string print_dict() const;
void save_dict();
std::string enum_string_translation(states in_state);
int size_of_map();
std::map<int, states> load_marker_map(std::string filename);
};
```

| Function                                                        | Use                                                                    |
|-----------------------------------------------------------------|------------------------------------------------------------------------|
| `MarkerDict()`                                                  | Constructor                                                            |
| `MarkerDict(std::map<int, states>& dict)`                       | Constructor                                                            |
| `MarkerDict(std::string filename);`                             | Constructor                                                            |
| `~MarkerDict()`                                                 | Destructor                                                             |
| `void add_marker(int id, states marker_state)`                  | Adds a (marker, state) key value pair to the internal marker state map |
| `states marker_translate(int id);`                              | Given an id this function returns the corresponding state              |
| `std::map<int, states> return_dict()`                           | Returns the internal marker map                                        |
| `std::string print_dict() const;`                               | Prints a Formatted representation of the inner marker map              |
| 	`void save_dict();`                                            | Saves the internal marker map to a text file                           |
| 	`std::string enum_string_translation(states in_state);`        | Given a state enum a corresponding string is returned                  |
| 	`int size_of_map();`                                           | Returns the Size of the marker Map                                     |
| 	`std::map<int, states> load_marker_map(std::string filename);` | Loads a marker map from a local text file                              |     