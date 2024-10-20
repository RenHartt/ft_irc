NAME = ft_irc

SRCS = $(wildcard src/*.cpp)
OBJS_DIR = src/.objs
OBJS = $(SRCS:src/%.cpp=$(OBJS_DIR)/%.o)

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -I include

all: $(OBJS) $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: src/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.SILENT:
.PHONY: all clean fclean re bonus
