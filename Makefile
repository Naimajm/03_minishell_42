# valgrind --leak-check=full --track-origins=yes ./minishell

# OUTPUT COLORS
DEF_COLOR           = \033[0;39m
ORANGE              = \033[38;5;209m
MID_GRAY            = \033[38;5;245m
DARK_GREEN          = \033[38;2;75;179;82m

# PROGRAM NAME 
NAME 			:= minishell

# ARCH.H
INCLUDES_DIR 	:= ./inc 

# LIBFT
LIBFT_DIR 		:= ./lib/libft
LIBFT_ARCHIVE 	:= libft.a

# GNL
GNL_DIR 		:= ./lib/gnl
GNL_ARCHIVE 	= libgnl.a

# C FLAGS
CC 				:= cc
CFLAGS 			:= -Wall -Wextra -Werror

#DIRECTORIES TO SEARCH HEADERS FILES .H
INCLUDES		:= -I$(LIBFT_DIR) -I$(GNL_DIR) -I$(INCLUDES_DIR)

#LIBRARIES UBICATION
EXT_LIBRARYS 	:= $(GNL_DIR)/$(GNL_ARCHIVE) $(LIBFT_DIR)/$(LIBFT_ARCHIVE) -lreadline

# SRC --------------------------------------------------------------------------------
SRC_DIR			:= ./src
SRC_FILES 		:= $(addprefix $(SRC_DIR)/, 00_main.c 01_execute_shell.c) 
OBJ_FILES 		:= $(SRC_FILES:%.c=%.o)

# PARSER --------------------------------------------------------------------------------

PAR_DIR			:= ./src/parser
PAR_FILES 		:= $(addprefix $(PAR_DIR)/,  01.1_check_syntax.c \
				02_command_generate.c  02.1_command_check.c 02.2_command_builder.c \
				03_lexical_analyzer.c 03.1_word_builder.c \
				04_tokenizer.c 04.1_token_builder.c \
				05_variable_expander.c \
				05.1_expand_list.c 05.2_expand_extractor.c 05.3_expand_builder.c 05.4_dequotizer.c \
				06_word_processor.c \
				07_semantic_check.c \
				08_execution_builder.c 08.1_execution_redict.c \
				09_free_manager.c \
				10_utils_core.c 10.1_utils_strings.c 10.2_utils_basic.c )

OBJ_FILES 		+= $(PAR_FILES:%.c=%.o)
# --------------------------------------------------------------------------
#ifeq (0,1)
# EXECUTOR-----------------------------------------------------------------------------
EXC_DIR			:= ./src/executor
EXC_FILES		:= $(addprefix $(EXC_DIR)/, 00_utils_clean.c \
				00_utils.c \
				01_signals.c \
				02.00_executer.c \
				02.01_executer_command.c \
				03_redirections.c \
				03.01_heredoc.c)

OBJ_FILES		+= $(EXC_FILES:%.c=%.o)
# --------------------------------------------------------------------------
#endif

#ifeq (0,1)
# BUILTIN --------------------------------------------------------------------------------
BTN_DIR			:= ./src/builtin
BTN_FILES		:= $(addprefix $(BTN_DIR)/, 00_exec_builtins.c \
				00_utils_builtins_00.c \
				00_utils_builtins_01.c \
				00_utils_builtins_02.c \
				00_utils_builtins_03.c \
				01_exec_echo.c \
				02_exec_cd.c \
				03_exec_pwd.c \
				04_exec_export.c \
				05_exec_unset.c \
				06_exec_env.c \
				07_exec_exit.c )

OBJ_FILES		+= $(BTN_FILES:%.c=%.o)
#endif
# --------------------------------------------------------------------------
all: $(NAME)

$(NAME):  $(OBJ_FILES)
	@make library
	@echo "$(ORANGE)🚀​ Compiling $(NAME)... $(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(INCLUDES) $(EXT_LIBRARYS) -o $(NAME)
	@echo "$(DARK_GREEN)📚 $(NAME) has been created			OK$(DEF_COLOR)"

library:
	@echo "$(ORANGE)📦 Packing external libraries...$(DEF_COLOR)"
	@make -s -C $(LIBFT_DIR)
	@make -s -C $(GNL_DIR)
	@echo "$(DARK_GREEN)📦 external libraries created			OK$(DEF_COLOR)"

clean:
	@rm -f $(OBJ_FILES)	
	@make clean -s -C $(LIBFT_DIR)
	@make clean -s -C $(GNL_DIR)
	@echo "${MID_GRAY}Cleaning objects $(NAME)			OK$(DEF_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@make fclean -s -C $(LIBFT_DIR)
	@make fclean -s -C $(GNL_DIR)
	@echo "$(MID_GRAY)Cleaning $(NAME)				OK$(DEF_COLOR)"

debug: CFLAGS += -g -fsanitize=address
debug: EXT_LIBRARYS += -fsanitize=address
debug: re
# ignore external leaks: ASAN_OPTIONS=detect_leaks=0 make debug

re: fclean all
	@echo "$(DARK_GREEN)🔁 Cleaning and recompiled -> $(NAME) 	OK$(DEF_COLOR)"	
	@make -s clean

.PHONY: all library clean fclean re debug
