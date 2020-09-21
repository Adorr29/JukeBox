##
## EPITECH PROJECT, 2019
## JukeBox
## File description:
## Makefile
##

NAME		=	JukeBox

CC		=	g++

SRCDIR		=	Src/
UTYDIR		=	$(SRCDIR)Utility/
INCDIR		=	Include/

SRC		=	$(SRCDIR)Main.cpp \
			$(SRCDIR)SendCommand.cpp \
			$(SRCDIR)JukeBox.cpp \
			$(SRCDIR)User.cpp \
			$(SRCDIR)Error.cpp \
			$(UTYDIR)Globpp.cpp \
			$(UTYDIR)Split.cpp \

OBJ		=	$(SRC:.cpp=.o)

CXXFLAGS	+=	-I $(INCDIR)
CXXFLAGS	+=	-W -Wall -Wextra

LDFLAGS		+=	-l boost_system
LDFLAGS		+=	-l pthread
LDFLAGS		+=	-l sfml-system
LDFLAGS		+=	-l sfml-audio
LDFLAGS		+=	-l sfml-network

%.o		:	%.cpp
			@$(CC) -c -o $@ $< $(CXXFLAGS)

all		:	$(NAME)

$(NAME)		:	$(OBJ)
			@$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

clean		:
			@rm -f $(OBJ)

fclean		:	clean
			@rm -f $(NAME)

re		:	fclean all

debug		:	CXXFLAGS += -g3
debug		:	re
