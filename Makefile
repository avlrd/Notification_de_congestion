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
# We look in the directory specified by -I and exhaustive warnings:
CFLAGS  = -I $(IPATH) -Wall -Wextra -Werror

# Define 
# ------------------------

define utilS =
$(info ) $(info Bienvenue $(whoami) !!)
$(info ) $(info Utilisation simple : make) 
$(info ) $(info Utilisation complexe : make infos) 
endef
ifeq ($(MAKELEVEL),0)
$(call utilS)
endif 

define utilC =
$(info ) $(info Utilisation complexe :  'make' ET/OU 'NAME=<binName>' ET/OU 'TFP=<path>' (TFP=src par défaut) ET/OU {'MODE = MF' ET 'TFM=<'filename1 filename2 etc'>'} )
$(info ) $(info Exemple	complexe     :   make NAME=binaryFile TFP=sourceChemin MODE=MF TFM='filename1 filename2 etc')
$(info ) $(info Nettoyer bin et obj  :   make clean) 
$(info )
endef

define infoError =
$(info Information utile : Saisissez correctement le nom des fichiers et du chemin)
$(error Effectuez        : 'make' ET/OU 'NAME=<binName>' ET/OU 'TFP=<path>' (TFP=src par défaut) ET/OU {'MODE = MF' ET 'TFM=<filename/s>'})
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
endif

# PHONY 
# ------------------------
# Without PHONY, the target will not work properly if a file 
# named all/clean is ever created in this directory.
.PHONY : all run clean infos

# TARGETS
# ------------------------
# all run  clean
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
	$(call utilC)

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