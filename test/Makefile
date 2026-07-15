NAME = tester


#----COLORS----#
RESET = \033[0m
WHITE_BOLD = \033[1;39m
BLACK = \033[1;30m
RED = \033[1;31m
RED_REGULAR = \033[0;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
PINK = \033[1;35m
CYAN = \033[1;36m


#----COMPILER----#
CC = cc
CCFLAGS += -Wall -Werror -Wextra


#----DIRS----#
BIN_DIR = bin/
SRC = src/
INC = inc/
INCLUDES = -I$(INC)


#----VPATH----#
vpath %.c	$(SRC)


#----SHARED----#
SRCS = main.c

OBJS = $(SRCS:%.c=$(BIN_DIR)%.o)
DEPS = $(OBJS:%.o=%.d)



#----- R U L E S -----#

all:
	@$(MAKE) --no-print-directory $(NAME)

$(NAME): $(OBJS)
	@printf "$(BLUE)Linking objects and creating program...$(RESET)\n"
	$(CC) $(CCFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)[✓] $(PINK)$(NAME)$(GREEN) created$(RESET)"

$(BIN_DIR)%.o: %.c Makefile
	@printf "$(CYAN)Compiling: $(PINK)$(notdir $<)...$(RESET)\n"
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CCFLAGS) $(INCLUDES) -MMD -c $< -o $@

clean:
	@rm -rf $(BIN_DIR)
	@echo "$(RED)Binaries deleted$(RESET)\n"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)Executable deleted$(RESET)\n"

re: fclean all

run: all
	@echo "$(CYAN)Running $(WHITE_BOLD)tests$(CYAN)...$(RESET)\n"
	@./tester

.PHONY: all \
		clean \
		fclean \
		re \
		run


-include $(DEPS)
-include $(MDEPS)
-include $(BDEPS)
.SILENT:
