NAME = ft_irc

SRCS = $(wildcard src/Classes/*.cpp src/Commands/*.cpp src/*.cpp)
OBJS_DIR = .objs
OBJS = $(SRCS:src/%.cpp=$(OBJS_DIR)/%.o)

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -I inc

all: $(OBJS) $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: src/%.cpp | $(OBJS_DIR)/Classes $(OBJS_DIR)/Commands
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/Classes:
	mkdir -p $(OBJS_DIR)/Classes

$(OBJS_DIR)/Commands:
	mkdir -p $(OBJS_DIR)/Commands

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.SILENT:
.PHONY: all clean fclean re
