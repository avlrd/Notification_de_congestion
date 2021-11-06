EXEC = #Binary file 
TFP = #Target File Path => make TF=filename TPF=path/ OU TF=* 
TF = #Target File => make TF=filename OU TF=* par defaut
CC = gcc # GNU Compiler Collection
CCG = $(CC) -g # gcc + informations de débogage par défaut
IPATH = include
OPATH = obj/
CPATH = src/
BPATH = bin/
CFLAGS = -I $(IPATH) -Wall -Wextra #-Werror
# On cherche dans le répertoires spécifié par -I 
# (donc ici notre répertoire "include") et warnings exhaustifs

$(info )
$(info #### make ET/OU TF=<filename> ET/OU TFP=<path> ####)
$(info )

ifneq ($(TF),) #chaine non vide donc TF=<filename>
	EXEC = $(TF)
else ifeq ($(TF),)
	EXEC = main
	TF = *
else
$(info Information utile : Saisissez correctement le nom du fichier)
$(error Effectuez : make TF=<filename> ou (tous les fichiers TF=* ou make))
endif

ifneq ($(CPATH),) #chaine non vide donc TFP=<path>
	CPATH = $(TFP)/
else
$(info Information utile : Saisissez correctement le chemin du fichier)
$(error Effectuez : make et/ou TF=<filename> et/ou TFP=<path> ou (TFP=src par défaut))
endif

# list of C source files into a list of object files by 
# replacing the ‘.c’ suffix with ‘.o’ in the result
OBJETS = $(patsubst $(CPATH)%.c,$(OPATH)%.o,$(wildcard $(CPATH)$(TF).c))

.PHONY : all
all: $(BPATH)$(EXEC)
$(BPATH)$(EXEC): $(OBJETS)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "\n#### Binaire '$(EXEC)' crée dans le répertoire : '$(BPATH)' ####\n"

$(OPATH)%.o : $(CPATH)%.c
	@mkdir -p $(@D)
	@$(CCG) $(CFLAGS) -o $@ -c $< 
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