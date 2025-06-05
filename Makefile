# Makefile buat Stock Forecast Program
# Build system simpel pake g++

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
LDFLAGS = -lncurses

# Direktori-direktori
SRCDIR = src
HEADERDIR = src/headers
OBJDIR = obj
BINDIR = bin
DATADIR = data

TARGET = $(BINDIR)/stock_forecast

# File source sama object
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/stock_data.cpp $(SRCDIR)/csv_reader.cpp $(SRCDIR)/regression.cpp $(SRCDIR)/ui_manager.cpp
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Target utama
all: directories $(TARGET)

# Bikin direktori kalo belum ada
directories:
	@mkdir -p $(OBJDIR) $(BINDIR) $(DATADIR)

# Build executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build selesai: $(TARGET)"

# Compile file source jadi object
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(HEADERDIR) -c $< -o $@

# Bersihin file build
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Jalanin program pake sample data
run: $(TARGET)
	./$(TARGET) $(DATADIR)/sample_stock.csv

# Build dan jalanin test UI
test-ui: directories
	$(CXX) $(CXXFLAGS) -I$(HEADERDIR) test/test_ui_init.cpp $(SRCDIR)/ui_manager.cpp -o $(BINDIR)/test_ui $(LDFLAGS)
	@echo "Test UI built: $(BINDIR)/test_ui"
	@echo "Jalanin: ./$(BINDIR)/test_ui"

# Jalanin test UI langsung
run-test: test-ui
	./$(BINDIR)/test_ui

.PHONY: all directories clean run test-ui run-test 