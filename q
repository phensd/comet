[33m433c044[m[33m ([m[1;36mHEAD[m[33m -> [m[1;32mmain[m[33m)[m HEAD@{0}: commit (amend): More MPRIS implementation.
[33mdadeeb5[m HEAD@{1}: commit: More MPRIS implementation.
[33m40d8a1c[m HEAD@{2}: commit: Generate dbus code files when running make, do not track them.
[33me668d11[m HEAD@{3}: commit: Start implementing MRPIS specification. Make song progress gauge smaller.
[33m68bfba2[m HEAD@{4}: commit: stop passing logger everywhere
[33m315b3c1[m[33m ([m[1;31morigin/main[m[33m)[m HEAD@{5}: commit: Comments & cleanup
[33m17ee230[m HEAD@{6}: commit: Make sure invalidated entries are correctly corrected in the map (see previous commit)
[33m2e6d368[m HEAD@{7}: commit (amend): Verify between key & value in song cache & entry cache incase JSON modified
[33m5ce5ef6[m HEAD@{8}: commit: Verify between key & value in song cache & entry cache incase JSON modified
[33m138516e[m HEAD@{9}: commit: Enter key should act just like the play button in the UI
[33m25beff1[m HEAD@{10}: commit: on boot, load back the song the song that was playing when the player closed
[33m92d0487[m HEAD@{11}: commit: Add FLAC magic number since MiniAudio can decode it
[33m224e4ad[m HEAD@{12}: commit: Make logging to cerr a bit more transparent. Log when cached entries are being validated
[33ma4e5bd9[m HEAD@{13}: pull: Merge made by the 'ort' strategy.
[33med7b165[m HEAD@{14}: commit (amend): When encountering multiple duplicate IDs, keep incrementing the number inside (x) to reflect how many duplicates were found before it
[33m5a39155[m HEAD@{15}: commit (amend): When encountering multiple duplicate IDs, keep incrementing the number inside (x) to reflect how many duplicates were found before it
[33m66739dd[m HEAD@{16}: commit (amend): When encountering multiple duplicate IDs, keep incrementing the number inside (x) to reflect how many duplicates were found before it
[33m1f20e0f[m HEAD@{17}: commit: When encountering multiple duplicate ID's, keep incrementing the number inside (x) to reflect how many duplicates were found before it
[33m9b4a0b7[m HEAD@{18}: pull: Fast-forward
[33m1a633e0[m HEAD@{19}: reset: moving to HEAD
[33m1a633e0[m HEAD@{20}: commit: Use EXCLUDE_FROM_ALL in fetchcontent_declare in cmakelists.txt so that library headers are not installed alongside the executable target
[33ma45dead[m HEAD@{21}: commit (amend): Add buttons to the UI to seek forward and backwards in song
[33m345ceb2[m HEAD@{22}: commit: Add buttons to the UI to seek forward and backwards in song
[33mabf97cc[m HEAD@{23}: commit: if the user is searching make sure the current song id stays selected to make it easier to track
[33m2da6701[m HEAD@{24}: commit: Remove need for std::reverse call by reversing '>' into '<'
[33mcf6399d[m HEAD@{25}: commit: Rename "option" -> "song_title_display_options"
[33me324006[m HEAD@{26}: commit: Add volume up and down buttons next to volume gauge. Add silent_log argument to map_song_ids to make sure the window doesnt flash when the user is changing the song ID display option.
[33maf8667a[m HEAD@{27}: commit: Make logging to cerr a little prettier
[33mc533717[m HEAD@{28}: commit: Hardening for state where no songs are in the song_ids list
[33mfbdd8b3[m HEAD@{29}: commit (amend): Always use processed entry cache even if rescan is true. This may be problematic in case the user ever changes the metadata of a file, but if they change even the file name it will cause a reprocess anyway.
[33mbe15b01[m HEAD@{30}: commit: Always use processed entry cache even if rescan is true. This may be problematic in case the user ever changes the metadata of a file, but if they change even the file name it will cause a reprocess anyway.
[33m004bc76[m HEAD@{31}: commit: Make sure JSON extraction defaults to reasonable values if the keys are not found
[33mc34d772[m HEAD@{32}: commit: use fetch_content for taglib
[33mdf43c70[m HEAD@{33}: commit: In loop mode, when pressing the next song button on the player and it calls play_next, do not act as if the current mode is set to shuffle
[33mb029657[m HEAD@{34}: commit: Whitespace cleanup, pressing escape now highlights the currently playing song if the search bar is not focused. When clearing the search, if there is no search selection to create with the filtered results, select the current playing song in the new context instead of the first song in the list
[33mee1a4da[m HEAD@{35}: commit (amend): Cache processed songs (alongside paths) into JSON file so they can be reused when needed
[33m875bab6[m HEAD@{36}: commit: Cache processed songs into JSON file so they can be reused when needed
[33md9fe040[m HEAD@{37}: commit: Just use a string for visually_select since we are not using the iterator
[33md364076[m HEAD@{38}: commit (amend): Implement shuffle as a proper queue instead of completely random selections
[33m93339e5[m HEAD@{39}: commit: Implement shuffle as a proper queue instead of completely random selections
[33m76e28d4[m HEAD@{40}: commit (amend): Add a Maybe decorator elements that are supposed to only be interactable on specific tabs. I should probably seperate the renderers entirely, but it feels odd to do that since most components are shared between tabs.
[33m6076b55[m HEAD@{41}: commit: Add a Maybe decorator elements that are supposed to only be interactable on specific tabs. I should probably seperate the renderers entirely, but it feels odd to do that since most components are shared between tabs.
[33m9443991[m HEAD@{42}: reset: moving to main
[33m9443991[m HEAD@{43}: commit (amend): Save song display option in JSON file and load it back on startup
[33m9c4f0f8[m HEAD@{44}: commit: Save song display option in JSON file and load it back on startup
[33mbebf536[m HEAD@{45}: commit: if a song is already playing (or paused but set in the player) make sure the song id updates in the display when we remap them
[33m26b5269[m HEAD@{46}: commit (amend): Add displaying songs by their MP3 tags (falls back to filename if they dont exist.) Allow search by the tags aswell
[33mc5ec6c4[m HEAD@{47}: commit (amend): Add displaying songs by their MP3 tags (falls back to filename if they dont exist.) Allow search by the tags aswell
[33m3e78572[m HEAD@{48}: commit (amend): Add displaying songs by their MP3 tags (falls back to filename if they dont exist.) Allow search by the tags aswell
[33mcbb9c4f[m HEAD@{49}: reset: moving to HEAD~
[33m821245d[m HEAD@{50}: commit: clear song ids map on refresh
[33mcbb9c4f[m HEAD@{51}: commit: Add displaying songs by their MP3 tags (falls back to filename if they dont exist.) Allow search by the tags aswell
[33mea151cc[m HEAD@{52}: commit: if the user modified the json in any way, make sure any invalid entries they might have added are removed.
[33m1de4af4[m HEAD@{53}: commit: Input boxes and scan directories button should rescan the directories, see previous commit
[33me939e80[m HEAD@{54}: commit: Cache song entries into json file, don't scan directories enless needed or asked to
[33mcede367[m HEAD@{55}: commit: Make JSON loading functions more generic
[33mddcdf38[m HEAD@{56}: commit: rename current song title to current song id. add buttons to the GUI for play/pause, next and previous track. separate play or pause function into two so it functions differently when pressing space or using the new GUI button
[33m6d37b13[m HEAD@{57}: commit: song title display toggle refreshes on click now with callback
[33mdae47f1[m HEAD@{58}: commit (amend): if there is a duplicate ID then add (1) to it so when play_next searches for a song a duplicate wont trip it up. Also, make sure all_songs vector is sorted by the file paths of the songs.
[33me4f2d56[m HEAD@{59}: commit (amend): if there is a duplicate ID then add (1) to it so when play_next searches for a song a duplicate wont trip it up. Also, make sure all_songs vector is sorted by the file paths of the songs.
[33m5c06fc1[m HEAD@{60}: commit: if there is a duplicate ID then add (1) to it so when play_next searches for a song a duplicate wont trip it up. Also, make sure all_songs vector is sorted by the file paths of the songs.
[33macd7911[m HEAD@{61}: commit (amend): match the search with the full path of the song entry, instead of the chosen display id
[33mb82787d[m HEAD@{62}: commit: match the search with the full path of the song entry, instead of the chosen display id
[33m318c3b4[m HEAD@{63}: commit: Implement song manager class to handle mapping of songs to arbitrary ids, so the user can choose how they want songs to be displayed
[33m1702364[m HEAD@{64}: commit: song manager class base
[33mc92376a[m HEAD@{65}: commit: Add a button to the options menu to refresh the song selection with the mouse
[33m83d933a[m HEAD@{66}: commit (amend): Remove extraneous check for whether a sound is playing in get_state_message(). for some reason despite this working before, it no longer does....
[33m53bc0fc[m HEAD@{67}: commit (amend): Remove extraneous check for whether a sound is playing in get_state_message(). for some reason despite this working before, it no longer does....
[33m0cd6945[m HEAD@{68}: commit: Remove extraneous check for whether a sound is playing in get_state_message(). for some reason despite this working before, it no longer does....
[33meacaed6[m HEAD@{69}: commit: Move default path entries function, and loading of user paths to filesystem manager class. comment out usage of engine.get_state_message() because it suddenly broke
[33m2a7c849[m HEAD@{70}: commit: encapsulate filesystem management as class, checkpoint commit this is not done yet
[33mdb0eaed[m HEAD@{71}: commit: Add an option to change how song titles are displayed. Not actually implemented yet
[33mef7e03d[m HEAD@{72}: commit: Dont show some elements in the options tab (This is ugly for now)
[33m4f4b984[m HEAD@{73}: commit: change music_player namespace name to project name comet
[33m56c6d3a[m HEAD@{74}: commit: Dont clear search when pressing enter on path input boxes, it causes an unusual endless loop if the resulting vector after refreshing song entries is empty
[33m0bb71e0[m HEAD@{75}: commit (merge): Add cmake compile options
[33me123bc6[m HEAD@{76}: commit (amend): Show current timestamp and total length of song in player
[33m2d214e0[m HEAD@{77}: commit (amend): Show current timestamp and total length of song in player
[33md4a17a1[m HEAD@{78}: commit: Show current timestamp and total length of song in player
[33m4eb4f96[m HEAD@{79}: commit: move player deconstructor to header since it is a oneliner
[33m2da195c[m HEAD@{80}: commit (amend): Split inputs into seperate file for cleanliness
[33me3229db[m HEAD@{81}: commit (amend): Split inputs into seperate file for cleanliness
[33m612785b[m HEAD@{82}: commit (amend): Split inputs into seperate file for cleanliness
[33md908d51[m HEAD@{83}: commit (amend): Split inputs into seperate file for cleanliness
[33m6988494[m HEAD@{84}: commit: Split inputs into seperate file for cleanliness
[33m5d873c3[m HEAD@{85}: commit: comments, cleanup
[33m9eeecbc[m HEAD@{86}: commit: add comments to play_next function
[33mfb6c94d[m HEAD@{87}: commit (amend): Try to reduce the number of linear searches
[33m9e73dd8[m HEAD@{88}: commit (amend): Try to reduce the number of linear searches
[33m294d66c[m HEAD@{89}: commit (amend): Try to reduce the number of linear searches
[33mbed01d0[m HEAD@{90}: commit: Try to reduce the number of linear searches
[33m680ed80[m HEAD@{91}: commit (amend): Make sure when a song starts playing, it is always set to the selected value in the player
[33m621c9a3[m HEAD@{92}: commit (amend): Make sure when a song starts playing, it is always set to the selected value in the player
[33m564cd36[m HEAD@{93}: commit (amend): Make sure when a song starts playing, it is always set to the selected value in the player
[33m66548c8[m HEAD@{94}: commit: Make sure when a song starts playing, it is always set to the selected value in the player
[33mb857768[m HEAD@{95}: commit (amend): Add keybindings to skip to next or previous song
[33m3da9d45[m HEAD@{96}: commit (amend): Add keybindings to skip to next or previous song
[33mf72beaa[m HEAD@{97}: commit: Add keybindings to skip to next or previous song
[33mc74ff2f[m HEAD@{98}: commit: wrap previous commit into a function for the cleanliness of it
[33m97f043c[m HEAD@{99}: commit: Use the actual index of the current song when progressing to the next rather than the users current selection
[33m6b36a31[m HEAD@{100}: commit: stop audio when loading a new song just in case, add flags to mini audio loading for optimization
[33md207743[m HEAD@{101}: commit: cleanup & encapsulation
[33m9445fcf[m HEAD@{102}: commit: Use file path for song title directly
[33me4e82e7[m HEAD@{103}: commit (amend): Add default play_next
[33mab19453[m HEAD@{104}: commit: Add default play_next
[33m6062e97[m HEAD@{105}: commit: Implement shuffle & loop controls properly, fix weird multithreading problem in main.cpp
[33m542466f[m HEAD@{106}: commit: Various comments
[33me4d38ac[m HEAD@{107}: commit: Implement a better system for displaying playing.. message including whether the state is set to loop or shuffle songs, add keybinds to toggle them aswell (todo: actually make shuffle and loop work...)
[33m0e9527b[m HEAD@{108}: commit: Set cpp standard to 20
[33meb1ad68[m HEAD@{109}: commit: After clearing search entry, make sure the song that was selected in the search context is re-selected in the new context
[33m06a5719[m HEAD@{110}: commit: Even if the search bar isn't focused, clear it when pressing delete
[33m06c5d32[m HEAD@{111}: commit: Use an std::set to filter out duplicate song entries, this has the side effect of sorting everything in alphabetical order, but that is fine for now
[33m4af47c7[m HEAD@{112}: commit: Make executable name match that of project's, add install command to CMakeLists
[33m3a6640f[m HEAD@{113}: commit (initial): Initial commit
