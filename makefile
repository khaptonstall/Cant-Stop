# g++ -c ../source/views/log_view.cpp -o ../build/log_view.o -std=c++11 -I../include -I/usr/local/include -I../build -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_TTF

TARGET		:=		cantstop
BUILD		:=		build
SOURCES		:=		source/ source/players/ source/views/ source/controllers/
INCLUDES	:=		include

#===

LIBS		:=		-lSDL2 -lSDL2_image -lSDL2_TTF
LIBDIRS		:=		/usr/local
LIBPATHS	:=		$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

CPPFILES	:=		$(foreach dir,$(SOURCES),$(wildcard $(dir)*.cpp))
OFILES		:=		$(CPPFILES:.cpp=.o)
OBJPATHS	:=		$(foreach obj,$(OFILES),$(BUILD)/$(obj))

INCLUDE		:=		$(foreach dir,$(INCLUDES),-I$(dir))	\
					$(foreach dir,$(LIBDIRS),-I$(dir)/include)

CXXFLAGS	:=		$(INCLUDE) -std=c++11

all: $(TARGET)

clean:
	@echo Cleaning...
	@rm -fr $(TARGET) $(OBJPATHS)

rebuild: clean
	@$(MAKE) all

$(TARGET): $(OFILES)
	@echo Linking and building $(TARGET)
	@$(CXX) $(CXXFLAGS) $(CPPFILES) $(LIBPATHS) $(LIBS) -o $(TARGET)

%.o: %.cpp
	@[ -d $@ ] || mkdir -p $(dir $@)
	@echo Compiling $(notdir $<)
	@$(CXX) $(CXXFLAGS) -c $< -o $(BUILD)/$(notdir $@)

rebuild: clean all

run: $(TARGET)
	@./$(TARGET)

debug:
	@echo $(OFILES)

.SECONDARY: