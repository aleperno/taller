#Makefile Mortal Kombat - Taller de programacion I

#OBJS specifies which files to compile as part of the project
OBJS = Source/*.cpp 

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -Werror -pedantic -pedantic-errors 

#COMPILER DIALECT
COMPILER_FLAGS += -std=c++11

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -I/usr/include/SDL2 -I$(PWD)/Headers -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2main -lSDL2_ttf -LHeaders/*.h 

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = MortalKombat

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	$(RM) log.txt $(OBJ_NAME)
