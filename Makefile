NAME = ft_ls


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


#----DIRS----#
BIN_DIR = bin/
SRC = src/
INC = inc/
TEST_DIR = test/


#----COMPILER----#
CC = cc
CCFLAGS += -Wall -Werror -Wextra
INCLUDES = -I$(INC)


#----LIBFT----#
LIBFT_DIR = lib/libft/
LIBFT_LIB = $(LIBFT_DIR)libft.a
INCLUDES += -I$(LIBFT_DIR)


#----MACROS----#
export GNL_BUFFER_SIZE := 50000


#----VPATH----#
vpath %.c	$(SRC)


#----SHARED----#
SRCS = ft_ls.c

OBJS = $(SRCS:%.c=$(BIN_DIR)%.o)
DEPS = $(OBJS:%.o=%.d)



#----- R U L E S -----#

all:
	@$(MAKE) --no-print-directory make_libft
	@$(MAKE) --no-print-directory $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJS)
	@printf "$(BLUE)Linking objects and creating program...$(RESET)\n"
	$(CC) $(CCFLAGS) $(OBJS) $(LIBFT_LIB) -o $(NAME)
	@echo "$(GREEN)[✓] $(PINK)$(NAME)$(GREEN) created$(RESET)"

$(BIN_DIR)%.o: %.c Makefile
	@printf "$(CYAN)Compiling: $(PINK)$(notdir $<)...$(RESET)\n"
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CCFLAGS) $(INCLUDES) -MMD -c $< -o $@

clean: libft_clean test_clean
	@rm -rf $(BIN_DIR)
	@echo "$(RED)Binaries deleted$(RESET)\n"

fclean: clean libft_fclean test_fclean
	@rm -rf $(NAME)
	@echo "$(RED)Executable deleted$(RESET)\n"

re: fclean all

bonus:
	$(MAKE) --no-print-directory all

b: bonus

bonusre: fclean bonus

bre: bonusre

test: make_libft
	@$(MAKE) --no-print-directory -C $(TEST_DIR) run

test_clean:
	@$(MAKE) --no-print-directory -C $(TEST_DIR) clean

test_fclean:
	@$(MAKE) --no-print-directory -C $(TEST_DIR) fclean

make_libft:
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) bonus

libft_clean:
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

libft_fclean:
	@echo "$(RED)Cleaning $(PINK)Libft$(RED)...$(RESET)"
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

.PHONY: all \
		clean \
		fclean \
		re \
		bonus \
		b \
		bonusre \
		bre \
		test \
		test_clean \
		test_fclean \
		make_libft \
		libft_clean \
		libft_fclean \

-include $(DEPS)

.SILENT:
