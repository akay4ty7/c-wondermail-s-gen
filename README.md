# Wondermail S Generator for Pokemon Mystery Dungeon: Explorers of Sky
## Project Goal
**Pokemon Mystery Dungeon: Explorers of Sky** allows for users to enter 'wondermail S passwords' to obtain missions to do in the game. The mission data is packed into 136 bits, a CRC32 checksum is appended and the whole thing is encrypted through bit shuffling, then converted to base-45 characters and scrambled into the final password format.

If you search online, you will be able to find Wondermail S generators. I am not reinventing any wheels, but was still curious enough to replicate the tool myself in C. I was able to inspect the JavaScript logic from existing generators and translate it to C. The existing generators allowed for passwords to  be generated despite exceeding floor maximums for dungeons. This caused the game to reject some passwords after being entered. I made an improvement to the logic to account for maximum floors for all dungeons as quality of life feature.

Although not deeply, I picked up skills like:
- Reverse engineering: Understanding someone else's JavaScript and adapting it to C.
- Low-level bit manipulation: Packing/unpacking data at bit level, shifting, masking, working with specific bit positions.
- C struct memory layout management: Handling bugs related to struct layouts. The order of attributes in structs matter for performance.
- Validation logic: Adding improvements to existing logic with floor restrictions for dungeons.
- Checksum/encoding pipelines: Validate data integrity and transform binary to text format.

## Thoughts and Future Improvements
This generator is not perfect, and can still produce seemingly valid passwords, only for the user to find out it does not work. At this stage I am unsure what exact conditions cause these passwords to be invalid. It could range from the user not progressing far enough in the game itself or some other impossibility checks that the game's code. Unless I decompile the ROM itself and investigate further, it will remain unknown to me.

That being said, future improvements include:
- Search menu selection with some form of fuzzy search.
- Investigate and improve the validity checks to make sure that passwords generated are always accepted.
- Automatic password entry for automated testing capabilities.
## Quick Demo
### Custom Wondermail S Generation
https://github.com/user-attachments/assets/c38a4b59-924d-4642-9168-cc5eedb57ea7
### Random Wondermail S Generation
https://github.com/user-attachments/assets/18ee04ae-1fe5-42d1-9925-60ddd7442d02
### Accepted Wondermail S Password
https://github.com/user-attachments/assets/f6fd2e65-6d7f-4e3f-b6e1-6342ed029834
## Prerequisite
- C Compiler (gcc/Clang or similar)
- make
## Build
```
make
```
## Run
```
./main
```
