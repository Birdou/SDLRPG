
##### If you are using SDL2 ####

SDL_COMP_FLAGS=-IY:/MinGW/include/SDL2 # -I[full path to SDL2 include folder]. Default: -IC:/MinGW/include/SDL2
SDL_LINK_FLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

################################

FILE=prog.exe

COMPILER=g++
COMPILATION_FLAGS=-Wall -Wextra -pedantic -g $(SDL_COMP_FLAGS)
LINKER_FLAGS=$(SDL_LINK_FLAGS)

DEFINES=DEBUG

MAINFILE=main
SOURCE_EXT=.cpp
HEADER_EXT=.h

SRCFOLDER=src
INCFOLDER=include
OBJFOLDER=objects

SRCPATH=./$(SRCFOLDER)/
INCPATH=./$(INCFOLDER)/
OBJPATH=./$(OBJFOLDER)/
FILE-SOURCE=$(wildcard $(SRCPATH)*$(SOURCE_EXT))
HEADERS=$(wildcard $(INCPATH)*$(HEADER_EXT))
OBJECTS=$(subst $(SOURCE_EXT),.o,$(subst $(SRCPATH),$(OBJPATH),$(FILE-SOURCE)))

#DEVELOPEMENT
all: $(OBJFOLDER) $(FILE)
	@ echo Finished all

$(FILE): $(OBJECTS)
	@ echo Linking executable $(FILE)
	@ $(COMPILER) $^ $(COMPILATION_FLAGS) $(LINKER_FLAGS) -o $(FILE) $(foreach I,$(INCPATH),$(shell echo -I$(I)))

$(OBJPATH)%.o: $(SRCPATH)%$(SOURCE_EXT) $(INCPATH)%$(HEADER_EXT) 
	@ echo Building $(COMPILER) object $@
	@ $(COMPILER) $< -c $(COMPILATION_FLAGS) -o $@ $(foreach I,$(INCPATH),$(shell echo -I$(I))) $(foreach d,$(DEFINES),$(shell echo -D$(d)))

$(OBJPATH)$(MAINFILE).o: $(SRCPATH)$(MAINFILE)$(SOURCE_EXT) $(HEADERS)
	@ echo Building $(COMPILER) object $@
	@ $(COMPILER) $< -c $(COMPILATION_FLAGS) -o $@ $(foreach I,$(INCPATH),$(shell echo -I$(I))) $(foreach d,$(DEFINES),$(shell echo -D$(d)))

$(OBJFOLDER):
	@ mkdir $(OBJFOLDER)

clean: $(OBJFOLDER)
	@ del /f /s /q objects\*.o $(FILE) >nul

.PHONY: all clean
