NAME = ircserv

SRC =	src/main.cpp 	\
		src/Execution/Client.cpp \
		src/Execution/Server.cpp \
		src/Execution/String.cpp \
		src/Execution/Logger.cpp \
		src/Execution/Message.cpp \
		src/Execution/Channel.cpp \
		src/Execution/ClientChannelMapping.cpp \
		src/Command/Command.cpp \
		src/Command/CommandFactory.cpp \
		src/Command/Nick.cpp \
		src/Command/Ping.cpp \
		src/Command/User.cpp \
		src/Command/Join.cpp \
		src/Command/Quit.cpp \
		src/Command/Kick.cpp \
		src/Command/Part.cpp \
		src/Command/Pass.cpp \
		src/Command/Privmsg.cpp \
		src/Command/Topic.cpp \
		src/Command/Mode.cpp \
		src/Command/Invite.cpp \
		

OBJDIR = ./objs
OBJ = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRC))
OBJDEPS = $(patsubst %.cpp, $(OBJDIR)/%.d, $(SRC))

CC = c++
RM = rm -f
CPPFLAGS =  -std=c++98 -I./include -I./include/Interfaces -I./include/Execution -I./include/Command
CXXFLAGS += -Wall -Wextra -Werror -g
LDFLAGS += -flto
MAKEFLAGS  = -j12 -o

GREEN		=	\e[92;5;118m
YELLOW		=	\e[93;5;226m
GRAY		=	\e[33;2;37m
RESET		=	\e[0m
CURSIVE		=	\e[33;3m

all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(CURSIVE)$(GRAY)- Compiling $(NAME)... $(RESET)\n"	
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $(NAME)
	@printf "$(GREEN)- Executable ready.\n$(RESET)"

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(OBJDEPS)

run:
	$(MAKE) all
	./$(NAME) $(filter-out $@,$(MAKECMDGOALS))

runv:
	$(MAKE) all
	valgrind --leak-check=full ./$(NAME) $(filter-out $@,$(MAKECMDGOALS))
	
clean:
	$(RM) -rf $(OBJDIR)
	@printf "$(YELLOW)- Objects removed.$(RESET)\n"

fclean: clean
	$(RM) $(NAME)
	@printf "$(YELLOW)- Executable removed.$(RESET)\n"

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re
