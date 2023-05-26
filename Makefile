NAME		= ircserv

CC			= c++
FLAGS		= -Wall -Wextra -Werror -std=c++98
RM			= rm -rf

OBJDIR = 	.objects

FILES		= main Server Message Client Server.register

SRC			= $(FILES:=.cpp)
OBJ			= $(addprefix $(OBJDIR)/, $(FILES:=.o))
HEADER		= Server.hpp Message.hpp Numerics.hpp Client.hpp global.hpp

#Colors:
YELLOW		=	\e[1;33m
GREEN		=	\e[1;32m
BLUE		=	\e[1;34m
PURPLE		=	\e[1;35m
UNDER_WHITE	=	\e[4;37m
BOLD_WHITE	=	\e[1;37m
BACK_WHITE	=	\e[0m

all: $(NAME)

$(NAME): $(OBJ) $(HEADER) Makefile
	@$(CC) $(OBJ) -o $(NAME) #-fsanitize=address -g
	@printf "\nI created the executable. Now you can enter $(YELLOW)./ircserv <port> <password> $(BACK_WHITE)to launch it and have fun.\n" 
	@printf "\n\n$(BOLD_WHITE)<<<<<<<<<<<<<<<<< Not clear? Follow these steps >>>>>>>>>>>>>>>>$(BACK_WHITE)\n" 
	@printf "\n$(UNDER_WHITE)Replace <port> with the desired port number on which the IRC server will listen for incoming connections.$(BACK_WHITE)\n"
	@printf "\n$(UNDER_WHITE)Replace <password> with the connection password that will be required for clients to connect to your server.$(BACK_WHITE)\n"
	@printf "\n$(BLUE)Creating and Joining Channels$(BACK_WHITE)\n"
	@printf ".To create a channel, use the command: $(PURPLE)/join #channel_name$(BACK_WHITE)\n"
	@printf ".To join an existing channel, use the command:  $(PURPLE)/join #channel_name$(BACK_WHITE)\n"

$(OBJDIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJDIR) $(OBJ)
	@printf "\nI removed all the object files as you asked\n\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "I removed the executable as you asked\n\n"

re: fclean all

.PHONY: all clean fclean re