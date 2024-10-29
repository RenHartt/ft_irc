NAME = ircserv

SRCS = $(shell find . -name "*.cpp")
OBJS_DIR = .objs
OBJS = $(patsubst %.cpp, $(OBJS_DIR)/%.o, $(SRCS))

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -I inc -g -MMD -MP

TOTAL_FILES := $(words $(OBJS))
COMPILED_FILES = 0

GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

all: gen_perm $(OBJS) $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) has been successfully built!$(RESET)"

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
	@printf "$(BLUE)Compiling %-40s$(RESET) [$(YELLOW)%2d$(RESET)/$(YELLOW)%d$(RESET)] ($(GREEN)%3d%%$(RESET))\n" "$<" $(COMPILED_FILES) $(TOTAL_FILES) $$(($(COMPILED_FILES) * 100 / $(TOTAL_FILES)))
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

gen_perm:
	./perm.sh

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	rm -rf $(OBJS_DIR)

fclean: clean
	@echo "$(YELLOW)Removing $(NAME)...$(RESET)"
	rm -f $(NAME)

re: fclean all

.SILENT:
.PHONY: all clean fclean re
