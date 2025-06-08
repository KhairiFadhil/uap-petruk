#include "ui_manager.hpp"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <windows.h>

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


std::string UIManager::selectFile() {
    std::vector<std::string> files;
    std::vector<std::string> dirs;
    std::filesystem::path currentPath = std::filesystem::current_path();
    int selectedIndex = 0;
    int scrollOffset = 0;
    while (true) {
        clear();
        files.clear();
        dirs.clear();   
        try {
            if (currentPath.has_parent_path() && currentPath != currentPath.root_path()) {
                dirs.push_back("..");
            }
        
            for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
                if (entry.is_directory()) {
                    dirs.push_back(entry.path().filename().string());
                } else if (entry.is_regular_file()) {
                    std::string filename = entry.path().filename().string();
                    if (filename.length() >= 4 && 
                        filename.substr(filename.length() - 4) == ".csv") {
                        files.push_back(filename);
                    }
                }
            }
        } catch (const std::exception& e) {
            showError("Tidak bisa membaca directory: " + std::string(e.what()));
            return "";
        }
        std::sort(dirs.begin(), dirs.end());
        std::sort(files.begin(), files.end());
        std::vector<std::string> allItems;
        std::vector<bool> isDirectory;
        for (const auto& dir : dirs) {
            allItems.push_back("[DIR] " + dir);
            isDirectory.push_back(true);
        }
        for (const auto& file : files) {
            allItems.push_back(file);
            isDirectory.push_back(false);
        }
        if (allItems.empty()) {
            allItems.push_back("(Tidak ada file CSV atau folder)");
            isDirectory.push_back(false);
        }
        
        if (selectedIndex >= (int)allItems.size()) {
            selectedIndex = allItems.size() - 1;
        }
        if (selectedIndex < 0) selectedIndex = 0;
        
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        renderTopHeader();
        mvprintw(2, 1, "Path: %s", currentPath.string().c_str());
        attron(COLOR_PAIR(4));
        mvprintw(maxY-2, 1, "W/S: Navigasi | Enter: Pilih | Esc: Batal");
        attroff(COLOR_PAIR(4));
        int listStartY = 4;
        int listHeight = maxY - 6;
        int displayStart = scrollOffset;
        int displayEnd = std::min(displayStart + listHeight, (int)allItems.size());
        for (int i = displayStart; i < displayEnd; i++) {
            int y = listStartY + (i - displayStart);
            
            if (i == selectedIndex) {
                attron(COLOR_PAIR(2) | A_REVERSE);
                mvprintw(y, 1, "> %-*s", maxX-4, allItems[i].c_str());
                attroff(COLOR_PAIR(2) | A_REVERSE);
            } else {
                if (isDirectory[i]) {
                    attron(COLOR_PAIR(5));
                } else {
                    attron(COLOR_PAIR(2));
                }
                mvprintw(y, 1, "  %-*s", maxX-4, allItems[i].c_str());
                if (isDirectory[i]) {
                    attroff(COLOR_PAIR(5));
                } else {
                    attroff(COLOR_PAIR(2));
                }
            }
        } 
        refresh();
        int ch = getch();
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                if (selectedIndex > 0) {
                    selectedIndex--;
                    if (selectedIndex < scrollOffset) {
                        scrollOffset = selectedIndex;
                    }
                }
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                if (selectedIndex < (int)allItems.size() - 1) {
                    selectedIndex++;
                    if (selectedIndex >= scrollOffset + listHeight) {
                        scrollOffset = selectedIndex - listHeight + 1;
                    }
                }
                break;
                
            case '\n':
            case '\r':
            case KEY_ENTER:
                if (selectedIndex < (int)allItems.size() && 
                    allItems[selectedIndex] != "(Tidak ada file CSV atau folder)") {
                    
                    if (isDirectory[selectedIndex]) {
                        std::string selectedDir = allItems[selectedIndex].substr(6); // Remove "[DIR] "
                        if (selectedDir == "..") {
                            currentPath = currentPath.parent_path();
                        } else {
                            currentPath = currentPath / selectedDir;
                        }
                        selectedIndex = 0;
                        scrollOffset = 0;
                    } else {
                        std::filesystem::path selectedFile = currentPath / allItems[selectedIndex];
                        return selectedFile.string();
                    
                    }
                }
                break;

            case 27: // ESC
            case 'q':
            case 'Q':
                return ""; // Batal
                
            default:
                break;
        }
    }
}

void UIManager::LoadingState() {
    clear();
    renderTopHeader();
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    const char spinner[] = {'|', '/', '-', '\\'};
    const int spinnerSize = 4;
    int centerY = maxY / 2;
    int centerX = maxX / 2;
    mvprintw(centerY - 1, centerX - 8, "Memuat Data CSV...");
    for (int i = 0; i < 10; i++) {
        mvprintw(centerY, centerX - 1, "   ");
        attron(COLOR_PAIR(2));
        mvprintw(centerY, centerX, "%c", spinner[i % spinnerSize]);
        attroff(COLOR_PAIR(2));
        std::string dots = "";
        for (int j = 0; j <= (i % 4); j++) {
            dots += ".";
        }
        mvprintw(centerY + 1, centerX - 2, "%-4s", dots.c_str());
        
        refresh();
        Sleep(50);
    }
    
    clear();
}

void UIManager::displayDataTable(const std::vector<StockPoint>& data) {
    LoadingState();
    if (data.empty()) {
        showError("Tidak ada data untuk ditampilkan");
        return;
    }
    int currentRow = 0;
    int scrollOffset = 0;
    bool exitTable = false;
    
    while (!exitTable) {
        clear();
        renderTopHeader();
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        mvprintw(2, 1, "Data CSV - Total Records: %zu", data.size());
        attron(COLOR_PAIR(1));
        mvprintw(4, 1, "%-12s %-8s %-8s %-8s %-8s %-12s", 
                 "Date", "Open", "High", "Low", "Close", "Volume");
        attroff(COLOR_PAIR(1));
        mvprintw(5, 1, std::string(maxX-2, '-').c_str());
        int headerSize = 6; 
        int footerSize = 3; 
        int visibleRows = maxY - headerSize - footerSize;
        if (currentRow < scrollOffset) {
            scrollOffset = currentRow;
        }
        if (currentRow >= scrollOffset + visibleRows) {
            scrollOffset = currentRow - visibleRows + 1;
        }
        
        for (int i = 0; i < visibleRows && (scrollOffset + i) < (int)data.size(); i++) {
            int dataIndex = scrollOffset + i;
            int y = headerSize + i;
            if (dataIndex == currentRow) {
                attron(COLOR_PAIR(2) | A_REVERSE);
                mvprintw(y, 0, ">");
            } else {
                mvprintw(y, 0, " ");
            }
            
            const StockPoint& point = data[dataIndex];
            mvprintw(y, 1, "%-12s %8.2f %8.2f %8.2f %8.2f %12ld", 
                     point.date.c_str(), 
                     point.open, 
                     point.high, 
                     point.low, 
                     point.close, 
                     point.volume);
            
            if (dataIndex == currentRow) {
                attroff(COLOR_PAIR(2) | A_REVERSE);
            }
        }

        attron(COLOR_PAIR(4));
        mvprintw(maxY-3, 1, "Row: %d/%zu | Scroll: %d", 
                 currentRow + 1, data.size(), scrollOffset + 1);
        mvprintw(maxY-2, 1, "W/S: Navigate | Page Up/Down: Fast scroll | Enter: View details | Esc/Q: Back");
        attroff(COLOR_PAIR(4));
        
        refresh();

        int ch = getch();
        switch (ch) {
            case KEY_UP:
            case 'w':
            case 'W':
                if (currentRow > 0) currentRow--;
                break;
                
            case KEY_DOWN:
            case 's':
            case 'S':
                if (currentRow < (int)data.size() - 1) currentRow++;
                break;
                
            case KEY_PPAGE: // Page Up
                currentRow = std::max(0, currentRow - visibleRows);
                break;
                
            case KEY_NPAGE: // Page Down
                currentRow = std::min((int)data.size() - 1, currentRow + visibleRows);
                break;
                
            case KEY_HOME:
                currentRow = 0;
                break;
                
            case KEY_END:
                currentRow = data.size() - 1;
                break;
                
            case '\n':
            case '\r':
            case KEY_ENTER:
                showRowDetails(data[currentRow]);
                break;
                
            case 27: // ESC
            case 'q':
            case 'Q':
                exitTable = true;
                break;
                
            default:
                break;
        }
    }
    
    clear();
}

void UIManager::showRowDetails(const StockPoint& point) {
    LoadingState();
    renderTopHeader();
    
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int centerY = maxY / 2 - 3;
    int centerX = maxX / 2 - 20;
    
    attron(COLOR_PAIR(1));
    mvprintw(centerY-1, centerX-2, std::string(44, ' ').c_str());
    mvprintw(centerY-1, centerX, "         DETAIL DATA SAHAM         ");
    attroff(COLOR_PAIR(1));
    
    mvprintw(centerY+1, centerX, "Tanggal      : %s", point.date.c_str());
    mvprintw(centerY+2, centerX, "Harga Buka   : %.2f", point.open);
    mvprintw(centerY+3, centerX, "Harga Tinggi : %.2f", point.high);
    mvprintw(centerY+4, centerX, "Harga Rendah : %.2f", point.low);
    mvprintw(centerY+5, centerX, "Harga Tutup  : %.2f", point.close);
    mvprintw(centerY+6, centerX, "Volume       : %ld", point.volume);
    
    double range = point.high - point.low;
    double change = point.close - point.open;
    double changePercent = (point.open != 0) ? (change / point.open) * 100 : 0;
    
    mvprintw(centerY+8, centerX, "Range Harga: %.2f", range);
    mvprintw(centerY+9, centerX, "Perubahan  : %.2f (%.2f%%)", change, changePercent);
    
    if (change > 0) {
        attron(COLOR_PAIR(2)); 
        mvprintw(centerY+10, centerX, "Status     : NAIK ↑");
        attroff(COLOR_PAIR(2));
    } else if (change < 0) {
        attron(COLOR_PAIR(3)); 
        mvprintw(centerY+10, centerX, "Status     : TURUN ↓");
        attroff(COLOR_PAIR(3));
    } else {
        attron(COLOR_PAIR(4)); 
        mvprintw(centerY+10, centerX, "Status     : STABIL →");
        attroff(COLOR_PAIR(4));
    }
    attron(COLOR_PAIR(4));
    mvprintw(maxY-2, 1, "Tekan sembarang tombol untuk kembali ke tabel...");
    attroff(COLOR_PAIR(4));
    
    refresh();
    getch();
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
    clear();
    if (headerWin) werase(headerWin);
    if (graphWin) werase(graphWin);
    if (infoWin) werase(infoWin);
    if (statusWin) werase(statusWin);
    renderHeader();
    renderGraph(data, regression, predictions);
    renderInfo(regression);
    renderStatus();
    refresh();
    if (headerWin) wrefresh(headerWin);
    if (graphWin) wrefresh(graphWin);
    if (infoWin) wrefresh(infoWin);
    if (statusWin) wrefresh(statusWin);
}

void UIManager::renderTopHeader() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, std::string(maxX, ' ').c_str());
    mvprintw(0, 2, "== Stock Forecast By Kelompok 4 (Anjay) ==");
    attroff(COLOR_PAIR(1));
}

void UIManager::renderHeader() {
    if (!headerWin) return;
    
    werase(headerWin);
    wattron(headerWin, COLOR_PAIR(1));
    
    int maxY, maxX;
    getmaxyx(headerWin, maxY, maxX);
    
    // Background
    for (int i = 0; i < maxY; i++) {
        mvwprintw(headerWin, i, 0, std::string(maxX, ' ').c_str());
    }
    
    // Title
    mvwprintw(headerWin, 0, 2, "== Stock Forecast By Kelompok 4 (Anjay) ==");
    mvwprintw(headerWin, 1, 2, "Analisis Regresi Linear untuk Prediksi Harga Saham");
    
    wattroff(headerWin, COLOR_PAIR(1));
    wrefresh(headerWin);
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