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
CCFLAGS += -Wall -Wextra -Werror -fsanitize=address


#----DIRS----#
BIN_DIR = bin/
SRC = src/
INC = inc/
SUITES_DIR = $(SRC)/suites
MOCKS_DIR = $(SRC)/mocks


#----CUNIT----#
FRAMEWORKS_DIR = $(CURDIR)/frameworks
CUNIT_DIR = $(FRAMEWORKS_DIR)/CUnit
CUNIT_ROOT = $(CUNIT_DIR)/CUnit
CUNIT_BUILD_DIR = $(CUNIT_DIR)/build
CUNIT_LIB = $(CUNIT_BUILD_DIR)/libcunit.a


#----ROOT PROJ----#
ROOT_DIR = ..
ROOT_SRC = $(ROOT_DIR)/src
ROOT_INC = $(ROOT_DIR)/inc
LIBFT_DIR = $(ROOT_DIR)/libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a


#----INCLUDES----#
INCLUDES = -I$(INC) -I$(ROOT_INC) -I$(LIBFT_DIR) -I$(CUNIT_ROOT)
LDFLAGS = $(LIBFT_LIB)


#----VPATH----#
vpath %.c	$(SRC):\
			$(SUITES_DIR):\
			$(MOCKS_DIR):\
			$(ROOT_SRC)/file_data:\
			$(ROOT_SRC)/scanner



#----SHARED----#
SRCS = main.c \
		opendir_mock.c \
		file_data.c \
		file_data_suite.c \
		scanner.c \
		scanner_suite.c

OBJS = $(SRCS:%.c=$(BIN_DIR)%.o)
DEPS = $(OBJS:%.o=%.d)



#----- R U L E S -----#

all: setup-cunit $(CUNIT_LIB) $(NAME)

$(NAME): $(OBJS) $(CUNIT_LIB)
	@printf "$(BLUE)Linking objects and creating tester...$(RESET)\n"
	@$(CC) $(CCFLAGS) $(OBJS) $(CUNIT_LIB) $(LDFLAGS) -lm -o $(NAME)
	@echo "$(GREEN)[✓] $(PINK)$(NAME)$(GREEN) created$(RESET)"

$(BIN_DIR)%.o: %.c Makefile
	@printf "$(CYAN)Compiling test: $(PINK)$(notdir $<)...$(RESET)\n"
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CCFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(CUNIT_LIB): $(CUNIT_ROOT)/CMakeLists.txt
	@printf "$(CYAN)Configuring CUnit with CMake...$(RESET)\n"
	@cmake -S $(CUNIT_ROOT) -Wno-dev -B $(CUNIT_BUILD_DIR) -DCUNIT_DISABLE_TESTS=ON -DCUNIT_DISABLE_EXAMPLES=ON -DCUNIT_CMAKE_INSTALL=no 2>&1 >/dev/null
	@printf "$(CYAN)Building CUnit library...$(RESET)\n"
	@cmake --build $(CUNIT_BUILD_DIR) --target cunit --parallel >/dev/null

setup-cunit:
	@if [ ! -d "$(FRAMEWORKS_DIR)/CUnit" ]; then \
		git clone --depth 1 https://gitlab.com/cunity/cunit.git $(FRAMEWORKS_DIR)/CUnit >/dev/null || exit 1; \
	fi

clean:
	@rm -rf $(BIN_DIR) $(CUNIT_BUILD_DIR)
	@echo "$(RED)Test binaries deleted$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@rm -rf $(CUNIT_DIR)
	@echo "$(RED)Tester deleted$(RESET)\n"

re: fclean all

run: all
	@echo "$(CYAN)Running $(WHITE_BOLD)tests$(CYAN)...$(RESET)\n"
	@./tester

.PHONY: all \
	clean \
	fclean \
	re \
	run \
	setup-cunit

-include $(DEPS)

.SILENT:
