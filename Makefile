NAME = ircserv
BOT_NAME = ircbot

SRCS = $(shell find ./src -name "*.cpp")
BOT_SRCS = $(shell find ./src_bonus -name "*.cpp")

OBJS_DIR = .objs
OBJS = $(patsubst ./src/%.cpp, $(OBJS_DIR)/src/%.o, $(SRCS))
BOT_OBJS = $(patsubst ./src_bonus/%.cpp, $(OBJS_DIR)/src_bonus/%.o, $(BOT_SRCS))

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -I inc -g -MMD -MP
TOTAL_FILES := $(words $(OBJS))
TOTAL_BOT_FILES := $(words $(BOT_OBJS))

GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

all: $(NAME)

bonus: all $(BOT_NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) has been successfully built!$(RESET)"

$(BOT_NAME): $(BOT_OBJS)
	$(CXX) $(CXXFLAGS) $(BOT_OBJS) -o $(BOT_NAME)
	@echo "$(GREEN)$(BOT_NAME) has been successfully built!$(RESET)"

FILE_INDEX := 0
BOT_FILE_INDEX := 0

$(OBJS_DIR)/src/%.o: src/%.cpp | $(OBJS_DIR)
	@mkdir -p $(dir $@)
	@$(eval FILE_INDEX=$(shell echo $$(($(FILE_INDEX)+1))))
	@printf "$(BLUE)Compiling %-40s$(RESET) [$(YELLOW)%2d$(RESET)/$(YELLOW)%d$(RESET)] ($(GREEN)%3d%%$(RESET))\n" \
	"$<" $(FILE_INDEX) $(TOTAL_FILES) $$(($(FILE_INDEX) * 100 / $(TOTAL_FILES)))
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR)/src_bonus/%.o: src_bonus/%.cpp | $(OBJS_DIR)
	@mkdir -p $(dir $@)
	@$(eval BOT_FILE_INDEX=$(shell echo $$(($(BOT_FILE_INDEX)+1))))
	@printf "$(BLUE)Compiling %-40s$(RESET) [$(YELLOW)%2d$(RESET)/$(YELLOW)%d$(RESET)] ($(GREEN)%3d%%$(RESET))\n" \
	"$<" $(BOT_FILE_INDEX) $(TOTAL_BOT_FILES) $$(($(BOT_FILE_INDEX) * 25 / $(TOTAL_BOT_FILES) + 100))
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
.PHONY: all clean fclean re bonus
