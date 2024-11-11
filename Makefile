NAME = ircserv
BOT_NAME = ircbot

SRCS = $(shell find ./src -name "*.cpp")
BOT_SRCS = $(shell find ./src_bonus -name "*.cpp")

OBJS_DIR = .objs
OBJS = $(patsubst %.cpp, $(OBJS_DIR)/%.o, $(SRCS))
BOT_OBJS = $(patsubst %.cpp, $(OBJS_DIR)/%.o, $(BOT_SRCS))

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -I inc -g -MMD -MP
TOTAL_FILES := $(words $(OBJS))
COMPILED_FILES = 0

GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

all: $(NAME) $(BOT_NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) has been successfully built!$(RESET)"

$(BOT_NAME): $(BOT_OBJS)
	$(CXX) $(CXXFLAGS) $(BOT_OBJS) -o $(BOT_NAME)
	@echo "$(GREEN)$(BOT_NAME) has been successfully built!$(RESET)"

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@printf "$(BLUE)Compiling %-40s$(RESET) [$(YELLOW)%2d$(RESET)/$(YELLOW)%d$(RESET)] ($(GREEN)%3d%%$(RESET))\n" "$<" $(COMPILED_FILES) $(TOTAL_FILES) $$(($(COMPILED_FILES) * 100 / $(TOTAL_FILES)))
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	rm -rf $(OBJS_DIR)

fclean: clean
	@echo "$(YELLOW)Removing $(NAME) and $(BOT_NAME)...$(RESET)"
	rm -f $(NAME) $(BOT_NAME)

re: fclean all

.SILENT:
.PHONY: all clean fclean re
