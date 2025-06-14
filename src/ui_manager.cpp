#include "ui_manager.hpp"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <mergeSort.hpp>
#include <windows.h>
#include <numeric>

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
        erase();  
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
        if (headerWin) wrefresh(headerWin); 
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

            case 27: 
            case 'q':
            case 'Q':
                return ""; // Batal
                
            default:
                break;
        }
    }
}

void UIManager::LoadingState() {
    erase(); 
    renderTopHeader();
    
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    const char spinner[] = {'|', '/', '-', '\\'};
    const int spinnerSize = 4;
    int centerY = maxY / 2;
    int centerX = maxX / 2;
    
    mvprintw(centerY - 1, centerX - 8, "Memuat Data CSV...");
    refresh();
    if (headerWin) wrefresh(headerWin); 
    
    for (int i = 0; i < 10; i++) {
        mvprintw(centerY, centerX - 1, "   ");
        mvprintw(centerY + 1, centerX - 2, "    ");
        
        attron(COLOR_PAIR(2));
        mvprintw(centerY, centerX, "%c", spinner[i % spinnerSize]);
        attroff(COLOR_PAIR(2));
        
        std::string dots = "";
        for (int j = 0; j <= (i % 4); j++) {
            dots += ".";
        }
        mvprintw(centerY + 1, centerX - 2, "%-4s", dots.c_str());
        
        refresh();
        if (headerWin) wrefresh(headerWin); 
        Sleep(50);
    }
    
    erase(); 
}

void UIManager::displayDataTable(const std::vector<StockPoint>& dataArr) {
    LoadingState();
    if (dataArr.empty()) {
        showError("Tidak ada data untuk ditampilkan");
        return;
    }
    int currentRow = 0;
    int scrollOffset = 0;
    bool exitTable = false;
    bool isSorted = false;
    std::vector<StockPoint> data = dataArr;
    // dataArr data constant dia yang nanti di pake di renderResult, "data" cuman copyannya aja biar bisa di maenin ama merge
    while (!exitTable) {  
        erase(); 
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
                 //Siapapun bikin dong ini sort untuk variabel yang lain (Harga Open, High, Low, Volume) kalau mau tambahin dah sortnya dari kecil ke besar juga mo makan 
        mvprintw(maxY-2, 1, "W/S: Navigate | Page Up/Down: Fast scroll | Enter: View details | Spasi: Lanjut ke Prediksi | Esc/Q: Back | P: Sort (Harga Close)");
        attroff(COLOR_PAIR(4));
        
        refresh();
        if (headerWin) wrefresh(headerWin);

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
                
            case KEY_PPAGE: 
                currentRow = std::max(0, currentRow - visibleRows);
                break;
                
            case KEY_NPAGE: 
                currentRow = std::min((int)data.size() - 1, currentRow + visibleRows);
                break;
                
            case KEY_HOME:
                currentRow = 0;
                break;
                
            case KEY_END:
                currentRow = data.size() - 1;
                break;
            case 'p' : 
                if(isSorted){
                    mergeSort(data, 0, data.size() - 1);
                }else{
                    data = dataArr;
                }
                isSorted = !isSorted;
                break;
            case '\n':
            case '\r':
            case KEY_ENTER:
                showRowDetails(data[currentRow]);
                break;
                
            case ' ': 
                exitTable = true;
                break;
                
            case 27: 
            case 'q':
            case 'Q':
                exitTable = true;
                break;
                
            default:
                break;
        }
    }
    
    erase();  
}

void UIManager::showRowDetails(const StockPoint& point) {
    erase();  
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
        mvprintw(centerY+10, centerX, "Status     : NAIK");
        attroff(COLOR_PAIR(2));
    } else if (change < 0) {
        attron(COLOR_PAIR(3)); 
        mvprintw(centerY+10, centerX, "Status     : TURUN");
        attroff(COLOR_PAIR(3));
    } else {
        attron(COLOR_PAIR(4)); 
        mvprintw(centerY+10, centerX, "Status     : STABIL");
        attroff(COLOR_PAIR(4));
    }
    attron(COLOR_PAIR(4));
    mvprintw(maxY-2, 1, "Tekan sembarang tombol untuk kembali ke tabel...");
    attroff(COLOR_PAIR(4));
    
    refresh();
    if (headerWin) wrefresh(headerWin);  
    getch();
}

void UIManager::cleanup() {
    if (headerWin) delwin(headerWin);
    if (infoWin) delwin(infoWin);
    if (statusWin) delwin(statusWin);
    if (resultWin) delwin(resultWin);
    headerWin = nullptr;
    infoWin = nullptr;
    statusWin = nullptr;
    resultWin = nullptr;
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
    int headerHeight = 1;
    int statusHeight = 3;
    int infoWidth = 30;

    if (headerWin) delwin(headerWin);
    if (infoWin) delwin(infoWin);
    if (statusWin) delwin(statusWin);
    if (resultWin) delwin(resultWin);
    
    headerWin = newwin(headerHeight, maxX, 0, 0);
    if (headerWin) wbkgd(headerWin, COLOR_PAIR(1));

    infoWin = newwin(maxY - headerHeight - statusHeight, infoWidth, 
                     headerHeight + 1, maxX - infoWidth);
    if (infoWin) wbkgd(infoWin, COLOR_PAIR(5));    
    
    statusWin = newwin(statusHeight, maxX, maxY - statusHeight, 0);
    if (statusWin) wbkgd(statusWin, COLOR_PAIR(4));

    resultWin = newwin(maxY - headerHeight - statusHeight, maxX - infoWidth,
                      headerHeight, 0);
    if (resultWin) {
        wbkgd(resultWin, COLOR_PAIR(6));
        box(resultWin, 0, 0);  
    }
}

void UIManager::renderResult(const std::vector<StockPoint>& data,
                           const LinearRegression& regression,
                           const std::vector<double>& predictions) {
    if (!resultWin) return;
    
    int maxY, maxX;
    getmaxyx(resultWin, maxY, maxX);
    
    wattron(resultWin, COLOR_PAIR(1));
    mvwprintw(resultWin, 1, 2, "=== INFORMASI REGRESI ===");
    wattroff(resultWin, COLOR_PAIR(1));
    mvwprintw(resultWin, 2, 2, "Persamaan: %s", regression.getEquation().c_str());
    mvwprintw(resultWin, 3, 2, "R-squared: %.4f", regression.getRSquared());
    mvwprintw(resultWin, 4, 2, "Trend: %s", regression.getTrend().c_str());
    
    wattron(resultWin, COLOR_PAIR(1));
    mvwprintw(resultWin, 6, 2, "=== PREDIKSI 10 HARI KEDEPAN ===");
    wattroff(resultWin, COLOR_PAIR(1));
    
    wattron(resultWin, COLOR_PAIR(5));
    mvwprintw(resultWin, 7, 2, "%-8s %-12s %-12s %-12s", 
             "Hari", "Prediksi", "Perubahan", "Persentase");
    wattroff(resultWin, COLOR_PAIR(5));
    mvwprintw(resultWin, 8, 2, std::string(maxX-4, '-').c_str());
    
    double lastPrice = data.back().close;
    int lastIndex = data.size() - 1;
    double slope = regression.getSlope();
                
    for (int i = 0; i < 10; i++) {
        double predPrice = regression.predict(lastIndex + i + 1);
        double change = predPrice - lastPrice;
        double changePercent = (lastPrice != 0) ? (change / lastPrice) * 100 : 0;
        
        mvwprintw(resultWin, 9 + i, 2, "%-8d %-12.2f", i + 1, predPrice);
        
        if (slope > 0.1) { 
            wattron(resultWin, COLOR_PAIR(2));
            mvwprintw(resultWin, 9 + i, 22, "+%.2f", change);
            mvwprintw(resultWin, 9 + i, 34, "(+%.2f%%)", changePercent);
            wattroff(resultWin, COLOR_PAIR(2));
        } else if (slope < -0.1) { 
            wattron(resultWin, COLOR_PAIR(3));
            mvwprintw(resultWin, 9 + i, 22, "%.2f", change);
            mvwprintw(resultWin, 9 + i, 34, "(%.2f%%)", changePercent);
            wattroff(resultWin, COLOR_PAIR(3));
        } else { 
            wattron(resultWin, COLOR_PAIR(4));
            mvwprintw(resultWin, 9 + i, 22, "%.2f", change);
            mvwprintw(resultWin, 9 + i, 34, "(%.2f%%)", changePercent);
            wattroff(resultWin, COLOR_PAIR(4));
        }
    }
}

void UIManager::renderAll(const std::vector<StockPoint>& data,
                         const LinearRegression& regression,
                         const std::vector<double>& predictions) {
    bool exitView = false;
    
    while (!exitView) {
        erase();
        refresh();
        
        renderTopHeader();
        if (headerWin) wrefresh(headerWin);
        
        if (resultWin) {
            werase(resultWin);
            box(resultWin, 0, 0);
            renderResult(data, regression, predictions);
            wrefresh(resultWin);
        }
        
        if (statusWin) {
            werase(statusWin);
            wattron(statusWin, COLOR_PAIR(4));
            mvwprintw(statusWin, 0, 1, "Spasi: Kembali ke Tabel Data | Q: Keluar");
            wattroff(statusWin, COLOR_PAIR(4));
            wrefresh(statusWin);
        }
        
        int ch = getch();
        switch (ch) {
            case ' ':
                displayDataTable(data); 
                break;
            case 'q':
            case 'Q':
            case 27: 
                exitView = true;
                break;
            default:
                break;
        }
    }
}

void UIManager::renderTopHeader() {
    if (!headerWin) return;
    
    int maxY, maxX;
    getmaxyx(headerWin, maxY, maxX);
    werase(headerWin);
    wattron(headerWin, COLOR_PAIR(1));
    mvwprintw(headerWin, 0, 0, std::string(maxX, ' ').c_str());
    mvwprintw(headerWin, 0, 2, "== Stock Forecast By Kelompok 4 (Anjay) ==");
    wattroff(headerWin, COLOR_PAIR(1));
}

void UIManager::renderInfo(const LinearRegression& regression, const std::vector<double>& predictions) {
    if (!infoWin) return;

    werase(infoWin);
    
    wattron(infoWin, COLOR_PAIR(1));
    mvwprintw(infoWin, 0, 1, "=== INFO PANEL ===");
    mvwprintw(infoWin, 2, 1, "Persamaan Regresi:");
    mvwprintw(infoWin, 3, 1, "%s", regression.getEquation().c_str());
    mvwprintw(infoWin, 5, 1, "R-squared: %.4f", regression.getRSquared());
    mvwprintw(infoWin, 6, 1, "Trend: %s", regression.getTrend().c_str());

    if (!predictions.empty()) {
        mvwprintw(infoWin, 8, 1, "Prediksi 3 Hari:");
        int maxPredictions = std::min(3, (int)predictions.size());
        for (int i = 0; i < maxPredictions; i++) {
            mvwprintw(infoWin, 9 + i, 1, "Hari %d: %.2f", i + 1, predictions[i]);
        }
    }
    
    wattroff(infoWin, COLOR_PAIR(1));
    wrefresh(infoWin);
}

void UIManager::renderStatus() {
    if (!statusWin) return;
    wattron(statusWin, COLOR_PAIR(4));
    mvwprintw(statusWin, 0, 1, "W/S: Navigasi | Page Up/Down: Scroll Cepat | Enter: Detail Baris | q: Keluar");
    mvwprintw(statusWin, 1, 1, "Home/End: Awal/Akhir Data | Prediksi: %s ", 
              showPredictions ? "ON" : "OFF");
    wattroff(statusWin, COLOR_PAIR(4));
}

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
    waitForInput();
}

int UIManager::waitForInput() {
    return getch();  
}

bool UIManager::processInput(int input) {
    switch (input) {
        case 'q':
        case 'Q':
        case 27:         
            return false;
        default:
            break;
    }
    return true;
} 