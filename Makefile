# 0 mode Normal (all files) ou MF (multiple files or one file, but not all files) 
MODE = N#pas d'espaces après N
EXEC = #Binary file 
TFP = #Target File Path => make TFM=filename TFP=path/ OU TFM=* 
TFM = #Target File => make TFM=/filename:/filename:/etc: OU TFM=* par defaut
CC = gcc # GNU Compiler Collection
CCG = $(CC) -g # gcc + informations de débogage par défaut
IPATH = include
OPATH = obj/
CPATH = #Target Repo 
BPATH = bin/
BIN = 
NAME = # par defaut ce sera 'main' si mode normal ou le nom du fichier indiqué dans TFM si mode MF
empty:=
space:= $(empty) $(empty)
CFLAGS = -I $(IPATH) -Wall -Wextra -Werror
# On cherche dans le répertoires spécifié par -I 
# (donc ici notre répertoire "include") et warnings exhaustifs

$(info )
$(info #### Information : 'make' ET/OU 'NAME=binName' ET/OU 'TFP=<path>' (TFP=src par défaut) ET/OU {'MODE = MF' ET 'TFM=</filename1:/filename2:/etc:>'} ####)
$(info )
$(info )
$(info #### Exemple : make NAME=binary TFP=sourceChemin MODE=MF TFM=/filename1:/filename2:/etc: ####)
$(info )

ifneq ($(TFP),) #chaine non vide donc TFP=<path>
	CPATH = $(TFP)/
else ifeq ($(TFP),)
	CPATH = src/
else
$(info Information utile : Saisissez correctement le nom des fichiers et du chemin)
$(error Effectuez : 'make' ET/OU 'NAME=binName' ET/OU 'TFP=<path>' (TFP=src par défaut) ET/OU {'MODE = MF' ET 'TFM=<filename/s>'})
endif

# list of C source files into a list of object files by 
# replacing the ‘.c’ suffix with ‘.o’ in the result
ifeq ($(MODE),N) # mode normal : 'make' ET/OU 'TFP=<path>' 
	ifeq ($(TFM),)
		EXEC = main
		TFM = *
		OBJETS = $(patsubst $(CPATH)%.c,$(OPATH)%.o,$(wildcard $(CPATH)$(TFM).c))
	else 
$(error Effectuez : 'make' ET/OU 'NAME=binName' ET/OU 'TFP=<path>' (TFP=src par défaut) ET/OU {'MODE = MF' ET 'TFM=<filename/s>'})
	endif
else ifeq ($(MODE),MF) # mode multiple TFMC : add cPath and .c, OBJETS : add oPath and .o
	ifneq ($(TFM),)
	TFMC = $(subst /,$(CPATH),$(subst :,.c ,$(TFM))) 
	OBJETS = $(patsubst $(CPATH)%.c,$(OPATH)%.o,$(TFMC))
	TFMS = $(subst /,,$(subst :, ,$(TFM))) 
	EXEC=$(word 1,$(TFMS))
	else
$(info Information utile : Saisissez correctement le nom des fichiers et du chemin)
$(error Effectuez : 'make' ET/OU 'NAME=binName' ET/OU 'TFP=<path>' (TFP=src par défaut) ET/OU {'MODE = MF' ET 'TFM=<filename/s>'})
	endif
else 
$(info Information utile : Saisissez correctement le nom des fichiers et du chemin)
$(error Effectuez : 'make' ET/OU 'NAME=binName' ET/OU 'TFP=<path>' (TFP=src par défaut) ET/OU {'MODE = MF' ET 'TFM=<filename/s>'})
endif

ifneq ($(NAME),)
	EXEC = $(NAME)
endif

BIN = $(BPATH)$(EXEC)

.PHONY : all
all: $(BIN)
$(BIN) : $(OBJETS)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "\n#### Binaire '$(EXEC)' crée dans le répertoire : '$(BPATH)' ####\n"

$(OPATH)%.o : $(CPATH)%.c msg_obj_compiling
	@mkdir -p $(@D)
	@$(CCG) $(CFLAGS) -o $@ -c $< 

.INTERMEDIATE: msg_obj_compiling
msg_obj_compiling:
	@echo "\n#### Objets : '$(OBJETS)' crée(s) dans le répertoire : '$(OPATH)' ####\n"

# Without PHONY clean target will not work properly if a file 
# named clean is ever created in this directory.
.PHONY : clean
clean :
	@rm -rf $(BPATH) $(OPATH)
	@echo "\n#### Les répertoires '$(OPATH)' et '$(BPATH)' ont été supprimés ####\n"

# $@ : nom de la cible qui provoque l’exécution de la commande
# $^ : nom de toutes les dépendances
# $< : nom de la 1ère dépendance
# $(@D) : is an automatic variable which expands to the directory part of the target path