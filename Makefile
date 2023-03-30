CC = g++
CFLAGS = -Wall -Wextra -std=c11 -fPIC
SRCDIR = ./src
OBJDIR = out
SOURCES = $(shell find $(SRCDIR) -name "*.cpp")
OBJECTS = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.cpp=.o))
EXECUTABLE = $(OBJDIR)/myprogram

LIB_NAME=json_parser
INSTALL_DIR=/usr/local

all: $(EXECUTABLE)
	./$(EXECUTABLE)

install:
	install -d $(INSTALL_DIR)/include/$(LIB_NAME)
	find $(SRCDIR) -name '*.h' -exec install -m 644 {} $(INSTALL_DIR)/include/$(LIB_NAME) \;
	install -m 644 $(LIB_NAME).so $(INSTALL_DIR)/lib
	ldconfig $(INSTALL_DIR)/lib


uninstall:
	$(RM) $(INSTALL_DIR)/include/$(LIB_NAME)/*.h
	rmdir $(INSTALL_DIR)/include/$(LIB_NAME)
	$(RM) $(INSTALL_DIR)/lib/$(LIB_NAME).so
	ldconfig $(INSTALL_DIR)/lib


$(LIB_NAME).so: $(OBJECTS)
	$(CC) -shared -o $(LIB_NAME).so $(OBJECTS)



$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(LIB_NAME).so
