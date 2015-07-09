BUILD		:=		build

CXXFLAGS	:=		-std=c++11
SOURCES		:=		$(notdir $(wildcard *.cpp)) views/log_view.cpp source/players/cpu_player.cpp \
					source/players/simple_player.cpp
OBJECTS		:=		$(SOURCES:.cpp=.o)
TARGET		:=		cantstop

#===

INCLUDES	:=		/usr/local/include
LIBDIRS		:=		/usr/local/lib
LIBNAMES	:=		SDL2 SDL2_image SDL2_TTF

#===

INCLUDE		:=		$(foreach dir,$(INCLUDES),-I$(dir))
LIBPATHS	:=		$(foreach dir,$(LIBDIRS),-L$(dir))
LIBS		:=		$(foreach dir,$(LIBNAMES),-l$(dir))

#===

OBJPATHS	:=		$(foreach obj,$(OBJECTS),build/$(obj))

all: $(SOURCES) $(TARGET)

run: $(TARGET)
	@./$(TARGET)

run4: $(TARGET)
	@./$(TARGET) & ./$(TARGET) & ./$(TARGET) & ./$(TARGET)

run5: $(TARGET)
	@./$(TARGET) & ./$(TARGET) & ./$(TARGET) & ./$(TARGET) & ./$(TARGET)

run10: $(TARGET)
	@./$(TARGET) & ./$(TARGET) & ./$(TARGET) & ./$(TARGET) & ./$(TARGET) & ./$(TARGET) & ./$(TARGET) & ./$(TARGET) & ./$(TARGET) & ./$(TARGET)

$(TARGET): $(OBJPATHS)
	@$(CXX) $(CXXFLAGS) $(OBJPATHS) $(INCLUDE) $(LIBPATHS) $(LIBS) -o $(TARGET)
	@echo Linking and building $(TARGET)

build/%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
	@echo Compiling $<

clean:
	@echo Cleaning...
	@rm -fr $(TARGET) $(OBJPATHS)

rebuild: clean
	@$(MAKE) all

.SECONDARY: