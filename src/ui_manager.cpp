#include "ui_manager.hpp"
#include <iostream>
#include <iomanip>

void UIManager::init() {
    initscr();
    if (has_colors() == FALSE) {
        endwin();
        throw std::runtime_error("Terminal gak support warna nih!");
    }
    
    cbreak();        
    noecho();        
    keypad(stdscr, TRUE); 
    curs_set(0);     
    
    setupColors();
    createWindows();
    refresh();
}

void UIManager::cleanup() {
    if (headerWin) delwin(headerWin);
    if (graphWin) delwin(graphWin);
    if (infoWin) delwin(infoWin);
    if (statusWin) delwin(statusWin);
    headerWin = nullptr;
    graphWin = nullptr;
    infoWin = nullptr;
    statusWin = nullptr;
    endwin();
}

void UIManager::setupColors() {
    start_color();
 
    init_pair(1, COLOR_WHITE, COLOR_BLUE);     
    init_pair(2, COLOR_GREEN, COLOR_BLACK);    
    init_pair(3, COLOR_RED, COLOR_BLACK);        
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);  
    init_pair(5, COLOR_CYAN, COLOR_BLACK);    
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); 
    init_pair(7, COLOR_WHITE, COLOR_RED);      
}

void UIManager::createWindows() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int headerHeight = 3;
    int statusHeight = 2;
    int infoWidth = 30;
    headerWin = newwin(headerHeight, maxX, 0, 0);
    wbkgd(headerWin, COLOR_PAIR(1));

    infoWin = newwin(maxY - headerHeight - statusHeight, infoWidth, 
                     headerHeight, maxX - infoWidth);
    wbkgd(infoWin, COLOR_PAIR(5));    
    graphWin = newwin(maxY - headerHeight - statusHeight, 
                      maxX - infoWidth, headerHeight, 0);
    
    statusWin = newwin(statusHeight, maxX, maxY - statusHeight, 0);
    wbkgd(statusWin, COLOR_PAIR(4));  
    scrollok(graphWin, TRUE);
}

void UIManager::prepareVisualization(const std::vector<StockPoint>& data,
                                    const std::vector<double>& predictions) {
    if (data.empty()) return;
    
    // Hitung batas viewport
    // Set min/max harga buat skala Y-axis
    // Set start/end indices buat X-axis
}

void UIManager::renderAll(const std::vector<StockPoint>& data,
                         const LinearRegression& regression,
                         const std::vector<double>& predictions) {
    // Bersihin semua window
    // Render tiap komponen
    // Refresh semua window
}

void UIManager::renderHeader() {
    // Print judul sama info dasar
}
void UIManager::renderGraph(const std::vector<StockPoint>& data, 
                           const LinearRegression& regression,
                           const std::vector<double>& predictions) {
    // Gambar sumbu
    // Plot data historis
    // Gambar garis regresi
    // Plot prediksi kalo dinyalain
}

void UIManager::renderInfo(const LinearRegression& regression) {
    // Tampilin persamaan regresi
    // Tampilin R-squared
    // Tampilin analisis trend
    // Tampilin prediksi harga berikutnya
}

void UIManager::renderStatus() {
    // Tampilin kontrol yang tersedia
    // "Panah: Navigasi | +/-: Zoom | Spasi: Toggle Prediksi | q: Keluar"
}

// Tunggu input user
int UIManager::waitForInput() {
    return getch();  
}

bool UIManager::processInput(int input) {
    switch (input) {
        // Handle tombol panah buat navigasi
        // Handle +/- buat zoom
        // Handle spasi buat toggle prediksi
        // Handle 'q' buat keluar
        case KEY_QUIT:
            return false;
        default:
            break;
    }
    return true;
}

// Error handling
void UIManager::showError(const std::string& message) {

    if (!statusWin) {
        std::cerr << "Error: " << message << std::endl;
        return;
    }

    werase(statusWin);
    wattron(statusWin, COLOR_PAIR(7));
    mvwprintw(statusWin, 0, 1, "ERROR: %s", message.c_str());
    mvwprintw(statusWin, 1, 1, "Tekan sembarang tombol buat lanjut...");
    wattroff(statusWin, COLOR_PAIR(7));
    wrefresh(statusWin);
    getch();
}



void UIManager::drawAxes() {
    // Gambar X sama Y axes dengan label
}

void UIManager::plotHistoricalData(const std::vector<StockPoint>& data) {
    // Plot harga penutupan sebagai titik atau garis
}


void UIManager::drawRegressionLine(const LinearRegression& regression) {
    // Gambar garis regresi di seluruh grafik
}


void UIManager::plotPredictions(const std::vector<double>& predictions) {
    // Plot prediksi masa depan dengan style beda
}

void UIManager::updateViewport(int direction) {
    // Adjust startIndex sama endIndex berdasarkan arah
}

void UIManager::adjustZoom(bool zoomIn) {
    // Atur zoom level sama update viewport
} 