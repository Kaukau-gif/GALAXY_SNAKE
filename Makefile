CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2 -Iinclude
TARGET  = snake_bfs.exe
SRCS    = src/game_state.c \
          src/bfs.c        \
          src/ranking.c    \
          src/renderer.c   \
          src/game.c       \
          src/input.c      \
          src/main.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	del /Q $(TARGET) 2>nul || rm -f $(TARGET)
