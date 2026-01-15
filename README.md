# Wondermail S Generator for Pokemon Mystery Dungeon: Explorers of Sky
## Project Goal
**Pokemon Mystery Dungeon: Explorers of Sky features** a password system called Wondermail S. Players enter alphanumeric codes to unlock missions with specific rewards, dungeons, and objectives. Under the hood, mission data is packed into 136 bits, a CRC32 checksum is appended, the result is encrypted via bit shuffling, then converted to base-45 characters and scrambled into the final password format. Generators for these passwords already exist online but I wanted to understand the pipeline myself by reimplementing one in C. I translated the logic from existing JavaScript implementations. During testing, I noticed these generators would produce passwords for floor numbers exceeding dungeon maximums, which the game silently rejects. I added validation tables for all dungeons to catch this before generation.

**Interesting Knowledge Gained:**
- Reverse engineering: Reading someone else's JavaScript and translating it to C with equivalent behavior.
- Bit-level data packing: Shifting, masking, and positioning data into a compact 136-bit structure.
- Encoding pipelines: Understanding how binary data moves through checksum, encryption, and base conversion stages.
- Struct layout awareness: Discovering that member ordering in C structs affects both correctness and performance.
## Known Limitations
The generator can still produce passwords the game rejects. Without decompiling the ROM, I cannot identify all the validity checks the game performs internally. Possible causes include story progression gates or item/dungeon unlock states.

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
