1 byte      version, start at 0
4 byte      packet_len uint32_t. Includes entire packet length (including headers, etc.)
1 byte      Obj type
    - 0 = Game Tile Info
    - 1 = Send Clue
    - 2 = Send Guess
    - 3 = Team/Role Assignment
    - 4 = Request Tile
    - 5 = End of Game Notification

Type 0 -- Game Tile (Remaining Packet Info)
1 byte x coord (uint8_t)
1 byte y coord (uint8_t)
1 byte color/status (TileType)
1 byte word len = k (< 64)
k bytes word
0-pad to 128 bytes?

Type 1 -- Send Clue (Remaining Packet Info)
1 byte clue len = k (< 64)
k bytes clue len
1 byte number of words
0-pad to 128 bytes?

Type 2 -- Send Guess
1 byte x coord (uint8_t)
1 byte y coord (uint8_t)
0-pad to 128 bytes

Type 3 -- Team/Role Assignment (PlayerInfo)
1 byte Team (0 = Red, 1 = Blue, use some enum)
1 byte Role (0 = Clue Sender, 1 = Clue Receiver)
0-pad to 128 bytes

Type 4 -- Request Tile
1 byte x coord (uint8_t)
1 byte y coord (uint8_t)
0-pad to 128 bytes

Type 5 -- End of Game Notification
1 byte winning team (0 = Red, 1 = Blue, 2 = Tie?)
0-pad to 128 bytes

Type 6 -- Start of game
0-pad to 128 bytes

// TODO: Combine message types 5 - 7 into 1 message type
Type 7 -- Game State (Going to new game state
1 byte Team (0 = Red, 1 = Blue, use some enum)
1 byte Role (0 = Clue Sender, 1 = Clue Receiver)
0-pad to 128 bytes
