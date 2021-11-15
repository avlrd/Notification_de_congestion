# Shell
# ------------------------
# 
# The program used as the shell is taken from the variable `SHELL'.  If
# this variable is not set in your makefile, the program `/bin/sh' is
# used as the shell. -> uncomment SHELL ONLY IF you need : 

# SHELL := /bin/bash 

# Choosing the Mode
# ------------------------
# 
# MODE    : C (Custom) [Editable according to the needs of each project]
# Mode N  : Normal (All files) 
# Mode MF : Multiple files or one file, but not all files
#
# PROJET  : Reseau -> MODE C by default!
# Mode    : Editable according to the needs :
MODE = C # C (Custom), N (Normal), MF (Multiple Files)

# Miscellaneous variables
# ------------------------
whoami := $(shell whoami)
NPS    := --no-print-directory # No Print directory make in Shell
CC      = gcc # GNU Compiler Collection
CCG     = $(CC) -g # gcc + default debugging info
IPATH   = include
OPATH   = obj/
BPATH   = bin/
NAME    = # Binary file name
# We look in the directory specified by -I and exhaustive warnings:
CFLAGS  = -I $(IPATH) -Wall -Wextra -Werror

# Custom MODE variables
# ------------------------
# 
#MODE ARQ for ./source -> S = stopandwait OR G = gobackn
MOPT = #OPTION for the medium
ifeq ($(strip $(MARQ)),S) 
	MARQS = $(strip stopandwait)
else ifeq ($(strip $(MARQ)),G) 
	MARQS = $(strip gobackn)
endif
# File source, destination and medium.
FMS = $(BPATH)source
FMD = $(BPATH)destination
FME = medium.py 

# Define 
# ------------------------

define utilS =
$(info ) $(info Bienvenue $(whoami) !!)
$(info ) $(info Utilisation simple                : make) 
$(info ) $(info Utilisation complexe              : make infos) 
$(info ) $(info Utilisation custom (PROJET, etc.) : make infosC) 
endef
ifeq ($(MAKELEVEL),0)
$(call utilS)
endif 

define utilComplex =
$(info ) $(info Utilisation complexe :  'make' ET/OU 'NAME=<binName>' ET/OU 'TFP=<path>' (TFP=src par défaut) ET/OU {'MODE = MF' ET 'TFM=<'filename1 filename2 etc'>'} )
$(info ) $(info Exemple	complexe     :   make NAME=binaryFile TFP=sourceChemin MODE=MF TFM='filename1 filename2 etc')
$(info ) $(info Nettoyer bin et obj  :   make clean) 
$(info )
endef

define infoError =
$(info Information utile : Saisissez correctement le nom des fichiers et du chemin)
$(error Effectuez        : 'make' ET/OU 'NAME=<binName>' ET/OU 'TFP=<path>' (TFP=src par défaut) ET/OU {'MODE = MF' ET 'TFM=<filename/s>'})
endef

# Custom MODE define
# ------------------------
# 
define utilCustom =
$(info ) $(info Pour le Projet de Reseau :   MARQ=S -> stopandwait ET MARQ=G -> gobackn)
$(info ) $(info Vous devez effectuer     :   make source MARQ=S OU MARQ=G)
$(info ) $(info Vous devez effectuer     :   make destination)
$(info ) $(info Vous devez effectuer     :   make medium ET/OU MOPT=votreOption)
$(info )
endef

# Target File Path 
# ------------------------

ifneq ($(TFP),) # string not empty so TFP=<path>
	CPATH = $(TFP)/
else ifeq ($(TFP),) 
	CPATH = src/
else 
    $(call infoError)
endif

# MODE (conditions) 
# ------------------------

ifeq ($(strip $(MODE)),N) # OBJETS : list of C replacing the ‘.c’ suffix with ‘.o’ in the result
	ifeq ($(TFM),)
		BIN = $(BPATH)main
		OBJETS = $(patsubst $(CPATH)%.c,$(OPATH)%.o,$(wildcard $(CPATH)*.c))
	else 
        $(call infoError)
	endif
else ifeq ($(strip $(MODE)),MF) 
	ifneq ($(TFM),)
		BIN = $(BPATH)$(word 1,$(TFM))
		OBJETS = $(patsubst $(CPATH)%.c,$(OPATH)%.o,$(addprefix $(CPATH),$(addsuffix .c,$(TFM))))
	else 
        $(call infoError)
	endif
else ifeq ($(strip $(MODE)),C) # Continue
else 
    $(call infoError)
endif

ifneq ($(strip $(NAME)),)
	FILE_EXISTS = $(or $(and $(wildcard $(BPATH)$(NAME)),1),0)
	ifeq ($(FILE_EXISTS),1)
    $(error Saisissez un autre NOM. Exemple : NAME=$(NAME)1)
	else 
		BIN = $(BPATH)$(NAME)
	endif 
endif

# PHONY 
# ------------------------
# Without PHONY, the target will not work properly if a file 
# named all/clean is ever created in this directory.
.PHONY : all run clean infos

# TARGETS
# ------------------------
# all run clean infos
all: 
	@if [ "$(strip $(MODE))" = "N" ]                   \
	 || [ "$(strip $(MODE))" = "MF" ]; then            \
		make $(NPS) run;                               \
	elif [ $(strip $(MODE)) = "C" ]; then              \
		make $(NPS) MODE=MF TFM='source utils';        \
		make $(NPS) MODE=MF TFM='destination utils';   \
	else                                               \
		echo "MODE problem";                           \
		exit 1;                                        \
    fi                                      
                                      
run: $(BIN) 
$(BIN) : $(OBJETS) 
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "\n#### Binaire '$(BIN:$(BPATH)%=%)' crée dans le répertoire : '$(BPATH)' ####\n"

$(OPATH)%.o : $(CPATH)%.c msg_obj_compiling
	@mkdir -p $(@D)
	@$(CCG) $(CFLAGS) -o $@ -c $< 

# INTERMEDIATE, msg, infos
# ---------------------------------
# Single display without redundancy

.INTERMEDIATE: msg_obj_compiling 
msg_obj_compiling:
	@echo "\n#### Objets : '$(OBJETS)' crée(s) dans le répertoire : '$(OPATH)' ####\n"
  
infos :
	$(call utilComplex)

infosC :
	$(call utilCustom)

# CUSTOM : Projet Reseau
# ---------------------------------
# source, destination, medium
# 127.0.0.1 -> standard address for IPv4 loopback traffic

source : 	
# @if [ ! -f $(FMS) ] ; then                         \
	# 	make $(NPS);                                   \
	# fi
	@make $(NPS)
	@if [ "$(strip $(MARQS))" = "" ]; then             \
		echo Effectuez : MARQ=S ou MARQ=G;             \
		exit 1;                                        \
	fi  
	./bin/source $(MARQS) 127.0.0.1 3333 4444
	
destination :
# @if [ ! -f $(FMD) ] ; then                       \
	# 	make $(NPS);                                   \
	# fi   (pour compiler et deboguer pas de fi)
	@make $(NPS)
	./bin/destination 127.0.0.1 6666 5555

medium :
	@if [ ! -f $(FME) ] ; then                         \
		echo Fichier \"medium.py\" manquant;           \
		exit 1;                                        \
	fi
	python3 medium.py $(MOPT)

# clean  
# ---------------------------------
# Rules for cleaning the directory how to delete all the object files and executables so that the directory is ' clean '.

clean :
	@rm -rf $(BPATH) $(OPATH)
	@echo "\n#### Les répertoires '$(OPATH)' et '$(BPATH)' ont été supprimés ####\n"	

# $@    : name of the target that triggers the execution of the command
# $^    : name of all dependencies
# $<    : name of the 1st dependency
# $(@D) : is an automatic variable which expands to the directory part of the target path