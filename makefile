#Makefile (por Felipe Sena)

#	$@	Nome da regra.
#	$<	Nome da primeira dependência
#	$^	Lista de dependências
#	$?	Lista de dependências mais recentes que a regra.
#	$*	Nome do arquivo sem sufixo

FILE_SDLP=SDLP.exe

COMPILER_SDLP=g++
COMPILATION_FLAGS_SDLP=-Wall -Wextra -pedantic -Wno-unused-result -IY:/MinGW/include/SDL2 -g -DDEBUG
LINKER_FLAGS_SDLP=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

#-mwindows
#-static -static-libgcc -static-libstdc++

MAINFILE_SDLP=main
 SRCPATH_SDLP=./src/
 INCPATH_SDLP=./include/
 OBJPATH_SDLP=./objects/

SOURCE_EXT_SDLP=.cpp
HEADER_EXT_SDLP=.h


FILE-SOURCE=$(wildcard $(SRCPATH_SDLP)*$(SOURCE_EXT_SDLP))
HEADERS=$(wildcard $(INCPATH_SDLP)*$(HEADER_EXT_SDLP))
OBJECTS=$(subst $(SOURCE_EXT_SDLP),.o,$(subst $(SRCPATH_SDLP),$(OBJPATH_SDLP),$(FILE-SOURCE)))

#DEVELOPEMENT
all: $(OBJFOLDER) $(FILE_SDLP)
	@ echo Finished all

$(FILE_SDLP): $(OBJECTS)
	@ echo  Linking executable $(FILE_SDLP)
	@ $(COMPILER_SDLP) $^ $(COMPILATION_FLAGS_SDLP) $(LINKER_FLAGS_SDLP) -o $(FILE_SDLP) $(foreach I,$(INCPATH_SDLP),$(shell echo -I$(I)))

$(OBJPATH_SDLP)%.o: $(SRCPATH_SDLP)%$(SOURCE_EXT_SDLP) $(HEADERS)
	@ echo  Building $(COMPILER_SDLP) object $@
	@ $(COMPILER_SDLP) $< -c $(COMPILATION_FLAGS_SDLP) -o $@ $(foreach I,$(INCPATH_SDLP),$(shell echo -I$(I)))

$(OBJPATH_SDLP)$(MAINFILE_SDLP).o: $(SRCPATH_SDLP)$(MAINFILE_SDLP)$(SOURCE_EXT_SDLP) $(HEADERS)
	@ echo  Building $(COMPILER_SDLP) object $@
	@ $(COMPILER_SDLP) $< -c $(COMPILATION_FLAGS_SDLP) -o $@ $(foreach I,$(INCPATH_SDLP),$(shell echo -I$(I)))

$(OBJFOLDER):
	@ mkdir $(OBJFOLDER)

clean: $(OBJFOLDER)
	@ del /f /s /q objects\*.o $(FILE_SDLP) >nul

.PHONY: all clean
