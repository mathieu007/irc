NAME = ircserv
SRC =	main.cpp 	\
		src/Client.cpp \
		src/Server.cpp \
		src/Command.cpp \
		src/CommandFactory.cpp \
		src/Nick.cpp \
		src/String.cpp \
		src/Logger.cpp \
		src/Message.cpp \
		src/Ping.cpp

OBJDIR = ./objs
OBJ = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRC))
OBJDEPS = $(patsubst %.cpp, $(OBJDIR)/%.d, $(SRC))

CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -std=c++11 -g -I./include

#Colors:
GREEN		=	\e[92;5;118m
YELLOW		=	\e[93;5;226m
GRAY		=	\e[33;2;37m
RESET		=	\e[0m
CURSIVE		=	\e[33;3m

all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(CURSIVE)$(GRAY)- Compiling $(NAME)... $(RESET)\n"
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)
	@printf "$(GREEN)- Executable ready.\n$(RESET)"

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) -MMD -MP -c $< -o $@

-include $(OBJDEPS)

run: all
	./$(NAME) $(filter-out $@,$(MAKECMDGOALS))
	
clean:
	$(RM) -rf $(OBJDIR)
	@printf "$(YELLOW)- Objects removed.$(RESET)\n"

fclean: clean
	$(RM) $(NAME)
	@printf "$(YELLOW)- Executable removed.$(RESET)\n"

re: fclean $(NAME)

.PHONY: all clean fclean re
