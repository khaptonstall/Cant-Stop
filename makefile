BUILD		:=		build

CXXFLAGS	:=		-std=c++11
SOURCES		:=		main.cpp GameState.cpp Player.cpp Human_Player.cpp			\
					AI_Player.cpp probability_player.cpp dice_probability.cpp	\
					rollout_player.cpp
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

$(TARGET): $(OBJPATHS)
	@$(CXX) $(CXXFLAGS) $(OBJPATHS) $(INCLUDE) $(LIBPATHS) $(LIBS) -o $(TARGET)
	@echo Linking and building $(TARGET)

build/%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
	@echo Compiling $(<:.cpp=)

clean:
	@echo Cleaning...
	@rm -fr $(TARGET) $(OBJPATHS)

rebuild: clean
	@$(MAKE) all

.SECONDARY: