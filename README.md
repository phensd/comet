

# Images

| Song Selector Tab | Options Tab |
| ------------- | ------------- |
| ![image](https://github.com/user-attachments/assets/e939c6b3-323e-48ab-a10d-4fd4ba893f80)  | ![image](https://github.com/user-attachments/assets/91224904-4381-4241-b30c-1cc5eb06ef15) |




# Keybindings
| Key           | Action |
| ------------- | ------------- |
| r             | Restart current song  |
| Enter         | Start selected song  |
| +/-             | Increase/decrease volume (Hold CTRL for a larger increment)  |
| Arrows up/down, Mousewheel        | Navigate song selector  |
| Arrows left/right            | Seek current song backwards/forwards (Hold CTRL for a larger increment) |
| ,/.            | Play previous/next song |
| Shift + /    | Focus search bar |
| Del         | (Search bar) Clear current search |
| Esc             | Highlight currently playing song, or focus song selector |
| s/l         | Toggle Shuffle/Loop  |
| Ctrl+C         | Close Comet  |


# Adding songs

By default, comet will add ``$HOME/Music`` to it's list of paths to search for music in. You may add additional paths in the ``Options`` tab. </br>
![image](https://github.com/user-attachments/assets/c2179e43-5eac-469e-9454-145333380fa0) ![image](https://github.com/user-attachments/assets/2c5de2ae-2d3a-4796-a985-8c40cd72c014) </br> </br>
Then, select how you want the songs to be displayed and press "Scan Directories"
![image](https://github.com/user-attachments/assets/ce0061fa-6526-43c5-b8b5-22083f028f7f)




# Installation

Run the following commands to compile & install comet:
```
git clone https://github.com/phensd/comet.git
cd comet
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j
sudo make install 
```
Now it can be ran with
```comet```
in your terminal.


# Libraries/Frameworks Used

[FTXUI](https://github.com/ArthurSonzogni/FTXUI) (For user interface) </br>
[MiniAudio](https://miniaud.io/) (For audio playback) </br>
[Nlohmann Json](https://github.com/nlohmann/json) (For caching song paths & user options)  </br>
[TagLib](https://github.com/taglib/taglib) (For getting audio metadata) </br>
