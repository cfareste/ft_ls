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
CCFLAGS += -g -Wall -Wextra -Werror -fsanitize=address -Dopendir=mock_opendir -Dreaddir=mock_readdir -Dclosedir=mock_closedir -Dft_printf=printf_mock -Dstat=stat_mock
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage


#----DIRS----#
BIN_DIR = bin/
SRC = src/
INC = inc/
SUITES_DIR = $(SRC)/suites
MOCKS_DIR = $(SRC)/mocks
REPORTS_DIR = reports/


#----Reports----#
REPORT_OUTPUT_MODE ?= file


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
			$(ROOT_SRC)/application:\
			$(ROOT_SRC)/parsed_arguments:\
			$(ROOT_SRC)/filesystem:\
			$(ROOT_SRC)/file_entry_list:\
			$(ROOT_SRC)/scanner:\
			$(ROOT_SRC)/renderer



#----SHARED----#
ROOT_SRCS = application.c \
			parsed_arguments.c \
			directory.c \
			file_stats.c \
			file_entry_list.c \
			file_entry_list_factory.c \
			scanner.c \
			renderer.c

SRCS = main.c \
		directory_mock.c \
		print_mock.c \
		stat_mock.c \
		directory_suite.c \
		file_entry_list_suite.c \
		file_entry_list_factory_suite.c \
		renderer_suite.c \
		file_stats_suite.c \
		parsed_arguments_suite.c \
		application_suite.c \
		$(ROOT_SRCS)

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
	@rm -rf $(CUNIT_DIR) $(REPORTS_DIR)
	@echo "$(RED)Tester deleted$(RESET)\n"

re: fclean all

cover: clean
	@mkdir -p $(REPORTS_DIR)
	@$(MAKE) --no-print-directory CCFLAGS="$(CCFLAGS) $(COVERAGE_FLAGS)" all
	@echo "$(CYAN)Generating $(WHITE_BOLD)coverage reports$(CYAN)...$(RESET)\n"
	@./tester > /dev/null
	@$(MAKE) --no-print-directory gcov_report REPORT_OUTPUT_MODE="$(REPORT_OUTPUT_MODE)"

gcov_report:
	@mkdir -p $(REPORTS_DIR)
	@gcov -b -o $(BIN_DIR) $(ROOT_SRCS) > /dev/null 2>&1
	@mv -f *.gcov $(REPORTS_DIR) >/dev/null 2>&1 || true
	@./generate_coverage_reports.sh "$(REPORT_OUTPUT_MODE)"
	@$(MAKE) --no-print-directory clean > /dev/null
	@echo "$(GREEN)[✓] $(PINK)Coverage reports saved in $(REPORTS_DIR)$(GREEN)$(RESET)"

run: all
	@echo "$(CYAN)Running $(WHITE_BOLD)tests$(CYAN)...$(RESET)\n"
	@./tester

e2e:
	@echo "$(CYAN)Running $(WHITE_BOLD)E2E tests$(CYAN)...$(RESET)\n"
	@cp ../ft_ls .
	@./run_e2e.sh
	@rm -rf ./ft_ls

.PHONY: all \
	clean \
	fclean \
	re \
	run \
	e2e \
	cover \
	gcov_report \
	setup-cunit

-include $(DEPS)

.SILENT:
